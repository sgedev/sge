//
//
#ifndef SGE_GAME_HPP
#define SGE_GAME_HPP

#include <QList>
#include <QThread>
#include <QMutex>
#include <QTimer>

#include <lua.hpp>

#include <sge/common.hpp>
#include <sge/renderer.hpp>

SGE_BEGIN

class Game: public QThread {
	Q_OBJECT

public:
	Game(void);
	virtual ~Game(void);

public:
	enum LogType {
		LogInfo,
		LogWarning,
		LogError
	};

public:
	const QString &dir(void) const;
	void setDir(const QString &dirname);
	const QString &name(void) const;
	void update(float elapsed);
	void draw(Renderer::View *view);

public: // for lua internal
	static void luaInitHook(lua_State *L);
	static void luaShutdownHook(lua_State *L);
	static void luaAddTaskHook(lua_State *L, lua_State *L1);
	static void luaRemoveTaskHook(lua_State *L, lua_State *L1);
	static void luaResumeTaskHook(lua_State *L, int n);
	static void luaYieldTaskHook(lua_State *L, int n);

signals:
	void trapLog(LogType type, const QString &text);
	int trapFps(void);
	int trapMaxFps(void);
	int trapSetMaxFps(int v);

protected:
    void run(void) Q_DECL_OVERRIDE;
	static int luaMain(lua_State *L);
	void main(lua_State *L);
	void initExports(lua_State *L);
	bool initMainTask(lua_State *L);
	void sched(lua_State *L);

protected:
	static int luaVersion(lua_State *L);
	static int luaInfo(lua_State *L);
	static int luaWarning(lua_State *L);
	static int luaError(lua_State *L);
	static int luaTask(lua_State *L);
	static int luaCurrent(lua_State *L);
	static int luaSleep(lua_State *L);
	static int luaFps(lua_State *L);
	static int luaMaxFps(lua_State *L);
	static int luaSetMaxFps(lua_State *L);
	static int luaName(lua_State *L);
	static int luaSetName(lua_State *L);

protected:
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

protected slots:
	void onTaskReady(Task *task);

private:
	QString m_dir;
	QMutex m_mutex;
	QString m_name;
	CXList m_readyTaskList;
	CXList m_sleepTaskList;
	Task *m_mainTask;
	bool m_running;
};

inline const QString &Game::dir(void) const
{
	return m_dir;
}

inline void Game::setDir(const QString &dirname)
{
	Q_ASSERT(!isRunning());
	m_dir = dirname;
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

SGE_END

#endif // SGE_GAME_HPP
