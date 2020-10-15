//
//
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

	if (flags & (FLAG_CREATE | FLAG_EXISTED))
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

SGE_END
