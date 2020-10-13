//
//
#ifndef SGE_ROOT_FS_HPP
#define SGE_ROOT_FS_HPP

#include <list>
#include <memory>

#include <sge/common.hpp>
#include <sge/fs.hpp>

SGE_BEGIN

typedef std::shared_ptr<fs> fs_ptr;
typedef std::list<fs_ptr> fs_list_t;

class root_fs : public fs {
public:
	root_fs(void);
	~root_fs(void) override;

public:
	bool init(const std::string &native_path) override;
	type_t type(const std::string &path) override;
	bool enum_dir(const std::string &path, string_list_t &result) override;
	bool make_dir(const std::string &path) override;
	io *open_file(const std::string &path, int io_flags) override;
	bool remove(const std::string &path) override;
	bool mount(const std::string &native_path, const std::string &path);
	bool unmount(const std::string &native_path, const std::string &path);
	const fs_list_t &mount_list(void) const;

private:
	fs_list_t m_fs_list;
};

SGE_END

#endif // SGE_ROOT_FS_HPP
