//
//
#ifndef SGE_FS_NATIVE_HPP
#define SGE_FS_NATIVE_HPP

#include <sge/fs/common.hpp>
#include <sge/fs/base.hpp>

SGE_FS_BEGIN

class native: public base {
public:
	native(void);
	virtual ~native(void);

public:
	virtual bool open(const char *native_path);
	virtual void close(void);
	virtual file_ptr get_file(const char *path);
	virtual file_ptr new_file(const char *path, int64_t size);
};

SGE_FS_END

#endif // SGE_FS_NATIVE_HPP
