//
//
#ifndef SGE_GAME_HPP
#define SGE_GAME_HPP

#include <mutex>
#include <condition_variable>
#include <future>
#include <thread>
#include <string>
#include <functional>

#include <lua.hpp>

#include <sge/common.hpp>
#include <sge/input.hpp>
#include <sge/scene.hpp>

SGE_BEGIN

class game {
	friend void sge_game_lua_open(lua_State *);
	friend void sge_game_lua_thread(lua_State *, lua_State *);

public:
	typedef sge_game_task task_t;

	enum state {
		STATE_IDLE = 0,
		STATE_INITIALIZING,
		STATE_READY,
		STATE_LOADING,
		STATE_PLAYING,
		STATE_ENDED,
		STATE_FATAL
	};

public:
	std::function<int (void)> trap_fps;
	std::function<bool (void)> trap_window_visibled;
	std::function<void (void)> trap_window_show;
	std::function<void (void)> trap_window_hide;
	std::function<const char *(void)> trap_window_title;
	std::function<void (const char *)> trap_window_set_title;
	std::function<void (int &, int &)> trap_window_position;
	std::function<void (int, int)> trap_window_move;
	std::function<void (int &, int &)> trap_window_size;
	std::function<void (int, int)> trap_window_resize;
	std::function<bool (void)> trap_editor_enabled;

public:
	game(void);
	virtual ~game(void);

public:
	bool init(void);
	void shutdown(void);
	void handle_event(const SDL_Event &event);
	void update(float elapsed);
	void draw(view &v);
	const char *name(void) const;
	state current_state(void) const;
	scene &current_scene(void);

public: // call by lua
	void on_luaclose(lua_State *L);
	void on_luathread(lua_State *L, lua_State *L1);
	void on_luafree(lua_State *L, lua_State *L1);
	void on_luaresume(lua_State *L, int n);
	void on_luayield(lua_State *L, int n);

private:
	void gmain(void);
	static int pmain(lua_State *L);
	void tmain(void);
	static void quit_async(uv_async_t *p);
	void schedule(void);
	bool init_main_task(void);
	void init_traps(void);

private:
	enum trap_type {
		TRAP_INVALID = 0,
		TRAP_FPS,
		TRAP_WINDOW_VISIBLED,
		TRAP_WINDOW_SHOW,
		TRAP_WINDOW_HIDE,
		TRAP_WINDOW_TITLE,
		TRAP_WINDOW_SET_TITLE,
		TRAP_WINDOW_POSITION,
		TRAP_WINDOW_MOVE,
		TRAP_WINDOW_RESIZE,
		TRAP_WINDOW_SIZE,
		TRAP_EDITOR_ENABLED,
		TRAP_MAX
	};

	int on_trap(lua_State *L, trap_type tt);
	int do_trap(lua_State *L, trap_type tt);

	static int on_trap_version(lua_State *L);
	static int on_trap_task(lua_State *L);
	static int on_trap_current(lua_State *L);

	static void on_trap_sleep_done(uv_timer_t *timer);
	static int on_trap_sleep(lua_State *L);

	static int on_trap_fps(lua_State *L);
	int do_trap_fps(lua_State *L);

	static int on_trap_window_visibled(lua_State *L);
	int do_trap_window_visibled(lua_State *L);

	static int on_trap_window_show(lua_State *L);
	int do_trap_window_show(lua_State *L);

	static int on_trap_window_hide(lua_State *L);
	int do_trap_window_hide(lua_State *L);

	static int on_trap_window_title(lua_State *L);
	int do_trap_window_title(lua_State *L);

	static int on_trap_window_set_title(lua_State *L);
	int do_trap_window_set_title(lua_State *L);

	static int on_trap_window_position(lua_State *L);
	int do_trap_window_position(lua_State *L);

	static int on_trap_window_move(lua_State *L);
	int do_trap_window_move(lua_State *L);

	static int on_trap_window_size(lua_State *L);
	int do_trap_window_size(lua_State *L);

	static int on_trap_window_resize(lua_State *L);
	int do_trap_window_resize(lua_State *L);

	static int on_trap_editor_enabled(lua_State *L);
	int do_trap_editor_enabled(lua_State *L);

private:
	std::thread m_thread;
	uv_async_t m_quit_async;
	std::mutex m_mutex;
	std::condition_variable_any m_cond;
	lua_State *m_L;
	uv_loop_t m_loop;
	bool m_running;
	input m_input;
	state m_state;
	scene m_scene;
	std::string m_name;
	trap_type m_current_trap;
	lua_State *m_current_trap_L;
	int m_current_trap_result;
	task_t *m_main_task;
	sge_list m_task_list;
	sge_list m_task_list_sleep;
};

inline const char *game::name(void) const
{
	return m_name.c_str();
}

inline game::state game::current_state(void) const
{
	return m_state;
}

inline scene &game::current_scene(void)
{
	return m_scene;
}

SGE_END

#endif // SGE_VM_HPP

