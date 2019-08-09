//
//
#ifndef SGE_GAME_HPP
#define SGE_GAME_HPP

#include <future>
#include <thread>
#include <functional>

#include <lua.hpp>

#include <sge/common.hpp>
#include <sge/input.hpp>
#include <sge/scene.hpp>

SGE_BEGIN

class game {
public:
	enum state {
		STATE_IDLE = 0,
		STATE_LOADING,
		STATE_READY,
		STATE_PLAYING,
	};

public:
	class traps {
	protected:
		virtual unsigned int trap_fps(void) = 0;
	};

public:
	game(void);
	virtual ~game(void);

public:
	bool init(traps *p);
	void shutdown(void);
	void handle_event(const SDL_Event &event);
	void update(float elapsed);
	void draw(view &v);
	state current_state(void) const;
	scene &current_scene(void);

private:
	void gmain(std::promise<bool> *init_result);
	static int pmain(lua_State *L);
	void tmain(std::promise<bool> *init_result);
	static void quit_async(uv_async_t *p);
	void init_traps(void);
	void schedule(void);

private:
	lua_State *m_L;
	traps *m_traps;
	std::thread m_thread;
	uv_loop_t m_loop;
	uv_async_t m_quit_async;
	bool m_running;
	input m_input;
	state m_state;
	scene m_scene;
};

inline game::state game::current_state(void) const
{
	return m_state;
}

inline scene &game::current_scene(void)
{
	return m_scene;
}

SGE_END

#endif // SGE_GAME_HPP

