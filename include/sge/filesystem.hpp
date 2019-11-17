//
//
#ifndef SGE_DATABASE_CONNECTION_HPP
#define SGE_DATABASE_CONNECTION_HPP

#include <QObject>
#include <QString>
#include <QStringList>
#include <QSharedPointer>
#include <QIODevice>
#include <QDomDocument>

#include <sge/common.hpp>

SGE_BEGIN

typedef QSharedPointer<QIODevice> FilePtr;

class FileSystem: public QObject {
	Q_OBJECT

public:
	enum Type {
		TypeNull = 0,
		TypeDirectory,
		TypeFile
	};

public:
	FileSystem(QObject *parent = Q_NULLPTR);
	virtual ~FileSystem(void);

public:
	bool exists(const QString &path);
	bool mkdir(const QString &dirname);
	bool rmdir(const QString &dirname);
	bool remove(const QString &filename);
	bool entryList(const QString &dirname, QStringList &entryList);
	Type pathType(const QString &path);
	FilePtr open(const QString &filename, QIODevice::OpenMode mode);
	bool isReadonly(void) const;

protected:
	static const int manifestVersion;
	static const QString archivePath;
	static const QString manifestPath;

	bool doInit(void);
	bool doLoad(void);
	void setReadonly(bool v);
	bool checkPath(const QString &path);
	void cleanup(void);
	QDomElement elementFromPath(const QString &path);
	QDomElement findChildElement(QDomElement parent, const QString &name);

protected:
	virtual bool isDirectory(const QString &path) = 0;
	virtual bool isFile(const QString &path) = 0;
	virtual qint64 fileSize(const QString &filename) = 0;
	virtual FilePtr openFile(const QString &filename, QIODevice::OpenMode mode) = 0;
	virtual bool removeFile(const QString &filename) = 0;
	virtual bool makeDirectory(const QString &dirname) = 0;
	virtual bool removeDirectory(const QString &dirname) = 0;

private:
	QDomDocument m_manifest;
	QDomElement m_root;
	bool m_readonly;
};

inline void FileSystem::setReadonly(bool v)
{
	m_readonly = v;
}

inline bool FileSystem::isReadonly(void) const
{
	return m_readonly;
}

SGE_END

#endif // SGE_DATABASE_CONNECTION_HPP
