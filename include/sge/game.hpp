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

signals:
	void trapInfo(const QString &text);
	void trapError(const QString &text);
	void trapWarning(const QString &text);
	int trapFps(void);
	int trapMaxFps(void);
	int trapSetMaxFps(int v);
	//Mode trapMode(void);

public:
	const QString &dir(void) const;
	const QString &name(void) const;
	bool setDir(const QString &dirname);
	void update(float elapsed);
	void draw(Renderer::View *view);

public: // for lua internal
	static void luaInit(lua_State *L);
	static void luaShutdown(lua_State *L);
	static void luaAddTask(lua_State *L, lua_State *L1);
	static void luaRemoveTask(lua_State *L, lua_State *L1);
	static void luaResumeTask(lua_State *L, int n);
	static void luaYieldTask(lua_State *L, int n);

protected:
    void run(void) Q_DECL_OVERRIDE;
	static int luaMain(lua_State *L);
	int gameMain(lua_State *L);
	void initExports(lua_State *L);
	void initMainTask(lua_State *L);
	void sched(lua_State *L);

protected:
	int Game::luaVersionTrap(lua_State *L, QString &errorMsg);
	static int luaVersion(lua_State *L);
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

protected slots:

private:
	typedef SGEGameInfo Info;

	struct Task {
		lua_State *L;
		QTimer sleepTimer;
	};

	static Info *infoFromLua(lua_State *L);
	static lua_State *infoToLua(Info *info);
	static Game *gameFromInfo(Info *info);
	static Game *gameFromLua(lua_State *L);
	static Task *taskFromInfo(Info *info);
	static Task *taskFromLua(lua_State *L);

protected slots:
	void onTaskReady(Task *task);

private:
	QString m_dir;
	QMutex m_mutex;
	QString m_name;
	CXList m_readyTaskList;
	CXList m_sleepTaskList;
	bool m_running;
};

inline const QString &Game::name(void) const
{
	return m_name;
}

inline Game::Info *Game::infoFromLua(lua_State *L)
{
	return (Info *)lua_getextraspace(L);
}

inline lua_State *Game::infoToLua(Info *info)
{
	return (lua_State *)(qintptr(info) + LUA_EXTRASPACE);
}

inline Game *Game::gameFromInfo(Info *info)
{
	Game *game = (Game *)(info->game);
	Q_ASSERT(game != Q_NULLPTR);
	return game;
}

inline Game::Task *Game::taskFromInfo(Info *info)
{
	Task *task = (Task *)(info->task);
	Q_ASSERT(task != Q_NULLPTR);
	return task;
}

inline Game *Game::gameFromLua(lua_State *L)
{
	return gameFromInfo(infoFromLua(L));
}

inline Game::Task *Game::taskFromLua(lua_State *L)
{
	return taskFromInfo(infoFromLua(L));
}

SGE_END

#endif // SGE_GAME_HPP

