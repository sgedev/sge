//
//
#include <algorithm>

#include <sge/db/blob.hpp>

SGE_DB_BEGIN

blob::blob(mz_zip_archive *archive, const char *filename)
	: m_archive(archive)
	, m_filename(filename)
	, m_data(NULL)
	, m_size(0)
{
	SGE_ASSERT(m_archive != NULL);
	SGE_ASSERT(!m_filename.empty());
}

blob::~blob(void)
{
	if (m_data != NULL)
		close();
}

bool blob::open(void)
{
	SGE_ASSERT(m_archive != NULL);
	SGE_ASSERT(m_data == NULL);

	m_data = mz_zip_reader_extract_file_to_heap(
		m_archive, m_filename.c_str(), &m_size, 0);

	if (m_data == NULL)
		return false;

	return true;
}

void blob::close(void)
{
	SGE_ASSERT(m_data != NULL);

	free(m_data);
}

size_t blob::get_size(void) const
{
	SGE_ASSERT(m_data != NULL);

	return m_size;
}

const void *blob::get_data(size_t offset) const
{
	SGE_ASSERT(m_data != NULL);
	SGE_ASSERT(offset <= m_size);

	return (((uint8_t *)m_data) + offset);
}

size_t blob::read(size_t offset, void *buf, size_t len)
{
	SGE_ASSERT(m_data != NULL);
	SGE_ASSERT(offset <= m_size);
	SGE_ASSERT((offset + len) <= m_size);

	size_t read_len = m_size - offset;
	read_len = std::min(read_len, len);

	if (read_len > 0)
		memcpy(buf, get_data(offset), len);

	return read_len;
}

SGE_DB_END
