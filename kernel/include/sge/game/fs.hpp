//
//
#ifndef SGE_VM_FS_HPP
#define SGE_VM_FS_HPP

#include <string>
#include <filesystem>

#include <sge/io.hpp>
#include <sge/vm/common.hpp>

SGE_VM_BEGIN

class fs {
public:
    typedef enum {
        TYPE_INVALID = 0,
        TYPE_DIR,
        TYPE_FILE
    } type_t;

public:
    fs(void);
    virtual ~fs(void);

public:
    virtual bool is_writable(void);
    virtual bool init(const std::string &native_path);
    const std::filesystem::path &native_path(void) const;
    virtual type_t type(const std::string &path);
    bool exists(const std::string &path);
    virtual bool enum_dir(const std::string &path, string_list_t &result);
    virtual bool make_dir(const std::string &path);
    virtual io *open_file(const std::string &path, int io_flags);
    virtual bool remove(const std::string &path);
    static bool check_name(const std::string &name);
    static bool check_path(const std::string &path);

protected:
    void set_native_path(const std::string &r);
    static bool is_name_char(char c);

private:
    std::filesystem::path m_native_path;
};

SGE_INLINE const std::filesystem::path &fs::native_path(void) const
{
    return m_native_path;
}

SGE_INLINE bool fs::exists(const std::string &path)
{
    return (type(path) != TYPE_INVALID);
}

SGE_VM_END

#endif // SGE_VM_FS_HPP
