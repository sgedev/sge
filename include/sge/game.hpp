//
//
#ifndef SGE_GAME_HPP
#define SGE_GAME_HPP

#include <future>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <functional>

#include <lua.hpp>

#include <sge/common.hpp>
#include <sge/event.hpp>
#include <sge/filesystem.hpp>
#include <sge/scene.hpp>
#include <sge/view.hpp>

SGE_BEGIN

class game {
public:
	typedef sge_game_task_t task_t;

	enum state {
		STATE_IDLE = 0,
		STATE_LOADING,
		STATE_READY,
		STATE_PLAYING,
	};

public:
	game(void);
	virtual ~game(void);

public:
	std::function<unsigned int (void)> trap_fps;
	std::function<bool (void)> trap_editor_enabled;

public:
	bool init(filesystem *fs);
	void shutdown(void);
	bool handle_event(const event *evt);
	void update(float elapsed);
	void draw(view *v);

private:
	static void quit_async_cb(uv_async_t *p);
	void init_traps(void);
	void schedule(void);
	void gmain(std::promise<bool> *init_result);
	static int pmain(lua_State *L);
	void tmain(std::promise<bool> *init_result);

private:
	enum trap_type {
		TRAP_TYPE_INVALID = 0,
		TRAP_TYPE_FPS,
		TRAP_TYPE_WINDOW_VISIBLED,
		TRAP_TYPE_WINDOW_SHOW,
		TRAP_TYPE_WINDOW_HIDE,
		TRAP_TYPE_WINDOW_TITLE,
		TRAP_TYPE_WINDOW_SET_TITLE,
		TRAP_TYPE_WINDOW_POSITION,
		TRAP_TYPE_WINDOW_MOVE,
		TRAP_TYPE_WINDOW_SIZE,
		TRAP_TYPE_WINDOW_RESIZE,
		TRAP_TYPE_EDITOR_ENABLED
	};

	enum trap_result {
		TRAP_RESULT_OK = 0,
		TRAP_RESULT_NOT_SET,
		TRAP_RESULT_NOT_IMPL
	};

	int trap_fe(lua_State *L, trap_type tt);
	int trap_be(lua_State *L, trap_type tt);

private:
	static int trap_version_fe(lua_State *L);
	static int trap_task_fe(lua_State *L);
	static int trap_current_fe(lua_State *L);
	static void trap_sleep_done(uv_timer_t *timer);
	static int trap_sleep_fe(lua_State *L);
	static int trap_fps_fe(lua_State *L);
	int trap_fps_be(lua_State *L);
	static int trap_editor_enabled_fe(lua_State *L);
	int trap_editor_enabled_be(lua_State *L);

private:
	filesystem* m_fs;
	lua_State *m_L;
	std::thread m_thread;
	std::mutex m_mutex;
	std::condition_variable_any m_cond;
	uv_loop_t m_loop;
	uv_async_t m_quit_async;
	bool m_running;
	state m_state;
	cx_list_t m_task_list;
	cx_list_t m_task_list_sleep;
	trap_type m_current_trap;
	trap_result m_current_trap_result;
	lua_State *m_current_trap_L;
	scene m_scene;
};

SGE_END

#endif // SGE_GAME_HPP
