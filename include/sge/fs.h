//
//
#ifndef SGE_FS_H
#define SGE_FS_H

#include <filesystem>

#include <sge/fs/common.h>
#include <sge/fs/file.h>
#include <sge/fs/path.h>

SGE_FS_BEGIN

bool init(const char *root);
void shutdown(void);

SGE_FS_END

#endif // SGE_FS_H
