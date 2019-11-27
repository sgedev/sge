//
//
#ifndef SGE_DATABASE_VALUE_HPP
#define SGE_DATABASE_VALUE_HPP

#include <QString>
#include <QDomNode>
#include <QDomText>
#include <QDomElement>

#include <sge/database/common.hpp>
#include <sge/database/filesystem.hpp>

SGE_DATABASE_BEGIN

class Value {
public:
	static const QString tag;
	static const QString attrName;

public:
	Value(void);
	Value(const Value &that);
	Value(FileSystem *fs, const QDomElement &element);

public:
	bool isNull(void) const;
	QString name(void) const;
	Value next(const QString &name) const;
	bool toBool(void) const;
	int toInt(void) const;
	double toDouble(void) const;
	QString toString(void) const;
	bool setBool(bool v);
	bool setInt(int v);
	bool setDouble(double v);
	bool setString(const QString &v);

public:
	Value &operator=(const Value &that);
	bool operator==(const Value &that);
	bool operator!=(const Value &that);

private:
	FileSystem *m_fs;
	QDomElement m_element;
};

inline Value::Value(void)
	: m_fs(Q_NULLPTR)
{
}

inline Value::Value(const Value &that)
	: m_fs(that.m_fs)
	, m_element(that.m_element)
{
}

inline Value::Value(FileSystem *fs, const QDomElement &element)
	: m_fs(fs)
	, m_element(element)
{
	Q_ASSERT(m_fs != Q_NULLPTR);
	Q_ASSERT(m_element.tagName() == tag);
	Q_ASSERT(m_element.hasAttribute(attrName));
}

inline bool Value::isNull(void) const
{
	return m_element.isNull();
}

inline QString Value::name(void) const
{
	return m_element.attribute(attrName);
}

inline bool Value::toBool(void) const
{
	return m_element.text() == "true";
}

inline int Value::toInt(void) const
{
	return m_element.text().toInt();
}

inline double Value::toDouble(void) const
{
	return m_element.text().toDouble();
}

inline QString Value::toString(void) const
{
	return m_element.text();
}

inline bool Value::setBool(bool v)
{
	return setString(v ? "true" : "false");
}

inline bool Value::setInt(int v)
{
	return setString(QString("%1").arg(v));
}

inline bool Value::setDouble(double v)
{
	return setString(QString("%1").arg(v));
}

inline Value &Value::operator=(const Value &that)
{
	if (this != &that) {
		m_fs = that.m_fs;
		m_element = that.m_element;
	}
	return (*this);
}

inline bool Value::operator==(const Value &that)
{
	return ((m_fs == that.m_fs) && (m_element == that.m_element));
}

inline bool Value::operator!=(const Value &that)
{
	return !((*this) == that);
}

SGE_DATABASE_END

#endif // #define SGE_DATABASE_VALUE_HPP

