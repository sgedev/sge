//
//
#include <QDomNode>
#include <QDomNodeList>
#include <QDomElement>

#include <sge/filesystem.hpp>

SGE_BEGIN

const int FileSystem::manifestVersion = 1;
const QString FileSystem::archivePath("/archive");
const QString FileSystem::manifestPath("/manifest.xml");

FileSystem::FileSystem(QObject *parent)
	: QObject(parent)
	, m_readonly(false)
{
}

FileSystem::~FileSystem(void)
{
}

bool FileSystem::exists(const QString &path)
{
	if (!checkPath(path))
		return false;

	return !elementFromPath(path).isNull();
}

bool FileSystem::mkdir(const QString &dirname)
{
	if (!checkPath(dirname))
		return false;

	if (isReadonly())
		return false;

	QStringList list = dirname.split('/', QString::SkipEmptyParts);
	QDomElement parent = m_root;
	QDomElement element;

	for (auto it(list.begin()); it != list.end(); ++it) {
		element = findChildElement(parent, *it);
		if (element.isNull()) {
			element.setTagName("dir");
			element.setAttribute("name", *it);
			parent.appendChild(element);
		}
		parent = element;
	}

	return true;
}

bool FileSystem::rmdir(const QString &dirname)
{
	if (!checkPath(dirname))
		return false;

	if (isReadonly())
		return false;

	QDomElement element = elementFromPath(dirname);
	if (element.isNull())
		return false;

	if (element == m_root)
		return false;

	if (element.tagName() != "dir")
		return false;
	
	QDomNode parent = element.parentNode();
	if (parent.isNull())
		return false;

	parent.removeChild(element);

	return true;
}

bool FileSystem::remove(const QString &filename)
{
	if (!checkPath(filename))
		return false;

	if (isReadonly())
		return false;

	QDomElement element = elementFromPath(filename);
	if (element.isNull())
		return false;

	if (element == m_root)
		return false;

	if (element.tagName() != "file")
		return false;

	QDomNode parent = element.parentNode();
	if (parent.isNull())
		return false;

	if (element.hasAttribute("id")) {
		QString id = element.attribute("id");
		QString path = archivePath + "/" + id;
		if (!removeFile(path))
			return false;
	}

	parent.removeChild(element);

	return true;
}

FileSystem::Type FileSystem::pathType(const QString &path)
{
	if (!checkPath(path))
		return TypeNull;

	QDomElement element = elementFromPath(path);
	if (element.isNull())
		return TypeNull;

	if (element.tagName() == "dir")
		return TypeDirectory;

	if (element.tagName() == "file")
		return TypeFile;

	return TypeNull;
}

bool FileSystem::entryList(const QString &dirname, QStringList &entryList)
{
	if (!checkPath(dirname))
		return false;

	QDomElement element = elementFromPath(dirname);
	if (element.isNull())
		return false;

	entryList.clear();

	QDomNodeList list = element.childNodes();
	for (int i = 0; i < list.count(); ++i) {
		QDomElement curr = list.at(i).toElement();
		if (curr.isNull())
			continue;
		if (curr.tagName() == "file" || curr.tagName() == "dir")
			entryList << curr.tagName();
	}

	return true;
}

FilePtr FileSystem::open(const QString &path, QIODevice::OpenMode mode)
{
	if (!checkPath(path))
		return FilePtr();

	QDomElement element = elementFromPath(path);
	if (element.isNull()) {
		if (mode & QIODevice::ExistingOnly)
			return FilePtr();
	} else {
		if (mode & QIODevice::NewOnly)
			return FilePtr();
	}

	if (mode & (QIODevice::WriteOnly | QIODevice::NewOnly | QIODevice::Truncate | QIODevice::Append)) {
		if (isReadonly())
			return FilePtr();
		if (element.isNull()) {
			
		}
	}

	if (element.isNull())
		return FilePtr();
	if (element.tagName() != "file")
		return FilePtr();

	return openFile(archivePath + element.text(), mode);
}

bool FileSystem::doInit(void)
{
	if (isReadonly())
		return false;

	if (!makeDirectory(archivePath))
		return false;

	m_manifest.clear();

	QDomElement root = m_manifest.createElement("sge");
	root.setAttribute("version", manifestVersion);
	m_manifest.appendChild(root);
	m_manifest.normalize();

	FilePtr file = openFile(manifestPath, QIODevice::WriteOnly | QIODevice::Truncate);
	if (!file)
		return false;

	qDebug() << m_manifest.toByteArray();

	file->write(m_manifest.toByteArray());
	file->close();

	return doLoad();
}

bool FileSystem::doLoad(void)
{
	if (!isDirectory(archivePath))
		return false;

	if (!isFile(manifestPath))
		return false;

	FilePtr file = openFile(manifestPath, QIODevice::ReadOnly);
	if (!file)
		return false;

	if (!m_manifest.setContent(file.data()))
		return false;

	m_root = m_manifest.firstChildElement("sge");
	if (m_root.isNull())
		return false;

	return true;
}

bool FileSystem::checkPath(const QString &path)
{
	if (path.size() < 1)
		return false;

	if (path.at(0) != '/')
		return false;

	bool slashed = true;

	for (int i = 1; i < path.size(); ++i) {
		QChar c = path.at(i);
		if (c == '/') {
			if (slashed)
				return false;
			slashed = true;
		} else {
			if (!c.isLetterOrNumber() && c != '_' && c != '.')
				return false;
			slashed = false;
		}
	}

	return true;
}

void FileSystem::cleanup(void)
{
	// TODO
}

QDomElement FileSystem::elementFromPath(const QString &path)
{
	Q_ASSERT(checkPath(path));
	Q_ASSERT(!m_manifest.isNull());
	Q_ASSERT(!m_root.isNull());

	if (path == "/")
		return m_root;

	QDomElement element;
	QDomElement parent = m_root;
	QStringList list = path.split('/', QString::SkipEmptyParts);

	for (auto it(list.begin()); it != list.end(); ++it) {
		element = findChildElement(parent, *it);
		if (element.isNull())
			return QDomElement();
		parent = element;
	}

	return element;
}

QDomElement FileSystem::findChildElement(QDomElement parent, const QString &name)
{
	Q_ASSERT(!parent.isNull());
	Q_ASSERT(!name.isEmpty());

	QDomElement element;
	QDomNodeList list = parent.childNodes();

	for (int i = 0; i < list.count(); ++i) {
		element = list.at(i).toElement();
		if (element.isNull())
			continue;
		if (element.tagName() == "dir" || element.tagName() == "file") {
			if (name.isEmpty())
				return element;
			if (element.attribute("name") == name)
				return element;
		}
	}

	return QDomElement();
}

SGE_END
