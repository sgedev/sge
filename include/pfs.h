/*
 *
 */
#ifndef PFS_H
#define PFS_H

#if defined(_WIN32)
#	include <Windows.h>
#	define PFS_PATH_MAX MAX_PATH
#	define PFS_PATH_SP "\\"
#elif defined(__linux__)
#	include <limits.h>
#	define PFS_PATH_MAX PATH_MAX
#	define PFS_PATH_SP "/"
#else
#	error platform is not supported.
#endif

/* TODO */
#define PFS_API

#define PFS_DIRENT_TYPE_INVALID 0
#define PFS_DIRENT_TYPE_DIR 1
#define PFS_DIRENT_TYPE_FILE 2

#ifdef __cplusplus
extern "C" {
#endif

typedef struct pfs_dir pfs_dir_t;

typedef struct {
	char name[PFS_PATH_MAX];
	int type;
	long size;
} pfs_dirent_t;

PFS_API void pfs_update_roots(void);
PFS_API int pfs_get_root_count(void);
PFS_API const char *pfs_get_root_name(int index);
PFS_API int pfs_get_path_root(const char *path);
PFS_API int pfs_get_real_path(const char *path, char *buf, int size);
PFS_API int pfs_get_curr_dir(char *buf, int size);
PFS_API int pfs_get_home_dir(char *buf, int size);
PFS_API int pfs_get_temp_dir(char *buf, int size);
PFS_API pfs_dir_t *pfs_dir_open(const char *dirname);
PFS_API void pfs_dir_close(pfs_dir_t *dir);
PFS_API int pfs_dir_read(pfs_dir_t *dir, pfs_dirent_t *ent);

#ifdef __cplusplus
}
#endif

#endif /* PFS_H */

