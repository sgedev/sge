//
//
#ifndef SGE_ZIP_READER_HPP
#define SGE_ZIP_READER_HPP

#include <string>

#include <miniz_zip.h>

#include <sge/common.hpp>
#include <sge/buffer.hpp>

SGE_BEGIN

class zip_reader {
public:
	typedef mz_zip_archive_file_stat stat_t;

public:
	zip_reader(void);
    zip_reader(const char *filename);
	virtual ~zip_reader(void);

public:
	const std::string &filename(void) const;
    void set_filename(const std::string &r);
	bool open(void);
	void close(void);
	bool is_open(void) const;
	int count(void);
    int index(const std::string &path);
	bool stat(int i, stat_t &r);
    bool stat(const std::string &path, stat_t &r);
	bool extract(int i, byte_array_t &ba);
    bool extract(const std::string &path, byte_array_t &ba);
	bool extract(int i, buffer &buf);
    bool extract(const std::string &path, buffer &buf);

private:
	std::string m_filename;
	mz_zip_archive m_archive;
};

SGE_END

#endif // SGE_ZIP_ARCHIVE_HPP
