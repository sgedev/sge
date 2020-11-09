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
        FLAG_TRUNCATE = 0x10,

        // internal
        FLAG_SEEKABLE = 0x10000
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
    virtual bool open(int flags);
    virtual void close(void);
    int flags(void) const;
    bool is_open(void) const;
    bool is_readable(void) const;
    bool is_writable(void) const;
    bool is_seekable(void) const;
    virtual int64_t size(void) = 0;
    int64_t tell(void);
    int64_t seek(int64_t offset, seek_from from = SEEK_FROM_START);
    virtual int64_t read(void *p, int64_t size) = 0;
    virtual int64_t write(const void *p, int64_t size) = 0;

protected:
    virtual int64_t pos(void) = 0;
    virtual int64_t set_pos(int64_t v) = 0;
    void set_seekable(bool v);

private:
    int m_flags;
};

SGE_INLINE int io::flags(void) const
{
    SGE_ASSERT(is_open());

    return m_flags;
}

SGE_INLINE bool io::is_open(void) const
{
    return (m_flags != 0);
}

SGE_INLINE bool io::is_readable() const
{
    SGE_ASSERT(is_open());

    return (m_flags & FLAG_READ);
}

SGE_INLINE bool io::is_writable(void) const
{
    SGE_ASSERT(is_open());

    return (m_flags & (FLAG_WRITE | FLAG_CREATE | FLAG_TRUNCATE));
}

SGE_INLINE bool io::is_seekable(void) const
{
    SGE_ASSERT(is_open());

    return (m_flags & FLAG_SEEKABLE);
}

SGE_INLINE int64_t io::tell(void)
{
    SGE_ASSERT(is_open());
    SGE_ASSERT(is_seekable());

    return pos();
}

SGE_END

#endif // SGE_IO_HPP
