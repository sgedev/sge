//
//
#ifndef SGE_DATABASE_ASSET_HPP
#define SGE_DATABASE_ASSET_HPP

#include <QString>
#include <QDomElement>

#include <sge/database/common.hpp>
#include <sge/database/filesystem.hpp>

SGE_DATABASE_BEGIN

class Asset {
public:
	static const QString tag;
	static const QString attrName;

public:
	Asset(void);
	Asset(const Asset &that);
	Asset(FileSystem *fs, const QDomElement &element);

public:
	bool isNull(void) const;
	QString name(void) const;
	Asset nextAsset(void) const;
	FilePtr open(QIODevice::OpenMode mode = QIODevice::ReadOnly);

public:
	Asset &operator=(const Asset &that);
	bool operator==(const Asset &that) const;
	bool operator!=(const Asset &that) const;

protected:
	QString archiveId(void) const;

private:
	FileSystem *m_fs;
	QDomElement m_element;
};

inline Asset::Asset(void)
	: m_fs(Q_NULLPTR)
{
}

inline Asset::Asset(const Asset &that)
	: m_fs(that.m_fs)
	, m_element(that.m_element)
{
}

inline Asset::Asset(FileSystem *fs, const QDomElement &element)
	: m_fs(fs)
	, m_element(element)
{
	Q_ASSERT(m_fs != Q_NULLPTR);
	Q_ASSERT(m_element.tagName() == tag);
	Q_ASSERT(m_element.hasAttribute(attrName));
	Q_ASSERT(!m_element.text().isEmpty());
}

inline bool Asset::isNull(void) const
{
	return m_element.isNull();
}

inline QString Asset::name(void) const
{
	return m_element.tagName();
}

inline Asset &Asset::operator=(const Asset &that)
{
	if (this != &that) {
		m_fs = that.m_fs;
		m_element = that.m_element;
	}
	return (*this);
}

inline bool Asset::operator==(const Asset &that) const
{
	return ((m_fs == m_fs) && (m_element == that.m_element));
}

inline bool Asset::operator!=(const Asset &that) const
{
	return !((*this) == that);
}

inline QString Asset::archiveId(void) const
{
	return m_element.nodeValue();
}

SGE_DATABASE_END

#endif // SGE_DATABASE_ASSET_HPP
