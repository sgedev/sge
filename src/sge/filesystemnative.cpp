//
//
#include <QFileInfo>
#include <QDir>
#include <QFile>

#include <sge/filesystemnative.hpp>

SGE_BEGIN

FileSystemNative::FileSystemNative(void)
{
	setReadonly(false);
}

FileSystemNative::~FileSystemNative(void)
{
}

bool FileSystemNative::init(const QString &path)
{
	QFileInfo fi(path);
	if (!fi.isDir())
		return false;

	m_path = path;

	return true;
}

bool FileSystemNative::isDir(const QString &path)
{
	QFileInfo fi(m_path + path);
	return fi.isDir();
}

bool FileSystemNative::createDir(const QString &dirname)
{
	QDir dir(m_path);
	return dir.mkpath(QString(dirname.data() + 1));
}

bool FileSystemNative::removeDir(const QString &dirname)
{
	QDir dir(m_path);
	return dir.rmdir(QString(dirname.data() + 1));
}

QStringList FileSystemNative::readDir(const QString &dirname)
{
	QDir dir(m_path + dirname);
	return dir.entryList();
}

bool FileSystemNative::isFile(const QString &path)
{
	QFileInfo fi(m_path + path);
	return fi.isFile();
}

FilePtr FileSystemNative::openFile(const QString &filename, QIODevice::OpenMode mode)
{
	FilePtr file(new QFile(m_path + filename));
	if (!file || !file->open(mode))
		return FilePtr();
	return file;
}

bool FileSystemNative::removeFile(const QString &filename)
{
	QFile file(m_path + filename);
	return file.remove();
}

SGE_END
