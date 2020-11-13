//
//
#include <cctype>

#include <sge/file.hpp>
#include <sge/vm/fs.hpp>

SGE_VM_BEGIN

fs::fs(void)
{
}

fs::~fs(void)
{
}

bool fs::is_writable(void)
{
    return true;
}

bool fs::init(const std::string &native_path)
{
    SGE_ASSERT(m_native_path.empty());

    if (!std::filesystem::is_directory(native_path))
        return false;

    if (!is_writable_path(native_path))
        return false;

    m_native_path = std::filesystem::canonical(native_path);

    return true;
}

fs::type_t fs::type(const std::string &path)
{
    if (!check_path(path))
        return TYPE_INVALID;

    if (std::filesystem::is_directory(m_native_path/path))
        return TYPE_DIR;

    if (std::filesystem::is_regular_file(m_native_path/path))
        return TYPE_FILE;

    return TYPE_INVALID;
}

bool fs::enum_dir(const std::string &path, string_list_t &result)
{
    if (!check_path(path))
        return false;

    if (!exists(path))
        return false;

    result.clear();

    for (auto &dent: std::filesystem::directory_iterator(m_native_path/path)) {
        result.push_back(dent.path().stem());
    }

    return true;
}

bool fs::make_dir(const std::string &path)
{
    if (!check_path(path))
        return false;

    return std::filesystem::create_directories(m_native_path/path);
}

io *fs::open_file(const std::string &path, int io_flags)
{
    if (!check_path(path))
        return nullptr;

    file *fp = new file((m_native_path/path).string());
    if (fp == nullptr)
        return nullptr;

    if (!fp->open(io_flags)) {
        delete fp;
        return nullptr;
    }

    return fp;
}

bool fs::remove(const std::string &path)
{
    if (!check_path(path))
        return false;

    return std::filesystem::remove(m_native_path/path);
}

bool fs::check_name(const std::string &name)
{
    for (size_t i = 0; i < name.size(); ++i) {
        if (!is_name_char(name[i]))
            return false;
    }

    return true;
}

bool fs::check_path(const std::string &path)
{
    if (path.empty())
        return false;

    if (path[0] != '/')
        return false;

    bool slashed = true;

    for (size_t i = 1; i < path.size(); ++i) {
        char c = path[i];
        if (c == '/') {
            if (slashed)
                return false;
            slashed = true;
        } else if (!is_name_char(c))
            return false;
        else
            slashed = false;
    }

    return true;
}

void fs::set_native_path(const std::string &r)
{
    m_native_path = std::filesystem::canonical(r);
}

bool fs::is_name_char(char c)
{
    if (std::isalnum(c))
        return true;

    if (c == '_' || c == '.')
        return true;

    return false;
}

SGE_VM_END
