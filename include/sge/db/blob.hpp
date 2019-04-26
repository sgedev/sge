//
//
#ifndef SGE_DB_BLOB_HPP
#define SGE_DB_BLOB_HPP

#include <string>

#include <sge/db/common.hpp>

SGE_DB_BEGIN

class blob {
public:
	blob(const char *filename);
	virtual ~blob(void);

public:
	bool open(void);
	void close(void);
	size_t get_size(void) const;
	const void *get_data(size_t offset) const;
	size_t read(size_t offset, void *buf, size_t len);

private:
	std::string m_filename;
	void *m_data;
	size_t m_size;
};

SGE_DB_END

#endif // SGE_DB_BLOB_HPP
