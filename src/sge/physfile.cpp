//
//
#include <sge/physfile.hpp>

SGE_BEGIN

PhysFile::PhysFile(const QString &path, QObject *parent)
	: QIODevice(parent)
	, m_path(path)
	, m_file(Q_NULLPTR)
{
}

PhysFile::~PhysFile(void)
{
	close();
}

bool PhysFile::atEnd(void) const
{
	if (m_file == Q_NULLPTR)
		return true;

	return PHYSFS_eof(m_file);
}

bool PhysFile::open(QIODevice::OpenMode mode)
{
	if (mode & QIODevice::WriteOnly || mode & QIODevice::Append ||
		mode & QIODevice::NewOnly || mode & QIODevice::Truncate)
		return false;

	if (m_file != Q_NULLPTR)
		return false;

	m_file = PHYSFS_openRead(m_path.toStdString().c_str());
	if (m_file == Q_NULLPTR)
		return false;

	if (!QIODevice::open(mode)) {
		PHYSFS_close(m_file);
		m_file = Q_NULLPTR;
		return false;
	}

	return true;
}

void PhysFile::close(void)
{
	if (m_file != Q_NULLPTR) {
		PHYSFS_close(m_file);
		m_file = Q_NULLPTR;
	}
}

bool PhysFile::seek(qint64 pos)
{
	if (m_file == Q_NULLPTR)
		return false;

	return PHYSFS_seek(m_file, pos);
}

qint64 PhysFile::size(void) const
{
	if (m_file == Q_NULLPTR)
		return 0;

	return PHYSFS_fileLength(m_file);
}

qint64 PhysFile::readData(char *data, qint64 maxSize)
{
	if (m_file == Q_NULLPTR)
		return -1;

	return PHYSFS_readBytes(m_file, data, maxSize);
}

SGE_END
