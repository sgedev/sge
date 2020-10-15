//
//
#ifndef SGE_IO_HPP
#define SGE_IO_HPP

#include <sge/common.hpp>

SGE_BEGIN

class io {
public:
	enum {
		FLAG_READ = 0x1,
		FLAG_WRITE = 0x2,
		FLAG_CREATE = 0x4,
		FLAG_EXISTED = 0x8,
		FLAG_TRUNCATE = 0x10
	};

	enum seek_from {
		SEEK_FROM_START = 0,
		SEEK_FROM_CURRENT,
		SEEK_FROM_END
	};

public:
	io(void);
	virtual ~io(void);

public:
	virtual bool is_seekable(void) = 0;
	virtual bool open(int flags);
	virtual void close(void);
	virtual int64_t size(void) = 0;
	virtual int64_t seek(int64_t offset, seek_from from = SEEK_FROM_START) = 0;
    virtual int64_t read(void *p, int64_t size) = 0;
    virtual int64_t write(const void *p, int64_t size) = 0;
	int flags(void) const;
	bool is_open(void) const;
    bool is_writable(void) const;

private:
	int m_flags;
};

SGE_INLINE int io::flags(void) const
{
	return m_flags;
}

SGE_INLINE bool io::is_open(void) const
{
	return (m_flags != 0);
}

SGE_INLINE bool io::is_writable(void) const
{
    return (m_flags & (FLAG_WRITE | FLAG_CREATE | FLAG_TRUNCATE));
}

SGE_END

#endif // SGE_IO_HPP
