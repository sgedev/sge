//
//
#ifndef SGE_VM_BUFFER_HPP
#define SGE_VM_BUFFER_HPP

#include <vector>

#include <sge/common.hpp>
#include <sge/io.hpp>

SGE_BEGIN

class buffer: public io {
public:
    buffer(void);
    buffer(const buffer &that);
	~buffer(void) override;

public:
    bool open(int flags) override;
    int64_t size(void) override;
    int64_t read(void *p, int64_t len) override;
    int64_t write(const void *p, int64_t len) override;

public:
    bool operator==(const buffer &that);
    bool operator!=(const buffer &that);
    buffer &operator=(const buffer &that);

public:
	byte_array_t data;

protected:
    int64_t pos(void) override;
    int64_t set_pos(int64_t pos) override;

private:
	int64_t m_pos;
};

SGE_INLINE bool buffer::operator==(const buffer &that)
{
	return (data == that.data);
}

SGE_INLINE bool buffer::operator!=(const buffer &that)
{
	return (data != that.data);
}

SGE_INLINE buffer &buffer::operator=(const buffer &that)
{
	if ((*this) != that)
		data = that.data;
	return (*this);
}

SGE_END

#endif // SGE_VM_BUFFER_HPP
