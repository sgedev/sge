//
//
#include <sge/db/blob.hpp>

SGE_DB_BEGIN

blob::blob(void)
	: m_data(NULL)
	, m_size(0)
{
}

blob::~blob(void)
{
	if (m_data != NULL) {
		SGE_ASSERT(m_size > 0);
		free(m_data);
	}
}

bool blob::init(mz_zip_archive *zip, const char *filename)
{
	SGE_ASSERT(zip != NULL);
	SGE_ASSERT(filename != NULL);

	int index = mz_zip_reader_locate_file(zip, filename, NULL, 0);
	if (index < 0)
		return false;

	m_data = mz_zip_reader_extract_to_heap(zip, index, &m_size, 0);
	if (m_data == NULL)
		return false;

	m_path = filename;

	return true;
}

SGE_DB_END

