//
//
#ifndef SGE_DATABASE_FILESYSTEM_HPP
#define SGE_DATABASE_FILESYSTEM_HPP

#include <QMap>
#include <QString>
#include <QStringList>
#include <QObject>

#include <miniz.h>

#include <sge/database/common.hpp>

SGE_DATABASE_BEGIN

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

SGE_DATABASE_END

#endif // SGE_DATABASE_FILESYSTEM_HPP
