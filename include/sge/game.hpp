//
//
#ifndef SGE_GAME_HPP
#define SGE_GAME_HPP

#include <map>
#include <string>
#include <thread>
#include <mutex>
#include <future>
#include <condition_variable>

#include <ttvfs.h>
#include <pugixml.hpp>

#include <sge/common.hpp>
#include <sge/event.hpp>
#include <sge/scene.hpp>
#include <sge/view.hpp>

SGE_BEGIN

class Game {
public:
	typedef SGE_Game_Task_t Task_t;

	enum state {
		STATE_IDLE = 0,
		STATE_INITIALIZING,
		STATE_LOADING,
		STATE_READY,
		STATE_PLAYING,
		STATE_PAUSED,
		STATE_FAILED,
		STATE_EXITED
	};

public:
	Game(void);
	virtual ~Game(void);

public:
	virtual bool init(ttvfs::Root *root);
	virtual void shutdown(void);
	virtual bool handleEvent(const Event *evt);
	virtual void update(float elapsed);
	virtual void draw(View *v);

public:
	void addLuaTask(lua_State *L);
	void removeLuaTask(lua_State *L);
	void resumeLuaTask(lua_State *L, int n);
	void yieldLuaTask(lua_State *L, int n);

private:
	static void quitAsync(uv_async_t *p);
	void initTraps(void);
	bool loadMainTask(void);
	void schedule(void);
	void gmain(std::promise<bool> *init_result);
	static int pmain(lua_State *L);
	void tmain(std::promise<bool> *init_result);

protected:
	enum TrapType {
		TRAP_INVALID = 0,
		TRAP_FPS,
		TRAP_EDITOR_IS_ENABLED
	};

	enum TrapResult {
		TRAP_RESULT_NOT_SET = -1,
		TRAP_RESULT_NOT_SUPPORT = -2
	};

	virtual int trapFps(lua_State *L) = 0;
	virtual int trapEditorIsEnabled(lua_State *L) = 0;

private:
	int trapFE(lua_State *L, TrapType tt);
	int trapBE(lua_State *L, TrapType tt);

private:
	static int trapVersion(lua_State *L);
	static int trapTask(lua_State *L);
	static int trapCurrent(lua_State *L);
	static void sleepDone(uv_timer_t *timer);
	static int trapSleep(lua_State *L);
	static int trapFpsFE(lua_State *L);
	static int trapEditorIsEnabledFE(lua_State *L);

private:
	ttvfs::Root *m_root;
	pugi::xml_document m_manifest;
	lua_State *m_L;
	std::thread m_luaThread;
	std::mutex m_mutex;
	std::condition_variable_any m_cond;
	uv_loop_t m_loop;
	uv_async_t m_quitAsync;
	bool m_running;
	state m_state;
	CXList m_taskList;
	CXList m_taskListSleep;
	Task_t *m_taskCurrent;
	TrapType m_currentTrap;
	int m_currentTrapResult;
	lua_State *m_currentTrapLua;
	Scene m_scene;
};

SGE_END

#endif // SGE_GAME_HPP

