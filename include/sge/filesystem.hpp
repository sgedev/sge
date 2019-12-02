//
//
#ifndef SGE_FILESYSTEM_HPP
#define SGE_FILESYSTEM_HPP

#include <QString>
#include <QStringList>
#include <QSharedPointer>
#include <QIODevice>
#include <QFileDevice>

#include <miniz.h>

#include <sge/common.hpp>

SGE_BEGIN

typedef QSharedPointer<QIODevice> FilePtr;

#define QIODEVICE_WRITE_MODES \
	(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Truncate | QIODevice::NewOnly)

class FileSystem {
public:
	FileSystem(void);
	virtual ~FileSystem(void);

public:
	bool isReadonly(void) const;
	FilePtr openManifest(QIODevice::OpenMode mode);
	QString createArchive(void);
	bool removeArchive(const QString id);
	bool isArchiveExists(const QString id);
	FilePtr openArchive(const QString id, QIODevice::OpenMode mode);
	QStringList archiveList(void);

protected:
	virtual bool isDir(const QString &path) = 0;
	virtual bool createDir(const QString &dirname) = 0;
	virtual bool removeDir(const QString &dirname) = 0;
	virtual QStringList readDir(const QString &dirname) = 0;
	virtual bool isFile(const QString &path) = 0;
	virtual FilePtr openFile(const QString &filename, QIODevice::OpenMode mode) = 0;
	virtual bool removeFile(const QString &filename) = 0;

protected:
	void setReadonly(bool v);
	bool createArchivePath(void);

protected:
	static const QString archivePath;
	static const QString manifestPath;

private:
	bool m_readonly;
};

inline bool FileSystem::isReadonly(void) const
{
	return m_readonly;
}

inline void FileSystem::setReadonly(bool v)
{
	m_readonly = v;
}

SGE_END

#endif // SGE_FILESYSTEM_HPP
