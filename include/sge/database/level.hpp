//
//
#ifndef SGE_DATABASE_LEVEL_HPP
#define SGE_DATABASE_LEVEL_HPP

#include <QString>
#include <QDomElement>

#include <sge/database/common.hpp>
#include <sge/database/filesystem.hpp>

SGE_DATABASE_BEGIN

class Level {
public:
	static const QString tag;
	static const QString attrName;

public:
	Level(void);
	Level(const Level &that);
	Level(FileSystem *fs, const QDomElement &element);

public:
	bool isNull(void) const;
	QString name(void) const;
	Level next(void) const;

public:
	Level &operator=(const Level &that);
	bool operator==(const Level &that) const;
	bool operator!=(const Level &that) const;

private:
	FileSystem *m_fs;
	QDomElement m_element;
};

inline Level::Level(void)
	: m_fs(Q_NULLPTR)
{
}

inline Level::Level(const Level &that)
	: m_fs(that.m_fs)
	, m_element(that.m_element)
{
}

inline Level::Level(FileSystem *fs, const QDomElement &element)
	: m_fs(fs)
	, m_element(element)
{
	Q_ASSERT(m_fs != Q_NULLPTR);
	Q_ASSERT(m_element.tagName() == tag);
	Q_ASSERT(m_element.hasAttribute(attrName));
	Q_ASSERT(!m_element.text().isEmpty());
}

inline bool Level::isNull(void) const
{
	return m_element.isNull();
}

inline QString Level::name(void) const
{
	return m_element.tagName();
}

inline Level &Level::operator=(const Level &that)
{
	if (this != &that) {
		m_fs = that.m_fs;
		m_element = that.m_element;
	}
	return (*this);
}

inline bool Level::operator==(const Level &that) const
{
	return ((m_fs == m_fs) && (m_element == that.m_element));
}

inline bool Level::operator!=(const Level &that) const
{
	return !((*this) == that);
}

SGE_DATABASE_END

#endif // SGE_DATABASE_LEVEL_HPP
