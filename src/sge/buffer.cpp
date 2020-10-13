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

bool buffer::is_seekable(void)
{
	return true;
}

int64_t buffer::size(void)
{
	return data.size();
}

int64_t buffer::seek(int64_t offset, seek_from from)
{
	int64_t pos = m_pos;

	switch (from) {
	case SEEK_FROM_START:
		pos = offset;
		break;
	case SEEK_FROM_CURRENT:
		pos += offset;
		break;
	case SEEK_FROM_END:
		pos = data.size() + offset;
		break;
	default:
		return -1;
	}

	m_pos = std::clamp(pos, int64_t(0), int64_t(data.size()));

	return m_pos;
}

int64_t buffer::read(void *p, int64_t len)
{
	int64_t ret = data.size() - m_pos;
	ret = std::min(ret, len);
	if (p != nullptr && ret > 0)
		memcpy(p, data.data() + m_pos, ret);
	m_pos += ret;
	return ret;
}

int64_t buffer::write(const void *p, int64_t len)
{
	int64_t ret = data.size() - m_pos;
	ret = std::min(ret, len);
	if (p != nullptr && ret > 0)
		memcpy(data.data() + m_pos, p, ret);
	m_pos += ret;
	return ret;
}

SGE_END
