/*
 *
 */
#ifndef SGE_H
#define SGE_H

#include <sge_config.h>

#ifdef __cplusplus
#	define SGE_EXTEN_C extern "C"
#	define SGE_BEGIN_DECLS extern "C" {
#	define SGE_END_DECLS }
#else
#	define SGE_EXTEN_C
#	define SGE_BEGIN_DECLS
#	define SGE_END_DECLS
#endif

#if defined(_MSC_VER)
#	define SGE_EXPORT __declspec(dllexport)
#	define SGE_IMPORT __declspec(dllimport)
#	define SGE_INT64 __int64
#	define SGE_UINT64 unsigned __int64
#elif defined(__GNUC__)
#	define SGE_EXPORT
#	define SGE_IMPORT
#	ifdef __LP64__
#		define SGE_INT64 long
#		define SGE_UINT64 unsigned long
#	else
#		define SGE_INT64 long long
#		define SGE_UINT64 unsigned long long
#	endif
#else
#	error unsupported compiler.
#endif

#ifdef SGE_SHARED
#	ifdef SGE_INTERNAL
#		define SGE_API SGE_EXPORT
#	else
#		define SGE_API SGE_IMPORT
#	endif
#else
#	define SGE_API
#endif

#define SGE_PATH_MAX 512
#define SGE_NAME_MAX 64

SGE_BEGIN_DECLS

/* common */

typedef signed char sge_int8_t;
typedef unsigned char sge_uint8_t;
typedef signed short sge_int16_t;
typedef unsigned char sge_uint16_t;
typedef signed int sge_int32_t;
typedef unsigned int sge_uint32_t;
typedef SGE_INT64 sge_int64_t;
typedef SGE_UINT64 sge_uint64_t;

typedef enum {
	SGE_FALSE = 0,
	SGE_TRUE = 1
} sge_bool_t;

/* fs */

typedef enum {
	SGE_DIR_ENTRY_TYPE_INVALID = 0,
	SGE_DIR_ENTRY_TYPE_DIR,
	SGE_DIR_ENTRY_TYPE_FILE,
	SGE_DIR_ENTRY_TYPE_MAX
} sge_dir_entry_type_t;

typedef struct {
	sge_dir_entry_type_t type;
	char name[SGE_NAME_MAX];
} sge_dir_entry_t;

typedef struct sge_fs_struct sge_fs_t;
typedef struct sge_dir_struct sge_dir_t;
typedef struct sge_file_struct sge_file_t;

SGE_API sge_fs_t *sge_fs_new(const char *zip_filename);
SGE_API void sge_fs_destroy(sge_fs_t *fs);
SGE_API sge_dir_t *sge_dir_open(sge_fs_t *fs, const char *dirname);
SGE_API void sge_dir_close(sge_dir_t *dir);
SGE_API sge_bool_t sge_dir_read(sge_dir_t *dir, sge_dir_entry_t *dentry);
SGE_API sge_file_t *sge_file_open(sge_fs_t *fs, const char *filename);
SGE_API void sge_file_close(sge_file_t *file);
SGE_API sge_int64_t sge_file_get_size(sge_file_t *file);
SGE_API sge_int64_t sge_file_read(sge_file_t *file, sge_int64_t offset, void *buf, sge_int64_t size);

/* camera */

typedef struct sge_camera_struct sge_camera_t;

typedef struct {
	int priv_size;
	sge_bool_t (*init)(sge_camera_t *camera);
	void (*shutdown)(sge_camera_t *camera);
	void (*draw)(sge_camera_t *camera);
} sge_camera_type_t;

extern SGE_API const sge_camera_type_t * const SGE_CAMERA_TYPE_OPENGL;
extern SGE_API const sge_camera_type_t * const SGE_CAMERA_TYPE_SOFTWARE;

SGE_API sge_camera_t *sge_camera_new(const sge_camera_type_t *type);
SGE_API void sge_camera_destroy(sge_camera_t *camera);
SGE_API void sge_camera_draw(sge_camera_t *camera);

/* scene */

typedef struct sge_scene_struct sge_scene_t;

SGE_API sge_scene_t *sge_scene_new(sge_fs_t *fs, const char *dirname);
SGE_API void sge_scene_destroy(sge_scene_t *scene);
SGE_API void sge_scene_update(sge_scene_t *scene, float elapsed);
SGE_API void sge_scene_draw(sge_scene_t *scene, sge_camera_t *camera);

SGE_END_DECLS

#endif /* SGE_H */

