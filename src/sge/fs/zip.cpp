//
//
#include <sge/fs/zip.hpp>

SGE_FS_BEGIN

class zip_file: public file {
public:
	zip_file(void *p, int size);
	virtual ~zip_file(void);

private:
	void *m_mem;
};

zip_file::zip_file(void *p, int size)
	: file(SDL_RWFromConstMem(p, size))
	, m_mem(p)
{
}

zip_file::~zip_file(void)
{
	if (m_mem != NULL)
		free(m_mem);
}

zip::zip(void)
{
	mz_zip_zero_struct(&m_zip);
}

zip::~zip(void)
{
}

bool zip::open(const char *native_path)
{
	SGE_ASSERT(native_path != NULL);

	mz_bool zret;

	zret = mz_zip_reader_init_file(&m_zip, native_path, 0);
	if (!zret)
		return false;

	m_native_path = native_path;

	return true;
}

void zip::close(void)
{
	mz_zip_reader_end(&m_zip);
}

file_ptr zip::get_file(const char *path)
{
	SGE_ASSERT(check_path(path));

	file_ptr fp;
	size_t size;
	void *p = mz_zip_reader_extract_file_to_heap(&m_zip, path + 1, &size, 0);
	if (p == NULL || size < 1)
		return fp;

	fp.reset(new zip_file(p, size));

	return fp;
}

file_ptr zip::new_file(const char *path, int64_t size)
{
	return NULL;
}

SGE_FS_END
