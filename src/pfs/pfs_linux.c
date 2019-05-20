/*
 *
 */
#include <limits.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>

#include <pfs.h>

struct pfs_dir {
	DIR *dir;
	char dirname[PFS_PATH_MAX];
};

PFS_API void pfs_update_roots(void)
{
}

PFS_API int pfs_get_root_count(void)
{
	return 1;
}

PFS_API const char *pfs_get_root_name(int index)
{
	if (index != 0)
		return NULL;

	return "";
}

PFS_API int pfs_get_path_root(const char *path)
{
	return 0;
}

PFS_API int pfs_get_real_path(const char *path, char *buf, int size)
{
	if (path == NULL || buf == NULL || size < PFS_PATH_MAX)
		return -1;

	char *p = realpath(path, buf);
	if (p == NULL)
		return -1;

	return 0;
}

PFS_API int pfs_get_curr_dir(char *buf, int size)
{
	const char *p;

	if (buf == NULL || size < 1)
		return -1;

	p = getcwd(buf, size);
	if (p == NULL)
		return -1;

	return 0;
}

PFS_API int pfs_get_home_dir(char *buf, int size)
{
	const char *home_dir;

	if (buf == NULL || size < 1)
		return -1;

	home_dir = getenv("HOME");
	if (home_dir != NULL)
		strncpy(buf, home_dir, size);
	else
		strncpy(buf, "/", size);

	return 0;
}

PFS_API int pfs_get_temp_dir(char *buf, int size)
{
	const char *home_dir;

	if (buf == NULL || size < 1)
		return -1;

	home_dir = getenv("TEMP");
	if (home_dir == NULL)
		home_dir = getenv("TMP");

	if (home_dir != NULL)
		strncpy(buf, home_dir, size);
	else
		strncpy(buf, "/", size);

	return 0;
}

PFS_API pfs_dir_t *pfs_dir_open(const char *dirname)
{
	pfs_dir_t *dir;

	if (dirname == NULL)
		return NULL;

	dir = malloc(sizeof(pfs_dir_t));
	if (dir == NULL)
		return NULL;

	dir->dir = opendir(dirname);
	if (dir->dir == NULL) {
		free(dir);
		return NULL;
	}

	strncpy(dir->dirname, dirname, PFS_PATH_MAX);

	return dir;
}

PFS_API void pfs_dir_close(pfs_dir_t *dir)
{
	if (dir == NULL)
		return;

	if (dir->dir != NULL)
		closedir(dir->dir);

	free(dir);
}

PFS_API int pfs_dir_read(pfs_dir_t *dir, pfs_dirent_t *ent)
{
	int ret;
	struct dirent *d;

	if (dir == NULL || dir->dir == NULL || ent == NULL)
		return -1;

	d = readdir(dir->dir);
	if (d == NULL)
		return 0;

	strncpy(ent->name, d->d_name, PFS_PATH_MAX);
	if (d->d_type != DT_DIR) {
		struct stat st;
		char buf[PATH_MAX];
		ent->type = PFS_DIRENT_TYPE_FILE;
		snprintf(buf, sizeof(buf), "%s/%s", dir->dirname, d->d_name);
		ret = lstat(buf, &st);
		ent->size = !ret ? st.st_size : 0;
	} else {
		ent->type = PFS_DIRENT_TYPE_DIR;
		ent->size = 0;
	}

	return 1;
}

