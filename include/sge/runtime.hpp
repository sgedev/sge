//
//
#ifndef SGE_RUNTIME_HPP
#define SGE_RUNTIME_HPP

#include <functional>

#include <lua.hpp>

#include <sge/common.hpp>
#include <sge/input.hpp>

SGE_BEGIN

class runtime {
public:
	std::function<int (void)> do_get_fps;

public:
	runtime(void);
	virtual ~runtime(void);

public:
	bool init(void);
	void shutdown(void);
	void handle_event(const SDL_Event &event);
	void update(float elapsed);

private:
	lua_State *m_L;
	input m_input;
};

SGE_END

#endif // SGE_RUNTIME_HPP

