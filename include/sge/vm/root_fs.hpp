//
//
#ifndef SGE_VM_ROOT_FS_HPP
#define SGE_VM_ROOT_FS_HPP

#include <list>
#include <memory>

#include <sge/vm/common.hpp>
#include <sge/vm/fs.hpp>

SGE_VM_BEGIN

typedef std::shared_ptr<fs> fs_ptr;

struct mount_info {
    mount_info(fs_ptr f, const std::string &p): fs(f), path(p) { }

    fs_ptr fs;
    std::string path;
};

typedef std::list<mount_info> mount_info_list_t;

class root_fs: public fs {
public:
    root_fs(void);
    ~root_fs(void) override;

public:
    type_t type(const std::string &path) override;
    bool enum_dir(const std::string &path, string_list_t &result) override;
    io *open_file(const std::string &path, int io_flags) override;
    bool mount(const std::string &native_path, const std::string &path);
    bool unmount(const std::string &native_path, const std::string &path);
    const mount_info_list_t &mount_list(void) const;
    bool is_writable(const std::string &path);

protected:
    mount_info_list_t::iterator find_mount(const std::string &native_path, const std::string &path);

private:
    mount_info_list_t m_mount_list;
};

SGE_VM_END

#endif // SGE_VM_ROOT_FS_HPP
