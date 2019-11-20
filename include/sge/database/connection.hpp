//
//
#ifndef SGE_DATABASE_CONNECTION_HPP
#define SGE_DATABASE_CONNECTION_HPP

#include <QDomDocument>

#include <sge/database/common.hpp>
#include <sge/database/filesystem.hpp>
#include <sge/database/group.hpp>
#include <sge/database/value.hpp>

SGE_DATABASE_BEGIN

class Connection {
public:
	Connection(void);

public:
	bool reset(FileSystem *fs);
	bool load(FileSystem *fs);
	bool save(FileSystem *fs);
	Group root(void) const;

private:
	FileSystem *m_fs;
	QDomDocument m_manifest;
	QDomElement m_root;
};

inline Group Connection::root(void) const
{
	if (m_root.tagName() != Group::tag || !m_root.hasAttribute(Group::attrName))
		return Group();

	return Group(m_fs, m_root);
}

SGE_DATABASE_END

#endif // SGE_DATABASE_CONNECTION_HPP
