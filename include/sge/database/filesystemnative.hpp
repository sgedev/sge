//
//
#ifndef SGE_DATABASE_FILESYSTEMNATIVE_HPP
#define SGE_DATABASE_FILESYSTEMNATIVE_HPP

#include <sge/database/common.hpp>
#include <sge/database/filesystem.hpp>

SGE_DATABASE_BEGIN

class FileSystemNative : public FileSystem {
public:
	FileSystemNative(void);
	virtual ~FileSystemNative(void);

public:
	bool init(const QString &root);

protected:
	bool isDir(const QString &path) override;
	bool createDir(const QString &dirname) override;
	bool removeDir(const QString &dirname) override;
	QStringList readDir(const QString &dirname) override;
	bool isFile(const QString &path) override;
	FilePtr openFile(const QString &filename, QIODevice::OpenMode mode) override;
	bool removeFile(const QString &filename) override;

private:
	QString m_root;
};

SGE_DATABASE_END

#endif // SGE_DATABASE_FILESYSTEMNATIVE_HPP
