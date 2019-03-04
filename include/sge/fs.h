//
//
#ifndef SGE_FS_H
#define SGE_FS_H

#include <sge/fs/common.h>
#include <sge/fs/dentry.h>
#include <sge/fs/file.h>

SGE_FS_BEGIN

bool init(const char *root);
void shutdown(void);
void update(void);

SGE_FS_END

#endif // SGE_FS_H

