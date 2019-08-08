//
//
#ifndef SGE_RUNTIME_HPP
#define SGE_RUNTIME_HPP

#include <future>
#include <thread>
#include <functional>

#include <lua.hpp>

#include <sge/common.hpp>
#include <sge/input.hpp>

SGE_BEGIN

class runtime {
public:
	std::function<int (void)> do_get_fps;
	std::function<void (float)> do_move_forward;
	std::function<void (float)> do_move_backward;
	std::function<void (float)> do_move_left;
	std::function<void (float)> do_move_right;
	std::function<void (float, float)> do_mouse_view;

public:
	runtime(void);
	virtual ~runtime(void);

public:
	bool init(void);
	void shutdown(void);
	void handle_event(const SDL_Event &event);
	void update(float elapsed);

private:
	void rmain(std::promise<bool> *init_result);
	static int pmain(lua_State *L);
	void tmain(std::promise<bool> *init_result);

private:
	lua_State *m_L;
	std::thread m_thread;
	uv_loop_t m_loop;
	input m_input;
};

SGE_END

#endif // SGE_RUNTIME_HPP

