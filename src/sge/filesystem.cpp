//
//
#include <QDir>
#include <QFile>
#include <QUuid>

#include <physfs.h>

#include <sge/filesystem.hpp>

SGE_BEGIN

inline static bool isWriteMode(QIODevice::OpenMode mode)
{
	return (mode & QIODevice::WriteOnly || mode & QIODevice::Append ||
		mode & QIODevice::NewOnly || mode & QIODevice::Truncate);
}

FileSystem::FileSystem(QObject *parent)
	: QObject(parent)
{
	m_virtualRootPath = QString("/%1/").arg(qlonglong(this));
	m_virtualTreePath = m_virtualRootPath + "/tree/";
	m_virtualDataPath = m_virtualRootPath + "/data/";
}

FileSystem::~FileSystem(void)
{
}

bool FileSystem::create(const QString &nativeRootPath)
{
	QDir dir(nativeRootPath);

	dir.mkdir("data");
	dir.mkdir("tree");

	return load(nativeRootPath);
}

bool FileSystem::load(const QString &nativeRootPath)
{
	m_nativeRootInfo.setFile(nativeRootPath);
	if (!m_nativeRootInfo.exists())
		return false;

	if (!PHYSFS_mount(nativeRootPath.toStdString().c_str(), m_virtualRootPath.toStdString().c_str(), 0))
		return false;

	m_nativeRootPath = nativeRootPath;
	if (!m_nativeRootPath.endsWith("/"))
		m_nativeRootPath += "/";
	m_nativeDataPath = m_nativeRootPath + "data/";
	m_nativeTreePath = m_nativeRootPath + "tree/";

	qDebug() << m_virtualTreePath;

	if (!PHYSFS_isDirectory(m_virtualTreePath.toStdString().c_str())) {
		PHYSFS_unmount(m_virtualRootPath.toStdString().c_str());
		return false;
	}

	qDebug() << m_virtualDataPath;

	if (!PHYSFS_isDirectory(m_virtualDataPath.toStdString().c_str())) {
		PHYSFS_unmount(m_virtualRootPath.toStdString().c_str());
		return false;
	}

	return true;
}

bool FileSystem::exists(const QString &path)
{
	if (PHYSFS_exists((m_virtualTreePath + path).toStdString().c_str()))
		return true;

	if (PHYSFS_isDirectory((m_virtualTreePath + path).toStdString().c_str()))
		return true;

	return false;
}

FilePtr FileSystem::open(const QString &path, QIODevice::OpenMode openMode)
{
	FilePtr file;
	QString dataId;
	QString dataPath;
	bool needCreate = false;

	if (isWriteMode(openMode)) {
		if (!m_nativeRootInfo.isDir())
			return false;
		if (!exists(path)) {
			qDebug() << m_nativeTreePath;
			QString treePath = m_nativeTreePath + path;
			qDebug() << treePath;
			QFile treeFile(treePath);
			if (!treeFile.open(QIODevice::WriteOnly | QIODevice::Truncate))
				return false;
			QUuid uuid = QUuid::createUuid();
			qDebug() << uuid;
			dataId = uuid.toString(QUuid::WithoutBraces);
			qDebug() << dataId;
			if (treeFile.write(dataId.toLatin1()) != dataId.length()) {
				qDebug() << treeFile.errorString();
				return false;
			}
			treeFile.flush();
		} else {
			if (openMode & QIODevice::NewOnly)
				return false;
			if (!readDataId(path, dataId))
				return false;
		}
		qDebug() << m_nativeDataPath + dataId;
		file.reset(new QFile(m_nativeDataPath + dataId));
	} else
		file.reset(new PhysFile(m_virtualTreePath + path));

	if (!file || !file->open(openMode))
		return FilePtr();

	qDebug() << file->isOpen();

	return file;
}

bool FileSystem::mkpath(const QString &path)
{
	return true;
}

bool FileSystem::readDataId(const QString &treePath, QString &dataId)
{
	PHYSFS_File *link = PHYSFS_openRead((m_virtualTreePath + treePath).toStdString().c_str());
	if (link == Q_NULLPTR)
		return false;

	char buf[37];
	PHYSFS_sint64 len = PHYSFS_readBytes(link, buf, 36);
	PHYSFS_close(link);
	if (len != 36)
		return false;

	dataId = buf;

	return true;
}

SGE_END
