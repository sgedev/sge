//
//
#ifndef SGE_FS_HPP
#define SGE_FS_HPP

#include <string>
#include <filesystem>

#include <sge/common.hpp>
#include <sge/io.hpp>

SGE_BEGIN

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
	const std::string &native_path(void) const;
	virtual type_t type(const std::string &path);
	virtual bool enum_dir(const std::string &path, strings_t &result);
	virtual bool make_dir(const std::string &path);
	virtual io *open_file(const std::string &path, int io_flags);
	virtual bool remove(const std::string &path);
	static bool check_name(const std::string &name);
	static bool check_path(const std::string &path);

protected:
	static bool is_name_char(char c);

private:
	std::string m_native_path;
};

SGE_END

#endif // SGE_FS_HPP
