//
//
#include <QFileInfo>
#include <QDomNode>
#include <QDomNodeList>

#include <sge/database.hpp>

SGE_BEGIN

const QString Database::blobsPath("/blobs");
const QString Database::manifestPath("/manifest.xml");
const QString Database::tagGroup("group");
const QString Database::tagBool("bool");
const QString Database::tagNumber("number");
const QString Database::tagString("string");
const QString Database::tagBlob("blob");
const QString Database::attrName("name");

Database::Database(void)
	: m_elementCache(64)
	, m_typeCache(64)
{
}

Database::~Database(void)
{
}

Database::Type Database::type(const QString &path)
{
	if (!checkPath(path))
		return TypeNull;

	Type *p = m_typeCache[path];
	if (p != Q_NULLPTR)
		return *p;

	Type type = elementType(elementFromPath(path));
	if (type != TypeNull)
		m_typeCache.insert(path, &type);

	return type;
}

bool Database::toBool(const QString &path, bool def_value)
{
	if (!checkPath(path))
		return def_value;

	QDomElement element = elementFromPath(path);
	if (element.isNull())
		return def_value;

	QString text = element.text();
	if (text.isEmpty())
		return def_value;

	Type type = elementType(element);

	if (type == TypeBool || type == TypeString)
		return (text == "true");

	if (type == TypeNumber)
		return !!text.toInt();

	return def_value;
}

int Database::toInt(const QString &path, int def_value)
{
	if (!checkPath(path))
		return def_value;

	QDomElement element = elementFromPath(path);
	if (element.isNull())
		return def_value;

	QString text = element.text();
	if (text.isEmpty())
		return def_value;

	Type type = elementType(element);
	if (type == TypeNumber)
		return text.toInt();
	if (type == TypeBool || type == TypeString)
		return int(text == "true");

	return def_value;
}

double Database::toDouble(const QString &path, double def_value)
{
	if (!checkPath(path))
		return def_value;

	QDomElement element = elementFromPath(path);
	if (element.isNull())
		return def_value;

	QString text = element.text();
	if (text.isEmpty())
		return def_value;

	Type type = elementType(element);
	if (type == TypeNumber)
		return text.toDouble();
	if (type == TypeBool || type == TypeString)
		return double(text == "true");

	return def_value;
}

QString Database::toString(const QString &path, const QString &def_value)
{
	if (!checkPath(path))
		return def_value;

	QDomElement element = elementFromPath(path);
	if (element.isNull())
		return def_value;

	QString text = element.text();
	if (text.isEmpty())
		return def_value;

	return text;
}

Database::BlobPtr Database::toBlob(const QString &path)
{
	if (!checkPath(path))
		return BlobPtr();

	QDomElement element = elementFromPath(path);
	if (element.isNull() || element.tagName() != "blob")
		return BlobPtr();

	QString text = element.text();
	if (text.isEmpty())
		return BlobPtr();

	QIODevice::OpenMode mode = QIODevice::ReadOnly;
	if (!isReadonly())
		mode |= QIODevice::WriteOnly;

	QString blobPath = blobsPath + "/" + text;
	BlobPtr blob(blobFromFile(blobPath));
	if (!blob | !blob->open(mode))
		return BlobPtr();

	return blob;
}

bool Database::remove(const QString &path)
{
	if (!checkPath(path))
		return false;

	QDomElement element = elementFromPath(path);
	if (element.isNull())
		return false;
	if (element == m_root)
		return false;

	Type type = elementType(element);
	if (type == TypeBlob) {
		QString text = element.text();
		if (!text.isEmpty()) {
			QString blobPath = blobsPath + "/" + text;
			if (!removeFile(blobPath))
				return false;
		}
	}

	QDomNode parent = element.parentNode();
	Q_ASSERT(!parent.isNull());
	parent.removeChild(element);

	return true;
}

