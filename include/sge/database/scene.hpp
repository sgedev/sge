//
//
#ifndef SGE_DATABASE_SCENE_HPP
#define SGE_DATABASE_SCENE_HPP

#include <QString>
#include <QDomElement>

#include <sge/database/common.hpp>
#include <sge/database/filesystem.hpp>

SGE_DATABASE_BEGIN

class Scene {
public:
	static const QString tag;
	static const QString attrName;

public:
	Scene(void);
	Scene(const Scene &that);
	Scene(FileSystem *fs, const QDomElement &element);

public:
	bool isNull(void) const;
	QString name(void) const;
	Scene next(void) const;

public:
	Scene &operator=(const Scene &that);
	bool operator==(const Scene &that) const;
	bool operator!=(const Scene &that) const;

private:
	FileSystem *m_fs;
	QDomElement m_element;
};

inline Scene::Scene(void)
	: m_fs(Q_NULLPTR)
{
}

inline Scene::Scene(const Scene &that)
	: m_fs(that.m_fs)
	, m_element(that.m_element)
{
}

inline Scene::Scene(FileSystem *fs, const QDomElement &element)
	: m_fs(fs)
	, m_element(element)
{
	Q_ASSERT(m_fs != Q_NULLPTR);
	Q_ASSERT(m_element.tagName() == tag);
	Q_ASSERT(m_element.hasAttribute(attrName));
	Q_ASSERT(!m_element.text().isEmpty());
}

inline bool Scene::isNull(void) const
{
	return m_element.isNull();
}

inline QString Scene::name(void) const
{
	return m_element.tagName();
}

inline Scene &Scene::operator=(const Scene &that)
{
	if (this != &that) {
		m_fs = that.m_fs;
		m_element = that.m_element;
	}
	return (*this);
}

inline bool Scene::operator==(const Scene &that) const
{
	return ((m_fs == m_fs) && (m_element == that.m_element));
}

inline bool Scene::operator!=(const Scene &that) const
{
	return !((*this) == that);
}

inline QString Scene::archiveId(void) const
{
	return m_element.nodeValue();
}

SGE_DATABASE_END

#endif // SGE_DATABASE_SCENE_HPP
