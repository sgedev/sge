//
//
#ifndef SGE_DATABASE_HPP
#define SGE_DATABASE_HPP

#include <QCache>
#include <QString>
#include <QStringList>
#include <QSharedPointer>
#include <QIODevice>
#include <QDomDocument>
#include <QDomElement>

#include <sge/common.hpp>

SGE_BEGIN

class Database {
public:
	enum Type {
		TypeNull = 0,
		TypeGroup,
		TypeBool,
		TypeNumber,
		TypeString,
		TypeBlob
	};

	typedef QSharedPointer<QIODevice> BlobPtr;

public:
	Database(void);
	virtual ~Database(void);

public:
	bool isReadonly(void) const;
	bool isExists(const QString &path);
	Type type(const QString &path);
	bool toBool(const QString &path, bool def_value = false);
	int toInt(const QString &path, int def_value = -1);
	double toDouble(const QString &path, double def_value = 0.0);
	QString toString(const QString &path, const QString &def_value = QString());
	BlobPtr toBlob(const QString &path);
	bool remove(const QString &path);
	bool setBool(const QString &path, bool value, bool overWrite = false);
	bool setInt(const QString &path, int value, bool overWrite = false);
	bool setDouble(const QString &path, double value, bool overWrite = false);
	bool setString(const QString &path, const QString &value, bool overWrite = false);
	bool setBlob(const QString &path, BlobPtr blob, bool overWrite = false);

protected:
	static const QString blobsPath;
	static const QString manifestPath;
	static const QString tagGroup;
	static const QString tagBool;
	static const QString tagNumber;
	static const QString tagString;
	static const QString tagBlob;
	static const QString attrName;

	bool doInit(void);
	bool doLoad(void);
	bool doSave(void);
	void setReadonly(bool v);
	QString baseName(const QString &path);
	QString pathName(const QString &path);
	bool checkPath(const QString &path) const;
	void cleanup(void);
	Type elementType(QDomElement element);
	QDomElement createGroup(const QString &path);
	QDomElement elementFromPath(const QString &path);
	QDomElement findChildElement(QDomElement parent, const QString &name) const;

protected:
	virtual bool isDir(const QString &path) const = 0;
	virtual bool isFile(const QString &path) const = 0;
	virtual bool createFile(const QString &filename) = 0;
	virtual bool removeFile(const QString &filename) = 0;
	virtual bool createDir(const QString &dirname) = 0;
	virtual bool removeDir(const QString &dirname) = 0;
	virtual BlobPtr blobFromFile(const QString &path) = 0;

private:
	typedef QCache<const QString, QDomElement> ElementCache;
	typedef QCache<const QString, Type> TypeCache;

	QDomDocument m_manifest;
	QDomElement m_root;
	ElementCache m_elementCache;
	TypeCache m_typeCache;
	bool m_readonly;
};

inline bool Database::isReadonly(void) const
{
	return m_readonly;
}

inline bool Database::isExists(const QString &path)
{
	return (!elementFromPath(path).isNull());
}

SGE_END

#endif // SGE_DATABASE_HPP
