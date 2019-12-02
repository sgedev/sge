//
//
#ifndef SGE_FILESYSTEMNATIVE_HPP
#define SGE_FILESYSTEMNATIVE_HPP

#include <sge/common.hpp>
#include <sge/filesystem.hpp>

SGE_BEGIN

class FileSystemNative : public FileSystem {
public:
	FileSystemNative(void);
	virtual ~FileSystemNative(void);

public:
	bool init(const QString &dirname);
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
	QString m_path;
};

inline const QString &FileSystemNative::path(void) const
{
	return m_path;
}

SGE_END

#endif // SGE_FILESYSTEMNATIVE_HPP
