//
//
#ifndef SGE_FS_COMMON_H
#define SGE_FS_COMMON_H

#include <sge/common.h>

#define SGE_FS_BEGIN SGE_BEGIN namespace fs {
#define SGE_FS_END } SGE_END

SGE_FS_BEGIN

SDL_RWops *get_root_file(void);

SGE_FS_END

#endif // SGE_FS_COMMON_H

