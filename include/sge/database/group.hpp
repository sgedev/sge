//
//
#ifndef SGE_DATABASE_GROUP_HPP
#define SGE_DATABASE_GROUP_HPP

#include <QString>
#include <QDomElement>

#include <sge/database/common.hpp>
#include <sge/database/filesystem.hpp>
#include <sge/database/value.hpp>
#include <sge/database/asset.hpp>

SGE_DATABASE_BEGIN

class Group {
public:
	static const QString tag;
	static const QString attrName;

public:
	Group(void);
	Group(const Group &that);
	Group(FileSystem *fs, const QDomElement &element);

public:
	bool isNull(void) const;
	QString name(void) const;
	Group firstChildGroup(const QString &name) const;
	Group nextGroup(void) const;
	Group createChildGroup(const QString &name);
	bool removeChildGroup(const QString &name);
	Value firstChildValue(const QString &name) const;
	Value createChildValue(const QString &name);
	bool removeChildValue(const QString &name);
	Asset firstChildAsset(const QString &name) const;
	Asset createChildAsset(const QString &name);
	bool removeChildAsset(const QString &name);

public:
	Group &operator=(const Group &that);
	bool operator==(const Group &that) const;
	bool operator!=(const Group &that) const;

private:
	FileSystem *m_fs;
	QDomElement m_element;
};

inline Group::Group(void)
	: m_fs(Q_NULLPTR)
{
}

inline Group::Group(const Group &that)
	: m_fs(that.m_fs)
	, m_element(that.m_element)
{
}

inline Group::Group(FileSystem *fs, const QDomElement &element)
	: m_fs(fs)
	, m_element(element)
{
	Q_ASSERT(m_fs != Q_NULLPTR);
	Q_ASSERT(m_element.tagName() == tag);
	Q_ASSERT(m_element.hasAttribute(attrName));
}

inline bool Group::isNull(void) const
{
	return m_element.isNull();
}

inline QString Group::name(void) const
{
	return m_element.attribute(attrName);
}

inline Group &Group::operator=(const Group &that)
{
	if (this != &that) {
		m_fs = that.m_fs;
		m_element = that.m_element;
	}
	return (*this);
}

inline bool Group::operator==(const Group &that) const
{
	return ((m_fs == that.m_fs) && (m_element == that.m_element));
}

inline bool Group::operator!=(const Group &that) const
{
	return !((*this) == that);
}

SGE_DATABASE_END

#endif // SGE_DATABASE_GROUP_HPP
