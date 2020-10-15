//
//
#include <algorithm>
#include <filesystem>
#include <string_view>

#include <sge/vm/zip_fs.hpp>

SGE_VM_BEGIN

// zip_file

class zip_file: public io {
public:
    zip_file(zip_reader_ptr &reader, int index);
    ~zip_file(void) override;

public:
    bool is_seekable(void) override;
    bool open(int flags) override;
    void close(void) override;
    int64_t size(void) override;
    int64_t seek(int64_t offset, seek_from from) override;
    int64_t read(void *p, int64_t size) override;
    int64_t write(const void *p, int64_t size) override;

private:
    zip_reader_ptr m_reader;
    int m_index;
    byte_array_t m_data;
    int64_t m_pos;
};

zip_file::zip_file(zip_reader_ptr &reader, int index):
    m_reader(reader),
    m_index(index),
    m_pos(0)
{
    SGE_ASSERT(m_reader);
    SGE_ASSERT(m_index >= 0);
}

zip_file::~zip_file(void)
{
    if (is_open())
        close();
}

bool zip_file::is_seekable(void)
{
    return true;
}

bool zip_file::open(int flags)
{
    SGE_ASSERT(m_reader);
    SGE_ASSERT(m_index >= 0);

    if (flags & (FLAG_WRITE | FLAG_CREATE | FLAG_TRUNCATE))
        return false;

    if (!io::open(flags))
        return false;

    zip_reader::stat_t st;
    if (!m_reader->stat(m_index, st)) {
        io::close();
        return false;
    }

    if (!m_reader->extract(st.m_file_index, m_data)) {
        io::close();
        return false;
    }

    m_pos = 0;

    return true;
}

void zip_file::close(void)
{
    m_data.clear();
    m_data.reserve(0);

    io::close();
}

int64_t zip_file::size(void)
{
    SGE_ASSERT(is_open());

    return m_data.size();
}

int64_t zip_file::seek(int64_t offset, seek_from from)
{
    SGE_ASSERT(is_open());

    int64_t pos;

    switch (from) {
    case SEEK_FROM_START:
        pos = offset;
        break;
    case SEEK_FROM_CURRENT:
        pos = m_pos + offset;
        break;
    case SEEK_FROM_END:
        pos = m_data.size() - offset;
        break;
    default:
        return -1;
    }

    m_pos = std::clamp(pos, int64_t(0), int64_t(m_data.size()));

    return m_pos;
}

int64_t zip_file::read(void *p, int64_t size)
{
    SGE_ASSERT(is_open());

    int64_t len = std::min(int64_t(m_data.size()) - m_pos, size);
    if (len > 0) {
        if (p != nullptr)
            memcpy(p, m_data.data() + m_pos, len);
        m_pos += len;
    }

    return len;
}

int64_t zip_file::write(const void *p, int64_t size)
{
    SGE_ASSERT(is_open());

    int64_t len = std::min(int64_t(m_data.size()) - m_pos, size);
    if (len > 0) {
        if (p != nullptr)
            memcpy(m_data.data() + m_pos, p, len);
        m_pos += len;
    }

    return len;
}

// zip_fs

zip_fs::zip_fs(void):
    m_tree(0)
{
}

zip_fs::~zip_fs(void)
{
}

bool zip_fs::is_writable(void)
{
	return false;
}

bool zip_fs::init(const std::string &native_path)
{
    SGE_ASSERT(!m_reader);

	if (!std::filesystem::is_regular_file(native_path))
		return false;

    auto reader = std::make_shared<zip_reader>(native_path.c_str());
    if (!reader || !reader->open())
		return false;

    m_tree.clear();
    m_tree.reserve(0);

    int n = reader->count();
    if (n < 1)
        return true;

    m_tree.reserve(n);

    for (int i = 0; i < n; ++i) {
        zip_reader::stat_t st;
        if (reader->stat(i, st)) {
            if (!populate_node(st))
                return false;
        }
    }

    m_reader = reader;
    set_native_path(native_path);

	return true;
}

zip_fs::type_t zip_fs::type(const std::string &path)
{
    SGE_ASSERT(m_reader);

	if (!check_path(path))
		return TYPE_INVALID;

    zip_reader::stat_t st;
    if (!m_reader->stat(path, st))
        return TYPE_INVALID;

    if (st.m_is_directory)
        return TYPE_DIR;

    return TYPE_FILE;
}

bool zip_fs::enum_dir(const std::string &path, string_list_t &result)
{
    SGE_ASSERT(m_reader);

	if (!check_path(path))
		return false;

    zip_reader::stat_t st;
    if (!m_reader->stat(path, st))
        return false;
    if (!st.m_is_directory)
        return false;

    result.clear();

    for (int i = m_tree[st.m_file_index].first_child; i >= 0; i = m_tree[i].next_sibling)
        result.push_back(m_tree[i].name);

    return true;
}

bool zip_fs::make_dir(const std::string &path)
{
    SGE_ASSERT(m_reader);
    SGE_UNUSED(path);

	return false;
}

io *zip_fs::open_file(const std::string &path, int io_flags)
{
    SGE_ASSERT(m_reader);

	if (!check_path(path))
		return nullptr;

    if (path.back() == '/')
        return nullptr;

    if (io_flags & (io::FLAG_WRITE | io::FLAG_CREATE | io::FLAG_TRUNCATE))
		return nullptr;

    int i = m_reader->index(path);
    if (i < 0)
        return nullptr;

    zip_file *fp = new zip_file(m_reader, i);
	if (fp == nullptr)
		return nullptr;

	if (!fp->open(io_flags)) {
		delete fp;
		return nullptr;
	}

	return fp;
}

bool zip_fs::remove(const std::string &path)
{
    SGE_ASSERT(m_reader);
    SGE_UNUSED(path);

	return false;
}

bool zip_fs::populate_node(const zip_reader::stat_t &st)
{
    int parent = 0;
    std::filesystem::path path(st.m_filename);

    for (auto it(path.begin()); it != path.end(); ++it) {
        if (it->filename().empty())
            break;
        int i = find_child(it->filename(), parent);
        if (i < 0) {
            zip_reader::stat_t st;
            if (!m_reader->stat(it->string(), st))
                return false;
            m_tree[st.m_file_index].name = it->filename();
            m_tree[st.m_file_index].next_sibling = m_tree[parent].first_child;
            m_tree[parent].first_child = st.m_file_index;
        }
        parent = i;
    }

    return true;
}

int zip_fs::find_child(const std::string &name, int parent)
{
    for (int i = m_tree[parent].first_child; i >= 0; i = m_tree[i].next_sibling) {
        if (name == m_tree[i].name)
            return i;
        i = m_tree[i].next_sibling;
    }

    return -1;
}

SGE_VM_END
