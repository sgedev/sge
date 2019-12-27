//
//
#ifndef SGE_GAME_HPP
#define SGE_GAME_HPP

#include <QString>
#include <QList>
#include <QSharedPointer>
#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QTimer>

#include <lua.hpp>

#include <sge/common.hpp>
#include <sge/renderer.hpp>
#include <sge/scene.hpp>
#include <sge/level.hpp>

SGE_BEGIN

typedef QSharedPointer<Level> LevelPtr;
typedef QList<LevelPtr> LevelList;

class Game: public QThread {
	Q_OBJECT

public:
	Game(void);
	virtual ~Game(void);

public:
	bool setPath(const QString &path);
	const QString &path(void) const;
	const QString &name(void) const;
	void update(float elapsed);
	void draw(Renderer::View *view);

public: // for lua internal
	static void initLua(lua_State *L);
	static void shutdownLua(lua_State *L);
	static void addLuaTask(lua_State *L, lua_State *L1);
	static void removeLuaTask(lua_State *L, lua_State *L1);
	static void resumeLuaTask(lua_State *L, int n);
	static void yieldLuaTask(lua_State *L, int n);

private:
	enum TrapType {
		TrapNull = 0,
		TrapInfo,
		TrapWarning,
		TrapError,
		TrapName,
		TrapSetName,
		TrapCreateScene		
	};

	typedef SGEGameContext Context;

	struct Task {
		lua_State *L;
		QTimer sleepTimer;
	};

	static Context *contextFromLua(lua_State *L);
	static lua_State *contextToLua(Context *ctx);
	static Game *gameFromContext(Context *ctx);
	static Game *gameFromLua(lua_State *L);
	static Task *taskFromContext(Context *ctx);
	static Task *taskFromLua(lua_State *L);

	void run(void) override;
	static int luaMain(lua_State *L);
	void main(lua_State *L);
	void initExports(lua_State *L);
	bool initMainTask(lua_State *L);
	void scheduleTasks(lua_State *L);
	static int doTrap(lua_State *L, TrapType trap);
	int makeLuaError(L, const char *errMsg);

private:
	static int luaVersion(lua_State *L);

	static int luaInfo(lua_State *L);
	int trapInfo(lua_State *L);

	static int luaWarning(lua_State *L);
	int trapWarning(lua_State *L);

	static int luaError(lua_State *L);
	int trapError(lua_State *L);

	static int luaTask(lua_State *L);

	static int luaCurrent(lua_State *L);

	static int luaSleep(lua_State *L);

	static int luaName(lua_State *L);
	int trapName(lua_State *L);

	static int luaSetName(lua_State *L);
	int trapSetName(lua_State *L);

	static int luaCreateScene(lua_State *L);
	int trapCreateScene(lua_State *L);

private slots:
	void onTaskReady(Task *task);

private:
	QString m_path;
	QString m_name;
	QMutex m_mutex;
	QWaitCondition m_cond;
	SceneList m_scenes;
	LevelList m_levels;

private:
	cx_list_t m_readyTaskList;
	cx_list_t m_sleepTaskList;
	bool m_running;
	TrapType m_trapType;
	lua_State *m_trapArgs;
	int m_trapResult;
};

inline const QString &Game::path(void) const
{
	return m_path;
}

inline const QString &Game::name(void) const
{
	return m_name;
}

inline Game::Context *Game::contextFromLua(lua_State *L)
{
	Q_ASSERT(L != Q_NULLPTR);
	return (Context *)lua_getextraspace(L);
}

inline lua_State *Game::contextToLua(Context *ctx)
{
	Q_ASSERT(ctx != Q_NULLPTR);
	return (lua_State *)(qintptr(ctx) + LUA_EXTRASPACE);
}

inline Game *Game::gameFromContext(Context *ctx)
{
	Game *game = (Game *)(ctx->game);
	Q_ASSERT(game != Q_NULLPTR);
	return game;
}

inline Game *Game::gameFromLua(lua_State *L)
{
	return gameFromContext(contextFromLua(L));
}

inline Game::Task *Game::taskFromContext(Context *ctx)
{
	Task *task = (Task *)(ctx->task);
	Q_ASSERT(task != Q_NULLPTR);
	return task;
}

inline Game::Task *Game::taskFromLua(lua_State *L)
{
	return taskFromContext(contextFromLua(L));
}

inline int Game::makeTrapError(L, const char *errMsg)
{
	lua_pushstring(L, errMsg);
	return -1;
}

SGE_END

#endif // SGE_GAME_HPP

