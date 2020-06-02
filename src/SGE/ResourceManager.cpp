//
//
#include <QChar>
#include <QFileInfo>

#include <SGE/ResourceManager.hpp>

SGE_BEGIN

ResourceManager::ResourceManager(QObject *parent):
	QObject(parent)
{
}

ResourceManager::~ResourceManager(void)
{
}

bool ResourceManager::init(const QString &baseDir)
{
	Q_ASSERT(!m_base);

	m_base.reset(new Package(baseDir));
	if (!m_base || !m_base->init())
		return false;

	return true;
}

ResourcePtr ResourceManager::get(const QString &path)
{
	if (!m_base)
		return ResourcePtr();

	if (!checkPath(path) || path.endsWith('/'))
		return ResourcePtr();

	for (auto it(m_packageMapList.begin()); it != m_packageMapList.end(); ++it) {
		if (!path.startsWith(it->first))
			continue;
		ResourcePtr res(it->second->get(path));
		if (res)
			return res;
	}

	return m_base->get(path);
}

bool ResourceManager::registerPackage(const QString &nativePath, const QString &path)
{
	if (!m_base)
		return false;

	if (!checkPath(path))
		return false;

	QString rpath = path;
	if (!rpath.endsWith('/'))
		rpath += '/';

	for (auto it(m_packageMapList.begin()); it != m_packageMapList.end(); ++it) {
		if (it->second->nativePath() == nativePath && it->first == rpath)
			return false;
	}

	PackagePtr pkg;

	QFileInfo info(nativePath);
	if (info.isDir())
		pkg.reset(new Package(nativePath));
	else
		pkg.reset(new ZipPackage(nativePath));

	if (!pkg->init())
		return false;

	m_packageMapList.push_front(qMakePair(rpath, pkg));

	return true;
}

bool ResourceManager::unregisterPackage(const QString &nativePath, const QString &path)
{
	if (!m_base)
		return false;

	if (!checkPath(path))
		return false;

	QString rpath = path;
	if (!rpath.endsWith('/'))
		rpath += '/';

	for (auto it(m_packageMapList.begin()); it != m_packageMapList.end(); ++it) {
		if (it->second->nativePath() == nativePath && it->first == rpath) {
			m_packageMapList.erase(it);
			return true;
		}
	}

	return false;
}

void ResourceManager::unregisterAllPackages(void)
{
	m_packageMapList.clear();
}

bool ResourceManager::isNameChar(QChar c)
{
	return (c.isLetterOrNumber() || c == '_' || c == '.');
}

bool ResourceManager::checkName(const QString &name)
{
	if (name.isNull() || name.isEmpty())
		return false;
	
	for (int i = 1; i < name.size(); ++i) {
		if (!isNameChar(name[i]))
			return false;
	}

	return true;
}

bool ResourceManager::checkPath(const QString &path)
{
	if (path.isNull() || path.size() < 2)
		return false;

	if (!path.startsWith('/'))
		return false;

	bool slashed = true;

	for (int i = 1; i < path.size(); ++i) {
		QChar c = path[i];

		if (c == '/') {
			if (slashed)
				return false;
			slashed = true;
			continue;
		}

		if (!isNameChar(c))
			return false;

		slashed = false;
	}

	return true;
}

SGE_END
