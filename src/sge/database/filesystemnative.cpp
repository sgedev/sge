//
//
#include <QFileInfo>
#include <QDir>
#include <QFile>

#include <sge/database/filesystemnative.hpp>

SGE_DATABASE_BEGIN

FileSystemNative::FileSystemNative(void)
{
	setReadonly(false);
}

FileSystemNative::~FileSystemNative(void)
{
}

bool FileSystemNative::init(const QString &root)
{
	QFileInfo fi(root);
	if (!fi.isDir())
		return false;

	m_root = root;

	return true;
}

bool FileSystemNative::isDir(const QString &path)
{
	QFileInfo fi(m_root + path);
	return fi.isDir();
}

bool FileSystemNative::createDir(const QString &dirname)
{
	QDir dir(m_root);
	return dir.mkpath(QString(dirname.data() + 1));
}

bool FileSystemNative::removeDir(const QString &dirname)
{
	QDir dir(m_root);
	return dir.rmdir(QString(dirname.data() + 1));
}

QStringList FileSystemNative::readDir(const QString &dirname)
{
	QDir dir(m_root + dirname);
	return dir.entryList();
}

bool FileSystemNative::isFile(const QString &path)
{
	QFileInfo fi(m_root + path);
	return fi.isFile();
}

FilePtr FileSystemNative::openFile(const QString &filename, QIODevice::OpenMode mode)
{
	FilePtr file(new QFile(m_root + filename));
	if (!file || !file->open(mode))
		return FilePtr();
	return file;
}

bool FileSystemNative::removeFile(const QString &filename)
{
	QFile file(m_root + filename);
	return file.remove();
}

SGE_DATABASE_END
