//
//
#ifndef SGE_DB_BLOB_HPP
#define SGE_DB_BLOB_HPP

#include <algorithm>
#include <string>

#include <miniz_zip.h>

#include <sge/db/common.hpp>

SGE_DB_BEGIN

class blob {
public:
	blob(void);
	virtual ~blob(void);

public:
	bool init(mz_zip_archive *zip, const char *filename);
	size_t get_size(void) const;
	const void *get_data(size_t offset = 0) const;
	size_t read(size_t offset, void *buf, size_t buf_size) const;

private:
	std::string m_path;
	void *m_data;
	size_t m_size;
};

inline size_t blob::get_size(void) const
{
	SGE_ASSERT(m_data != NULL);

	return m_size;
}

inline const void *blob::get_data(size_t offset) const
{
	SGE_ASSERT(m_data != NULL);
	SGE_ASSERT(offset < m_size);

	return (((unsigned char *)m_data) + offset);
}

inline size_t blob::read(size_t offset, void *buf, size_t buf_size) const
{
	SGE_ASSERT(m_data != NULL);
	SGE_ASSERT(offset < m_size);

	size_t len = std::min(m_size - offset, buf_size);
	if (len > 0)
		memcpy(buf, ((unsigned char *)m_data) + offset, len);

	return len;
}

SGE_DB_END

#endif // SGE_DB_BLOB_HPP

