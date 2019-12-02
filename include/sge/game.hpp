//
//
#ifndef SGE_GAME_HPP
#define SGE_GAME_HPP

#include <QList>
#include <QSharedPointer>
#include <QThread>
#include <QMutex>

#include <sge/common.hpp>
#include <sge/filesystem.hpp>
#include <sge/asset.hpp>
#include <sge/scene.hpp>
#include <sge/level.hpp>
#include <sge/renderer.hpp>

SGE_BEGIN

typedef QSharedPointer<Asset> AssetPtr;
typedef QSharedPointer<Scene> ScenePtr;
typedef QSharedPointer<Level> LevelPtr;

typedef QList<AssetPtr> AssetList;
typedef QList<ScenePtr> SceneList;
typedef QList<LevelPtr> LevelList;

class Game {
public:
	enum State {
		StateIdle = 0,
		StateInitializing,
		StateLoading,
		StateReady,
		StatePlaying,
		StatePaused,
		StateFailed,
		StateExited
	};

public:
	Game(void);
	virtual ~Game(void);

public:
	bool init(FileSystem *fs);
	bool load(FileSystem *fs);
	bool save(FileSystem *fs);
	void update(float elapsed);
	void draw(Renderer::View *view);
	State state(void) const;
	const QString &name(void) const;
	void rename(const QString &name);
	int versionMajor(void) const;
	int versionMinor(void) const;
	const AssetList &assetList(void) const;
	const SceneList &sceneList(void) const;
	const LevelList &levelList(void) const;
	const LevelPtr currentLevel(void) const;

protected:
	QString m_name;
	int m_versionMajor;
	int m_versionMinor;
	State m_state;
	AssetList m_assetList;
	SceneList m_sceneList;
	LevelList m_levelList;
	LevelPtr m_currentLevel;
};

inline Game::State Game::state(void) const
{
	return m_state;
}

inline const QString &Game::name(void) const
{
	return m_name;
}

inline void Game::rename(const QString &name)
{
	m_name = name;
}

inline int Game::versionMajor(void) const
{
	return m_versionMajor;
}

inline int Game::versionMinor(void) const
{
	return m_versionMinor;
}

inline const AssetList &Game::assetList(void) const
{
	return m_assetList;
}

inline const SceneList &Game::sceneList(void) const
{
	return m_sceneList;
}

inline const LevelList &Game::levelList(void) const
{
	return m_levelList;
}

inline const LevelPtr Game::currentLevel(void) const
{
	return m_currentLevel;
}

SGE_END

#endif // SGE_GAME_HPP

