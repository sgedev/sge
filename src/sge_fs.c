/*
 *
 */
#include "sxmlc/sxmlc.h"
#include "miniz/miniz_zip.h"

#include "sge_fs.h"

struct sge_fs_struct {
	mz_zip_archive reader;
	XMLDoc manifest;
};

SGE_API sge_fs_t *sge_fs_new(const char *zip_filename)
{
	int ret;
	sge_fs_t *fs;
	mz_bool mz_ret;
	void *manifest_buf;
	size_t manifest_size;

	if (zip_filename == NULL)
		goto bad0;

	fs = malloc(sizeof(sge_fs_t));
	if (fs == NULL)
		goto bad0;

	mz_zip_zero_struct(&fs->reader);

	mz_ret = mz_zip_reader_init_file(&fs->reader, zip_filename, 0);
	if (!mz_ret)
		goto bad1;

	manifest_buf = mz_zip_reader_extract_file_to_heap(
		&fs->reader, "manifest.xml", &manifest_size, 0);
	if (manifest_buf == NULL)
		goto bad2;

	XMLDoc_init(&fs->manifest);
	
	ret = XMLDoc_parse_buffer_DOM(manifest_buf, NULL, &fs->manifest);
	free(manifest_buf);
	if (!ret)
		goto bad3;

	return fs;

bad3:
	XMLDoc_free(&fs->manifest);

bad2:
	mz_zip_end(&fs->reader);

bad1:
	free(fs);

bad0:
	return NULL;
}

SGE_API void sge_fs_destroy(sge_fs_t *fs)
{
	if (fs == NULL)
		return;

	XMLDoc_free(&fs->manifest);
	mz_zip_end(&fs->reader);
	free(fs);
}

SGE_API sge_dir_t *sge_dir_open(sge_fs_t *fs, const char *dirname)
{
	return NULL;
}

SGE_API void sge_dir_close(sge_dir_t *dir)
{
}

SGE_API sge_bool_t sge_dir_read(sge_dir_t *dir, sge_dir_entry_t *dentry)
{
	return SGE_FALSE;
}

SGE_API sge_file_t *sge_file_open(sge_fs_t *fs, const char *filename)
{
	return NULL;
}

SGE_API void sge_file_close(sge_file_t *file)
{
}

SGE_API sge_int64_t sge_file_get_size(sge_file_t *file)
{
	return -1;
}

SGE_API sge_int64_t sge_file_read(sge_file_t *file, sge_int64_t offset, void *buf, sge_int64_t size)
{
	return -1;
}

