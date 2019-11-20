//
//
#include <QDomText>
#include <QDomDocument>

#include <sge/database/group.hpp>

SGE_DATABASE_BEGIN

const QString Group::tag("group");
const QString Group::attrName("name");

Group Group::firstChildGroup(const QString &name) const
{
	QDomElement element = m_element.firstChildElement(tag);
	while (!element.isNull()) {
		if (name.isEmpty()) {
			if (element.hasAttribute(attrName))
				return Group(m_fs, element);
		} else {
			if (element.attribute(attrName) == name)
				return Group(m_fs, element);
		}
		element = element.nextSiblingElement(tag);
	}

	return Group();
}

Group Group::nextGroup(void) const
{
	QDomElement element = m_element.nextSiblingElement(tag);
	while (!element.isNull()) {
		if (element.hasAttribute(attrName))
			return Group(m_fs, element);

		element = element.nextSiblingElement(tag);
	}

	return Group();
}

Group Group::createChildGroup(const QString &name)
{
	if (m_fs->isReadonly())
		return Group();

	Group old = firstChildGroup(name);
	if (!old.isNull())
		return Group();

	QDomElement element = m_element.ownerDocument().createElement(tag);
	if (element.isNull())
		return Group();

	element.setAttribute(attrName, name);
	m_element.appendChild(element);

	return Group(m_fs, element);
}

bool Group::removeChildGroup(const QString &name)
{
	if (m_fs->isReadonly())
		return false;

	Group g = firstChildGroup(name);
	if (g.isNull())
		return false;

	m_element.removeChild(g.m_element);

	return true;
}

Value Group::firstChildValue(const QString &name) const
{
	QDomElement element = m_element.firstChildElement(Value::tag);
	while (!element.isNull()) {
		if (name.isEmpty()) {
			if (element.hasAttribute(attrName))
				return Value(m_fs, element);
		} else {
			if (element.attribute(attrName) == name)
				return Value(m_fs, element);
		}
		element = element.nextSiblingElement(Value::tag);
	}

	return Value();
}

Value Group::createChildValue(const QString &name)
{
	if (m_fs->isReadonly())
		return Value();

	Value old = firstChildValue(name);
	if (!old.isNull())
		return Value();

	QDomElement element = m_element.ownerDocument().createElement(Value::tag);
	if (element.isNull())
		return Value();

	element.setAttribute(Value::attrName, name);
	m_element.appendChild(element);

	return Value(m_fs, element);
}

bool Group::removeChildValue(const QString &name)
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

Asset Group::firstChildAsset(const QString &name) const
{
	QDomElement element = m_element.firstChildElement(Asset::tag);
	while (!element.isNull()) {
		if (name.isEmpty()) {
			if (element.hasAttribute(attrName))
				return Asset(m_fs, element);
		} else {
			if (element.attribute(attrName) == name)
				return Asset(m_fs, element);
		}
		element = element.nextSiblingElement(Asset::tag);
	}

	return Asset();
}

Asset Group::createChildAsset(const QString &name)
{
	if (m_fs->isReadonly())
		return Asset();

	QDomElement element = m_element.ownerDocument().createElement(Asset::tag);
	if (element.isNull())
		return Asset();

	QString id = m_fs->createArchive();
	if (id.isEmpty())
		return Asset();

	QDomText text = m_element.ownerDocument().createTextNode(id);
	if (text.isNull())
		return Asset();

	element.appendChild(text);
	element.setAttribute(Asset::attrName, name);

	m_element.appendChild(element);

	return Asset(m_fs, element);
}

bool Group::removeChildAsset(const QString &name)
{
	if (m_fs->isReadonly())
		return false;

	QDomElement element = m_element.firstChildElement(Asset::tag);
	while (!element.isNull()) {
		if (element.attribute(Asset::attrName) == name) {
			m_element.removeChild(element);
			return true;
		}
		element = element.nextSiblingElement(Asset::tag);
	}

	return false;
}

SGE_DATABASE_END
