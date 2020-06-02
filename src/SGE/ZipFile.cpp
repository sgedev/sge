//
//
#include <SGE/ZipFile.hpp>

SGE_BEGIN

ZipFile::ZipFile(ZipReaderPtr zip, int loc):
	m_zip(zip),
	m_loc(loc),
	m_pos(-1)
{
	Q_ASSERT(m_zip);
	Q_ASSERT(m_loc >= 0);
}

bool ZipFile::atEnd(void) const
{
	return (m_pos == m_data.size());
}

void ZipFile::close(void)
{
	if (m_pos > 0) {
		m_data.clear();
		m_pos = -1;
	}
}

bool ZipFile::isSequential(void) const
{
	return true;
}

bool ZipFile::open(OpenMode mode)
{
	Q_ASSERT(m_pos < 0);

	return m_zip->extract(m_loc, m_data);
}

qint64 ZipFile::pos(void) const
{
	Q_ASSERT(m_pos >= 0);

	return m_pos;
}

bool ZipFile::reset(void)
{
	Q_ASSERT(m_pos >= 0);

	m_pos = 0;

	return true;
}

bool ZipFile::seek(qint64 pos)
{
	Q_ASSERT(m_pos >= 0);

	if (pos < 0)
		m_pos = 0;
	else if (pos > m_data.size())
		m_pos = m_data.size();
	else
		m_pos = pos;

	return true;
}

qint64 ZipFile::size(void) const
{
	Q_ASSERT(m_pos >= 0);

	return m_data.size();
}

qint64 ZipFile::readData(char *data, qint64 maxlen)
{
	Q_ASSERT(m_pos >= 0);

	qint64 len = m_data.size() - m_pos;
	len = qMin(len, maxlen);
	if (len > 0)
		memcpy(data, m_data.data(), len);

	return len;
}

qint64 ZipFile::writeData(const char *data, qint64 len)
{
	Q_ASSERT(m_pos >= 0);

	return -1;
}

SGE_END
