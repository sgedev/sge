//
//
#include <sge/fs.hpp>

SGE_BEGIN

fs::fs(void)
{
}

fs::~fs(void)
{
	shutdown();
}

bool fs::init(const char *root_image)
{
	SGE_ASSERT(root_image != NULL);

	mz_zip_zero_struct(&m_reader);

	mz_bool ret;
	void *manifest_buf;
	size_t manifest_size;
	pugi::xml_parse_result res;

	ret = mz_zip_reader_init_file(&m_reader, root_image, 0);
	if (!ret)
		return false;

	manifest_buf = mz_zip_reader_extract_file_to_heap(
		&m_reader, "manifest.xml", &manifest_size, 0);

	if (manifest_buf == NULL || manifest_size <= 0) {
		mz_zip_reader_end(&m_reader);
		return false;
	}

	res = m_manifest.load_buffer(manifest_buf, manifest_size);
	free(manifest_buf);

	if (ret != pugi::status_ok) {
		mz_zip_reader_end(&m_reader);
		return false;
	}

	return true;
}

void fs::shutdown(void)
{
	mz_zip_reader_end(&m_reader);
}

SGE_END

