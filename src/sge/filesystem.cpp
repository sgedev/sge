//
//
#include <QChar>
#include <QUuid>
#include <QByteArray>
#include <QFileInfo>
#include <QDir>
#include <QFile>

#include <sge/filesystem.hpp>

SGE_BEGIN

const QString FileSystem::archivePath("/archive");
const QString FileSystem::manifestPath("/manifest");

FileSystem::FileSystem(void)
	: m_readonly(true)
{
}

FileSystem::~FileSystem(void)
{
}

FilePtr FileSystem::openManifest(QIODevice::OpenMode mode)
{
	if (mode & QIODEVICE_WRITE_MODES) {
		if (isReadonly())
			return FilePtr();
		createArchivePath();
	}

	return openFile(manifestPath, mode);
}

QString FileSystem::createArchive(void)
{
	if (isReadonly())
		return QString();

	createArchivePath();

	QUuid uuid = QUuid::createUuid();
	if (uuid.isNull())
		return QString();

	QString id = uuid.toString(QUuid::WithoutBraces);
	QString path = archivePath + "/" + id;

	FilePtr file = openFile(path, QIODevice::NewOnly);
	if (!file)
		return QString();

	return id;
}

bool FileSystem::removeArchive(const QString id)
{
	if (isReadonly())
		return false;

	createArchivePath();

	return removeFile(archivePath + "/" + id);
}

bool FileSystem::isArchiveExists(const QString id)
{
	if (id.isNull() || id.isEmpty())
		return false;

	return isFile(archivePath + "/" + id);
}

FilePtr FileSystem::openArchive(const QString id, QIODevice::OpenMode mode)
{
	if (mode & QIODEVICE_WRITE_MODES) {
		if (isReadonly())
			return FilePtr();
		createArchivePath();
	}

	return openFile(archivePath + "/" + id, mode);
}

QStringList FileSystem::archiveList(void)
{
	if (!isReadonly())
		createArchivePath();

	QStringList list;
	QStringList fileList = readDir(archivePath);

	for (auto it(fileList.begin()); it != fileList.end(); ++it) {
		if (isFile(archivePath + "/" + *it))
			list << *it;
	}

	return list;
}

bool FileSystem::createArchivePath(void)
{
	if (isDir(archivePath))
		return true;

	return createDir(archivePath);
}

SGE_END
