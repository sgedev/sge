//
//
#ifndef SGE_FILE_HPP
#define SGE_FILE_HPP

#include <string>

#include <sge/common.hpp>
#include <sge/io.hpp>

SGE_BEGIN

class file: public io {
public:
	file(void);
	file(const std::string &filename);
	~file(void) override;

public:
	bool is_seekable(void) override;
	bool open(int flags) override;
	void close(void) override;
	int64_t size(void) override;
	int64_t seek(int64_t offset, seek_from from) override;
	int64_t read(void *p, int64_t len) override;
	int64_t write(const void *p, int64_t len) override;
	const std::string &filename(void) const;
	void set_filename(const std::string &r);

private:
	std::string m_filename;
	SDL_RWops *m_fops;
};

SGE_INLINE const std::string &file::filename(void) const
{
	return m_filename;
}

SGE_INLINE void file::set_filename(const std::string &r)
{
	SGE_ASSERT(m_fops == nullptr);
	m_filename = r;
}

SGE_END

#endif // SGE_VM_FS_HPP
