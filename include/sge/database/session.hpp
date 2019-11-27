//
//
#ifndef SGE_DATABASE_SESSION_HPP
#define SGE_DATABASE_SESSION_HPP

#include <QList>
#include <QString>
#include <QDomDocument>
#include <QDomElement>

#include <sge/database/common.hpp>
#include <sge/database/filesystem.hpp>
#include <sge/database/group.hpp>
#include <sge/database/value.hpp>
#include <sge/database/asset.hpp>
#include <sge/database/scene.hpp>
#include <sge/database/level.hpp>

SGE_DATABASE_BEGIN

typedef QList<Asset> AssetList;
typedef QList<Scene> SceneList;
typedef QList<Level> LevelList;

class Session {
public:
	Session(void);
	virtual ~Session(void);

public:
	bool init(FileSystem *fs);
	bool load(FileSystem *fs);
	bool save(FileSystem *fs = Q_NULLPTR);
	FileSystem *fs(void);
	const AssetList &assetList(void) const;
	Asset createAsset(const QString &name);
	const SceneList &sceneList(void) const;
	Scene createScene(const QString &name);

private:
	FileSystem *m_fs;
	QDomDocument m_manifest;
	QDomElement m_assetsElement;
	QDomElement m_scenesElement;
	QDomElement m_levelsElement;
};

inline Group Session::root(void) const
{
	Q_ASSERT(m_fs != Q_NULLPTR);

	if (m_root.tagName() != Group::tag || !m_root.hasAttribute(Group::attrName))
		return Group();

	return Group(m_fs, m_root);
}

inline FileSystem *Session::fs(void)
{
	Q_ASSERT(m_fs != Q_NULLPTR);

	return m_fs;
}

SGE_DATABASE_END

#endif // SGE_DATABASE_SESSION_HPP
