//
//
#ifndef SGE_GAME_HPP
#define SGE_GAME_HPP

#include <map>
#include <future>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <functional>

#include <lua.hpp>
#include <cx/filesystem.hpp>

#include <sge/common.hpp>
#include <sge/event.hpp>
#include <sge/scene.hpp>
#include <sge/view.hpp>

SGE_BEGIN

class Game {
public:
	typedef SGEGameTask_t Task_t;

	enum state {
		STATE_IDLE = 0,
		STATE_LOADING,
		STATE_READY,
		STATE_PLAYING,
	};

public:
	Game(void);
	virtual ~Game(void);

public:
	virtual bool init(const char *root);
	virtual void shutdown(void);
	virtual bool handleEvent(const Event *evt);
	virtual void update(float elapsed);
	virtual void draw(View *v);

protected:
	enum TrapType {
		TRAP_INVALID = 0,
		TRAP_FPS,
		TRAP_WINDOW_VISIBLED,
		TRAP_WINDOW_SHOW,
		TRAP_WINDOW_HIDE,
		TRAP_WINDOW_TITLE,
		TRAP_WINDOW_SET_TITLE,
		TRAP_WINDOW_POSITION,
		TRAP_WINDOW_MOVE,
		TRAP_WINDOW_SIZE,
		TRAP_WINDOW_RESIZE,
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
	static void quitAsync(uv_async_t *p);
	void initLuaTraps(void);
	bool initMainTask(void);
	void schedule(void);
	void gmain(std::promise<bool> *init_result);
	static int pmain(lua_State *L);
	void tmain(std::promise<bool> *init_result);

private:
	lua_State *m_L;
	std::thread m_thread;
	std::mutex m_mutex;
	std::condition_variable_any m_cond;
	uv_loop_t m_loop;
	uv_async_t m_quit_async;
	bool m_running;
	state m_state;
	CXList m_task_list;
	CXList m_task_list_sleep;
	TrapType m_current_trap;
	int m_current_trap_result;
	lua_State *m_current_trap_L;
	Scene m_scene;
};

SGE_END

#endif // SGE_GAME_HPP
