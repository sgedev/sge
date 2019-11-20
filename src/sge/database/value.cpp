//
//
#include <QDomNode>
#include <QDomText>
#include <QDomDocument>

#include <sge/database/value.hpp>

SGE_DATABASE_BEGIN

const QString Value::tag("value");
const QString Value::attrName("name");

Value Value::nextValue(const QString &name) const
{
	QDomElement element = m_element.nextSiblingElement(tag);
	while (!element.isNull()) {
		if (name.isEmpty()) {
			if (element.hasAttribute(attrName))
				return Value(m_fs, element);
		} else {
			if (element.attribute(attrName) == name)
				return Value(m_fs, element);
		}
		element = element.nextSiblingElement(tag);
	}

	return Value();
}

bool Value::setString(const QString &v)
{
	if (m_fs->isReadonly())
		return false;

	QDomText text = m_element.ownerDocument().createTextNode(v);
	if (text.isNull())
		return false;

	QString nameValue = name();
	QDomNode parent = m_element.parentNode();
	QDomElement element = m_element.ownerDocument().createElement(tag);

	element.setAttribute(attrName, nameValue);
	element.appendChild(text);
	parent.replaceChild(m_element, element);
	m_element = element;

	return true;
}

SGE_DATABASE_END
