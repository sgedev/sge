//
//
#include <QDomText>
#include <QDomDocument>

#include <sge/database/group.hpp>

SGE_DATABASE_BEGIN

const QString Group::tag("group");
const QString Group::attrName("name");

Group Group::firstGroup(void) const
{
	QDomElement element = m_element.firstChildElement(tag);
	while (!element.isNull()) {
		if (!element.attribute(attrName).isEmpty())
			return Group(m_fs, element);
		element = element.nextSiblingElement(tag);
	}

	return Group();
}

Group Group::next(void) const
{
	QDomElement element = m_element.nextSiblingElement(tag);
	while (!element.isNull()) {
		if (element.attribute(attrName).size() > 0)
			return Group(m_fs, element);
		element = element.nextSiblingElement(tag);
	}

	return Group();
}

Group Group::createGroup(const QString &name)
{
	if (m_fs->isReadonly())
		return Group();

	if (isNameExists(name))
		return Group();

	QDomElement element = m_element.ownerDocument().createElement(tag);
	if (element.isNull())
		return Group();

	element.setAttribute(attrName, name);
	m_element.appendChild(element);

	return Group(m_fs, element);
}

bool Group::removeGroup(const QString &name)
{
	if (m_fs->isReadonly())
		return false;

	QDomElement element = m_element.firstChildElement(tag);
	while (!element.isNull()) {
		if (element.attribute(attrName) == name) {
			m_element.removeChild(element);
			return true;
		}
		element = element.nextSiblingElement(tag);
	}

	return false;
}

Value Group::firstValue(void) const
{
	QDomElement element = m_element.firstChildElement(Value::tag);
	while (!element.isNull()) {
		if (!element.attribute(attrName).isEmpty())
			return Value(m_fs, element);
		element = element.nextSiblingElement(Value::tag);
	}

	return Value();
}

Value Group::createValue(const QString &name)
{
	if (m_fs->isReadonly())
		return Value();

	if (isNameExists(name))
		return Value();

	QDomElement element = m_element.ownerDocument().createElement(Value::tag);
	if (element.isNull())
		return Value();

	element.setAttribute(Value::attrName, name);
	m_element.appendChild(element);

	return Value(m_fs, element);
}

bool Group::removeValue(const QString &name)
{
	if (m_fs->isReadonly())
		return false;

	QDomElement element = m_element.firstChildElement(Value::tag);
	while (!element.isNull()) {
		if (element.attribute(Value::attrName) == name) {
			m_element.removeChild(element);
			return true;
		}
		element = element.nextSiblingElement(Value::tag);
	}

	return false;
}

Blob Group::firstBlob(void) const
{
	QDomElement element = m_element.firstChildElement(Blob::tag);
	while (!element.isNull()) {
		if (!element.attribute(attrName).isEmpty())
			return Blob(m_fs, element);
		element = element.nextSiblingElement(Blob::tag);
	}

	return Blob();
}

Blob Group::createBlob(const QString &name, QIODevice *dev)
{
	if (m_fs->isReadonly())
		return Blob();

	if (isNameExists(name))
		return Blob();

	QDomElement element = m_element.ownerDocument().createElement(Blob::tag);
	if (element.isNull())
		return Blob();

	QString id = m_fs->createArchive();
	if (id.isEmpty())
		return Blob();

	QDomText text = m_element.ownerDocument().createTextNode(id);
	if (text.isNull())
		return Blob();

	element.appendChild(text);
	element.setAttribute(Blob::attrName, name);

	m_element.appendChild(element);

	if (dev == Q_NULLPTR)
		return Blob(m_fs, element);

	FilePtr file = m_fs->openArchive(id, QIODevice::WriteOnly | QIODevice::Truncate);
	if (!file)
		return Blob();

	qint64 size = dev->size();
	while (size > 0) {
		QByteArray data = dev->read(512);
		qint64 len = file->write(data);
		if (len != data.size())
			return Blob();
		size -= data.size();
	}

	return Blob(m_fs, element);
}

Blob Group::createBlobShare(const QString &name, Blob &shareWith)
{
	if (m_fs->isReadonly())
		return Blob();

	if (isNameExists(name) || shareWith.isNull())
		return Blob();

	QDomElement element = m_element.ownerDocument().createElement(Blob::tag);
	if (element.isNull())
		return Blob();

	QString id = shareWith.archiveId();
#if 0
	if (!m_fs->isArchiveExists(id))
		return Blob();
#else
	if (id.isNull() || id.isEmpty())
		return Blob();
#endif

	QDomText text = m_element.ownerDocument().createTextNode(id);
	if (text.isNull())
		return Blob();

	m_element.appendChild(element);

	return Blob(m_fs, element);
}

Blob Group::createBlobCopy(const QString &name, Blob &copyFrom)
{
	if (m_fs->isReadonly())
		return Blob();

	if (isNameExists(name) || copyFrom.isNull())
		return Blob();

	QDomElement element = m_element.ownerDocument().createElement(Blob::tag);
	if (element.isNull())
		return Blob();

	FilePtr file = copyFrom.open(QIODevice::ReadOnly);
	if (!file)
		return Blob();

	return createBlob(name, file.data());
}

bool Group::removeBlob(const QString &name)
{
	if (m_fs->isReadonly())
		return false;

	QDomElement element = m_element.firstChildElement(Blob::tag);
	while (!element.isNull()) {
		if (element.attribute(Blob::attrName) == name) {
			m_element.removeChild(element);
			return true;
		}
		element = element.nextSiblingElement(Blob::tag);
	}

	return false;
}

bool Group::isNameExists(const QString &name) const
{
	QDomElement element;

	for (element = m_element.firstChildElement(); !element.isNull(); element = element.nextSiblingElement()) {
		if (element.attribute(attrName) == name)
			return true;
	}

	return true;
}

SGE_DATABASE_END
