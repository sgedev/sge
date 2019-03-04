//
//
#ifndef SGE_FILE_H
#define SGE_FILE_H

#include <sge/fs/common.h>
#include <sge/fs/dentry.h>

SGE_FS_BEGIN

class file: public dentry {
public:
	file(void);
	virtual ~file(void);
};

SGE_FS_END

#endif // SGE_FILE_H

