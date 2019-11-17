//
//
#ifndef SGE_EDITOR_NATIVEFILESYSTEM_HPP
#define SGE_EDITOR_NATIVEFILESYSTEM_HPP

#include <QString>

#include <sge/filesystem.hpp>
#include <sge/editor/common.hpp>

SGE_EDITOR_BEGIN

class NativeFileSystem : public FileSystem {
	Q_OBJECT

public:
	NativeFileSystem(QObject *parent = Q_NULLPTR);
	virtual ~NativeFileSystem(void);

public:
	bool init(const QString &root);
	bool load(const QString &root);

protected:
	bool isDirectory(const QString &path) override;
	bool isFile(const QString &path) override;
	qint64 fileSize(const QString &filename) override;
	FilePtr openFile(const QString &filename, QIODevice::OpenMode mode) override;
	bool removeFile(const QString &filename) override;
	bool makeDirectory(const QString &dirname) override;
	bool removeDirectory(const QString &dirname) override;

private:
	QString m_root;
};

SGE_EDITOR_END

#endif // SGE_EDITOR_NATIVEFILESYSTEM_HPP
