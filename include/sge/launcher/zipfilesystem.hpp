//
//
#ifndef SGE_LAUNCHER_ZIPFILESYSTEM_HPP
#define SGE_LAUNCHER_ZIPFILESYSTEM_HPP

#include <miniz.h>

#include <sge/filesystem.hpp>
#include <sge/launcher/common.hpp>

SGE_LAUNCHER_BEGIN

class ZipFileSystem : public FileSystem {
	Q_OBJECT

public:
	ZipFileSystem(QObject *parent = Q_NULLPTR);
	virtual ~ZipFileSystem(void);

public:
	bool load(const QString &filename);

protected:
	bool isDirectory(const QString &path) override;
	bool isFile(const QString &path) override;
	qint64 fileSize(const QString &filename) override;
	FilePtr openFile(const QString &filename, QIODevice::OpenMode mode) override;
	bool removeFile(const QString &filename) override;
	bool makeDirectory(const QString &dirname) override;
	bool removeDirectory(const QString &dirname) override;

private:
	mz_zip_archive m_zip;
};

SGE_LAUNCHER_END

#endif // SGE_LAUNCHER_ZIPFILESYSTEM_HPP
