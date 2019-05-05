//
//
#ifndef SGE_FS_ZIP_HPP
#define SGE_FS_ZIP_HPP

#include <miniz_zip.h>

#include <sge/fs/common.hpp>
#include <sge/fs/base.hpp>

SGE_FS_BEGIN

class zip: public base {
public:
	zip(void);
	virtual ~zip(void);

public:
	virtual bool open(const char *native_path);
	virtual void close(void);
	virtual file_ptr get_file(const char *path);
	virtual file_ptr new_file(const char *path, int64_t size);

private:
	mz_zip_archive m_zip;
};

SGE_FS_END

#endif // SGE_FS_ZIP_HPP
