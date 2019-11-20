//
//
#ifndef SGE_DATABASE_FILESYSTEMZIP_HPP
#define SGE_DATABASE_FILESYSTEMZIP_HPP

#include <QMap>
#include <QString>

#include <miniz.h>

#include <sge/database/common.hpp>
#include <sge/database/filesystem.hpp>

SGE_DATABASE_BEGIN

class FileSystemZip: public FileSystem {
public:
	FileSystemZip(void);
	virtual ~FileSystemZip(void);

public:
	bool init(const QString &filename);

protected:
	bool isDir(const QString &path) override;
	bool createDir(const QString &dirname) override;
	bool removeDir(const QString &dirname) override;
	QStringList readDir(const QString &dirname) override;
	bool isFile(const QString &path) override;
	FilePtr openFile(const QString &filename, QIODevice::OpenMode mode) override;
	bool removeFile(const QString &filename) override;

private:
	typedef QMap<QString, int> FileList;

	mz_zip_archive m_zip;
	FileList m_fileList;
};

SGE_DATABASE_END

#endif // SGE_DATABASE_FILESYSTEMZIP_HPP
