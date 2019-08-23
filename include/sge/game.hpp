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
	static task_t *task_from_lua(lua_State *L);
	static lua_State *task_to_lua(task_t *T);
	void attach_to_lua(lua_State *L);
	static game *from_task(task_t *T);
	static game *from_lua(lua_State *L);

public: // call by lua
	void luaclose(lua_State *L);
	void luathread(lua_State *L, lua_State *L1);
	void luafree(lua_State *L, lua_State *L1);
	void luaresume(lua_State *L, int n);
	void luayield(lua_State *L, int n);

private:
	void gmain(void);
	static int pmain(lua_State *L);
	void tmain(void);
	static void quit_async(uv_async_t *p);
	void schedule(void);
	bool init_main_task(void);
	void init_traps(void);

protected:
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
		TRAP_TYPE_WINDOW_RESIZE,
		TRAP_TYPE_WINDOW_SIZE,
		TRAP_TYPE_EDITOR_ENABLED,
		TRAP_TYPE_MAX
	};

	static const int TRAP_RESULT_NOT_SET = -1;
	static const int TRAP_RESULT_NOT_IMPL	= -2;

	int trap_fe(lua_State *L, trap_type tt);
	int trap_be(lua_State *L, trap_type tt);

	static int trap_version_fe(lua_State *L);
	static int trap_task_fe(lua_State *L);
	static int trap_current_fe(lua_State *L);

	static void trap_sleep_done(uv_timer_t *timer);
	static int trap_sleep_fe(lua_State *L);

	static int trap_fps_fe(lua_State *L);
	int trap_fps_be(lua_State *L);

	static int trap_window_visibled_fe(lua_State *L);
	int trap_window_visibled_be(lua_State *L);

	static int trap_window_show_fe(lua_State *L);
	int trap_window_show_be(lua_State *L);

	static int trap_window_hide_fe(lua_State *L);
	int trap_window_hide_be(lua_State *L);

	static int trap_window_title_fe(lua_State *L);
	int trap_window_title_be(lua_State *L);

	static int trap_window_set_title_fe(lua_State *L);
	int trap_window_set_title_be(lua_State *L);

	static int trap_window_position_fe(lua_State *L);
	int trap_window_position_be(lua_State *L);

	static int trap_window_move_fe(lua_State *L);
	int trap_window_move_be(lua_State *L);

	static int trap_window_size_fe(lua_State *L);
	int trap_window_size_be(lua_State *L);

	static int trap_window_resize_fe(lua_State *L);
	int trap_window_resize_be(lua_State *L);

	static int trap_editor_enabled_fe(lua_State *L);
	int trap_editor_enabled_be(lua_State *L);

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

inline game::task_t *game::task_from_lua(lua_State *L)
{
	SGE_ASSERT(L != NULL);

	return (game::task_t *)lua_getextraspace(L);
}

inline lua_State *game::task_to_lua(game::task_t *T)
{
	SGE_ASSERT(T != NULL);

	return (lua_State *)SGE_PMOVB(T, sizeof(game::task_t));
}

inline void game::attach_to_lua(lua_State *L)
{
	task_from_lua(L)->data = this;
}

inline game *game::from_task(game::task_t *T)
{
	return (game *)(T->data);
}

inline game *game::from_lua(lua_State *L)
{
	return (game *)(task_from_lua(L)->data);
}

SGE_END

#endif // SGE_VM_HPP

