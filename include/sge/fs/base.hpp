//
//
#ifndef SGE_FS_BASE_HPP
#define SGE_FS_BASE_HPP

#include <memory>
#include <string>

#include <sge/noncopyable.hpp>
#include <sge/fs/common.hpp>
#include <sge/fs/file.hpp>

SGE_FS_BEGIN

typedef std::shared_ptr<file> file_ptr;

class base: public noncopyable {
public:
	base(void);
	virtual ~base(void);

public:
	virtual bool open(const char *native_path) = 0;
	virtual void close(void) = 0;
	virtual file_ptr get_file(const char *path) = 0;
	virtual file_ptr new_file(const char *path, int64_t size) = 0;
	const char *native_path(void) const;
	bool check_path(const char *path);

protected:
	std::string m_native_path;
};

inline const char *base::native_path(void) const
{
	return m_native_path.c_str();
}

SGE_FS_END

#endif // SGE_FS_BASE_HPP
