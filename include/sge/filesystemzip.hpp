//
//
#ifndef SGE_FILESYSTEMZIP_HPP
#define SGE_FILESYSTEMZIP_HPP

#include <QMap>
#include <QString>

#include <miniz.h>

#include <sge/common.hpp>
#include <sge/filesystem.hpp>

SGE_BEGIN

class FileSystemZip: public FileSystem {
public:
	FileSystemZip(void);
	virtual ~FileSystemZip(void);

public:
	bool init(const QString &filename);
	const QString &path(void) const;

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

	QString m_path;
	mz_zip_archive m_zip;
	FileList m_fileList;
};

inline const QString &FileSystemZip::path(void) const
{
	return m_path;
}

SGE_END

#endif // SGE_FILESYSTEMZIP_HPP
