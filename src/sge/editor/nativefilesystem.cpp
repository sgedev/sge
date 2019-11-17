//
//
#include <QDir>
#include <QFile>
#include <QFileInfo>

#include <sge/editor/nativefilesystem.hpp>

SGE_EDITOR_BEGIN

NativeFileSystem::NativeFileSystem(QObject *parent)
	: FileSystem(parent)
{
}

NativeFileSystem::~NativeFileSystem(void)
{

}

bool NativeFileSystem::init(const QString &root)
{
	QFileInfo fi(root);
	if (!fi.exists() || !fi.isDir())
		return false;

	m_root = root;
	setReadonly(false);

	if (!doInit())
		return false;

	return true;
}

bool NativeFileSystem::load(const QString &root)
{
	QFileInfo fi(root);
	if (!fi.exists() || !fi.isDir())
		return false;

	m_root = root;
	qDebug() << root;
	setReadonly(false);

	if (!doLoad())
		return false;

	return true;
}

bool NativeFileSystem::isDirectory(const QString &path)
{
	qDebug() << m_root + path;
	QFileInfo fi(m_root + path);
	return fi.isDir();
}

bool NativeFileSystem::isFile(const QString &path)
{
	qDebug() << path;
	QFileInfo fi(m_root + path);
	return fi.isFile();
}

qint64 NativeFileSystem::fileSize(const QString &filename)
{
	qDebug() << filename;
	QFileInfo fi(m_root + filename);
	return fi.size();
}

FilePtr NativeFileSystem::openFile(const QString &filename, QIODevice::OpenMode mode)
{
	FilePtr file(new QFile(m_root + filename));
	if (!file)
		return FilePtr();
	if (!file->open(mode))
		return FilePtr();
	return file;
}

bool NativeFileSystem::removeFile(const QString &filename)
{
	return QFile::remove(m_root + filename);
}

bool NativeFileSystem::makeDirectory(const QString &dirname)
{
	QDir root(m_root);
	QString path(dirname.data() + 1);
	return root.mkpath(path);
}

bool NativeFileSystem::removeDirectory(const QString &dirname)
{
	QDir root(m_root);
	QString path(dirname.data() + 1);
	return root.rmdir(path);
}

SGE_EDITOR_END
