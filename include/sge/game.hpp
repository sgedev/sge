//
//
#ifndef SGE_GAME_HPP
#define SGE_GAME_HPP

#include <QList>
#include <QSharedPointer>
#include <QThread>
#include <QMutex>

#include <sge/common.hpp>
#include <sge/asset.hpp>
#include <sge/database.hpp>
#include <sge/scene.hpp>
#include <sge/level.hpp>
#include <sge/renderer.hpp>

SGE_BEGIN

class Game: public QObject {
public:
	enum State {
		StateIdel = 0,
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
	bool load(Database::FileSystem *dbfs);
	void shutdown(void);
	void update(float elapsed);
	void draw(Renderer::View *view);
	const AssetList &assetList(void) const;
	const SceneList &sceneList(void) const;
	const LevelList &levelList(void) const;

protected:
	State m_state;
	Database::Session m_db;
	AssetList m_assetList;
	SceneList m_sceneList;
	LevelList m_levelList;
};

SGE_END

#endif // SGE_GAME_HPP

