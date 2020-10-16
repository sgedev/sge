//
//
#include <algorithm>

#include <sge/buffer.hpp>

SGE_BEGIN

buffer::buffer(void):
	m_pos(0)
{
}

buffer::buffer(const buffer &that):
	data(that.data),
	m_pos(0)
{
}

buffer::~buffer(void)
{
}

bool buffer::open(int flags)
{
    if (!io::open(flags))
        return false;

    set_seekable(true);

    return true;
}

int64_t buffer::size(void)
{
	return data.size();
}

int64_t buffer::read(void *p, int64_t size)
{
    int64_t len = std::min(int64_t(data.size() - m_pos), size);
    if (len > 0) {
        memcpy(p, data.data() + m_pos, len);
        m_pos += len;
    }

    return len;
}

int64_t buffer::write(const void *p, int64_t size)
{
    int64_t len = std::min(int64_t(data.size() - m_pos), size);
    if (len > 0) {
        memcpy(data.data() + m_pos, p, len);
        m_pos += len;
    }

    return len;
}

int64_t buffer::pos(void)
{
    return m_pos;
}

int64_t buffer::set_pos(int64_t pos)
{
    m_pos = std::clamp(pos, int64_t(0), int64_t(data.size()));
    return m_pos;
}

SGE_END
