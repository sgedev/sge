//
//
#include <algorithm>

#include <sge/io.hpp>

SGE_BEGIN

io::io(void) :
	m_flags(0)
{
}

io::~io(void)
{
}

bool io::open(int flags)
{
	if (flags == 0)
		return false;

    if (flags & (FLAG_CREATE | FLAG_EXISTED | FLAG_SEEKABLE))
		return false;

	if ((flags & (FLAG_CREATE | FLAG_TRUNCATE)) && !(flags & FLAG_WRITE))
		return false;

    m_flags = flags;

	return true;
}

void io::close(void)
{
	m_flags = 0;
}

void io::set_seekable(bool v)
{
    if (v)
        m_flags |= FLAG_SEEKABLE;
    else
        m_flags &= ~FLAG_SEEKABLE;
}

int64_t io::seek(int64_t offset, seek_from from)
{
    SGE_ASSERT(is_open());
    SGE_ASSERT(is_seekable());

    int64_t new_pos;
    int64_t sz = size();

    switch (from) {
    case SEEK_FROM_START:
        new_pos = 0;
        break;
    case SEEK_FROM_CURRENT:
        new_pos = pos() + offset;
        break;
    case SEEK_FROM_END:
        new_pos = sz + offset;
        break;
    default:
        return -1;
    }

    return set_pos(std::clamp(new_pos, int64_t(0), sz));
}

SGE_END
