//
//
#ifndef SGE_RESOURCEMANAGER_HPP
#define SGE_RESOURCEMANAGER_HPP

#include <QMap>
#include <QPair>
#include <QList>
#include <QString>
#include <QSharedPointer>

#include <SGE/Common.hpp>
#include <SGE/Package.hpp>
#include <SGE/ZipPackage.hpp>

SGE_BEGIN

typedef IODevicePtr ResourcePtr;

class Q_DECL_EXPORT ResourceManager: public QObject {
	Q_OBJECT

public:
	ResourceManager(QObject *parnet = Q_NULLPTR);
	virtual ~ResourceManager(void);

public:
	bool init(const QString &baseDir);
	ResourcePtr get(const QString &path);
	bool registerPackage(const QString &nativePath, const QString &path = QString("/"));
	bool unregisterPackage(const QString &nativePath, const QString &path);
	void unregisterAllPackages(void);
	static bool isNameChar(QChar c);
	static bool checkName(const QString &name);
	static bool checkPath(const QString &path);

private:
	typedef QSharedPointer<Package> PackagePtr;
	typedef QPair<QString, PackagePtr> PackageMap;
	typedef QList<PackageMap> PackageMapList;

	PackagePtr m_base;
	PackageMapList m_packageMapList;
};

SGE_END

#endif // SGE_RESOURCEMANAGER_HPP
