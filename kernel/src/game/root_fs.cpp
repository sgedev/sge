//
//
#include <filesystem>

#include <sge/vm/zip_fs.hpp>
#include <sge/vm/root_fs.hpp>

SGE_VM_BEGIN

root_fs::root_fs(void)
{
}

root_fs::~root_fs(void)
{
}

root_fs::type_t root_fs::type(const std::string &path)
{
    type_t t = fs::type(path);
    if (t != TYPE_INVALID)
        return t;

    for (auto it(m_mount_list.begin()); it != m_mount_list.end(); ++it) {
        if (it->path.size() < path.size())
            continue;
        if (strncmp(it->path.data(), path.data(), it->path.size()))
            continue;
        t = it->fs->type(path.data() + it->path.size());
        if (t != TYPE_INVALID)
            return t;
    }

    return TYPE_INVALID;
}

bool root_fs::enum_dir(const std::string &path, string_list_t &result)
{
    result.clear();
    fs::enum_dir(path, result);
    result.sort();

    for (auto it(m_mount_list.begin()); it != m_mount_list.end(); ++it) {
        if (it->path.size() < path.size())
            continue;
        if (strncmp(it->path.data(), path.data(), it->path.size()))
            continue;
        string_list_t tmp;
        if (it->fs->enum_dir(path.data() + it->path.size(), tmp))
            result.merge(tmp);
    }

    return true;
}

io *root_fs::open_file(const std::string &path, int io_flags)
{
    io *p = fs::open_file(path, io_flags);
    if (p != nullptr)
        return p;

    if (io_flags & (io::FLAG_WRITE | io::FLAG_CREATE | io::FLAG_TRUNCATE))
        return nullptr;

    for (auto it(m_mount_list.begin()); it != m_mount_list.end(); ++it) {
        if (it->path.size() < path.size())
            continue;
        if (strncmp(it->path.data(), path.data(), it->path.size()))
            continue;
        io *p = it->fs->open_file(path.data() + it->path.size(), io_flags);
        if (p != nullptr)
            return p;
    }

    return nullptr;
}

bool root_fs::mount(const std::string &native_path, const std::string &path)
{
    if (!check_path(path))
        return false;

    if (find_mount(native_path, path) != m_mount_list.end())
        return false;

    fs_ptr p;

    if (std::filesystem::is_directory(native_path))
        p.reset(new fs());
    else if (std::filesystem::is_regular_file(native_path))
        p.reset(new zip_fs());

    if (!p || !p->init(native_path))
        return false;

    std::string tmp(path);
    if (tmp.back() != '/')
        tmp += '/';

    m_mount_list.emplace_front(p, tmp);

    return true;
}

bool root_fs::unmount(const std::string &native_path, const std::string &path)
{
    auto it(find_mount(native_path, path));
    if (it == m_mount_list.end())
        return false;

    m_mount_list.erase(it);

    return true;
}

const mount_info_list_t &root_fs::mount_list(void) const
{
    return m_mount_list;
}

bool root_fs::is_writable(const std::string &path)
{
    return (fs::type(path) != TYPE_INVALID);
}

mount_info_list_t::iterator root_fs::find_mount(const std::string &native_path, const std::string &path)
{
    std::string tmp(path);
    if (tmp.back() != '/')
        tmp += '/';

    for (auto it(m_mount_list.begin()); it != m_mount_list.end(); ++it) {
        if (it->fs->native_path() == native_path && it->path == tmp)
            return it;
    }

    return m_mount_list.end();
}

SGE_VM_END