bool Database::setBool(const QString &path, bool value, bool overWrite)
{
	if (!checkPath(path))
		return false;

	QDomElement parent;
	QDomElement element = elementFromPath(path);

	if (element.isNull()) {
		QString groupPath = pathName(path);
		parent = createGroup(groupPath);
		if (parent.isNull())
			return false;

		element = m_manifest.createElement(tagBool);
		if (element.isNull())
			return false;


	} else {
		if (!overWrite)
			return false;

	}

	element.text() = value ? "true" : "false";
	element.setAttribute(attrName, baseName(path));

	parent.appendChild(element);

	return true;
}

bool Database::setInt(const QString &path, int value, bool overWrite)
{
	QString groupPath = pathName(path);
	QDomElement parent = createGroup(groupPath);
	if (parent.isNull())
		return false;

	QDomElement element = m_manifest.createElement(tagNumber);
	if (element.isNull())
		return false;

	element.text() = QString("%1").arg(value);
	element.setAttribute(attrName, baseName(path));

	parent.appendChild(element);

	return true;
}

bool Database::setDouble(const QString &path, double value, bool overWrite)
{
	QString groupPath = pathName(path);
	QDomElement parent = createGroup(groupPath);
	if (parent.isNull())
		return false;

	QDomElement element = m_manifest.createElement(tagNumber);
	if (element.isNull())
		return false;

	element.text() = QString("%1").arg(value);
	element.setAttribute(attrName, baseName(path));

	parent.appendChild(element);

	return true;
}

bool Database::setString(const QString &path, const QString &value, bool overWrite)
{
	QString groupPath = pathName(path);
	QDomElement parent = createGroup(groupPath);
	if (parent.isNull())
		return false;

	QDomElement element = m_manifest.createElement(tagString);
	if (element.isNull())
		return false;

	element.text() = value;
	element.setAttribute(attrName, baseName(path));

	parent.appendChild(element);

	return true;
}

bool Database::setBlob(const QString &path, BlobPtr blob, bool overWrite)
{
	if (!checkPath(path))
		return false;

	QString groupPath = pathName(path);
	QDomElement parent = createGroup(groupPath);
	if (parent.isNull())
		return false;

	QDomElement element = m_manifest.createElement(tagBlob);
	if (element.isNull())
		return false;

	element.text() = QString("%1").arg(value);
	element.setAttribute(attrName, baseName(path));

	parent.appendChild(element);

	return true;
}

bool Database::doInit(void)
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

bool Database::doLoad(void)
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

bool Database::doSave(void)
{
	return false;
}

QString Database::baseName(const QString &path)
{
	QFileInfo fi(path);
	return fi.baseName();
}

QString Database::pathName(const QString &path)
{
	QFileInfo fi(path);
	return fi.filePath();
}

bool Database::checkPath(const QString &path)
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

void Database::cleanup(void)
{
	// TODO
}

Database::Type Database::elementType(QDomElement element)
{
	QString tag = element.tagName();
	if (tag == tagGroup)
		return TypeGroup;
	if (tag == tagBool)
		return TypeBool;
	if (tag == tagNumber)
		return TypeNumber;
	if (tag == tagString)
		return TypeString;
	if (tag == tagBlob)
		return TypeBlob;

	return TypeNull;
}

QDomElement Database::createGroup(const QString &path)
{
	QDomElement parent(m_root);
	QDomElement element(m_root);
	QStringList list = path.split('/', QString::SkipEmptyParts);

	for (int i = 0; i < list.count(); ++i) {
		QString name = list.at(i);
		element = findChildElement(parent, name);
		if (element.isNull()) {
			element = m_manifest.createElement(tagGroup);
			element.attribute(attrName, name);
			parent.appendChild(element);
		}
		parent = element;
	}

	return element;
}

QDomElement Database::elementFromPath(const QString &path)
{
	if (path == "/")
		return m_root;

	QDomElement *p = m_elementCache[path];
	if (p != Q_NULLPTR)
		return *p;

	QDomElement element;
	QDomElement parent = m_root;
	QStringList list = path.split('/', QString::SkipEmptyParts);

	for (auto it(list.begin()); it != list.end(); ++it) {
		element = findChildElement(parent, *it);
		if (element.isNull())
			return QDomElement();
		parent = element;
	}

	m_elementCache.insert(path, element);

	return element;
}

QDomElement Database::findChildElement(QDomElement parent, const QString &name)
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
