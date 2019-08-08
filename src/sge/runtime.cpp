//
//
#include <sge/runtime.hpp>

SGE_BEGIN

runtime::runtime(void)
	: m_L(NULL)
{
}

runtime::~runtime(void)
{
	if (m_L != NULL)
		shutdown();
}

bool runtime::init(void)
{
	SGE_ASSERT(m_L == NULL);

	std::promise<bool> init_result;
	m_thread = std::thread(&runtime::tmain, this, &init_result);
	if (!m_thread.joinable())
		return false;

	auto init_done = init_result.get_future();
	init_done.wait();

	return init_done.get();
}

void runtime::shutdown(void)
{
	SGE_ASSERT(m_L != NULL);

	if (m_thread.joinable())
		m_thread.join();
}

void runtime::handle_event(const SDL_Event &event)
{
	m_input.handle_event(event);
}

void runtime::update(float elapsed)
{
}

void runtime::rmain(std::promise<bool> *init_result)
{
	uv_loop_init(&m_loop);

	m_input.init();

	uv_run(&m_loop, UV_RUN_DEFAULT);

	m_input.shutdown();

	uv_loop_close(&m_loop);
}

int runtime::pmain(lua_State *L)
{
	auto _this = (runtime *)lua_touserdata(L, 1);
	auto init_result = (std::promise<bool> *)lua_touserdata(L, 2);

	SGE_ASSERT(_this != NULL);
	SGE_ASSERT(init_result != NULL);

	_this->rmain(init_result);
}

void runtime::tmain(std::promise<bool> *init_result)
{
	SGE_ASSERT(m_L == NULL);

	m_L = luaL_newstate();
	if (m_L == NULL) {
		init_result->set_value(false);
		return;
	}

	lua_pushcfunction(m_L, &runtime::pmain);
	lua_pushlightuserdata(m_L, this);
	lua_pushlightuserdata(m_L, init_result);
	int status = lua_pcall(m_L, 2, 1, 0);
	int result = lua_toboolean(m_L, -1);
	lua_close(m_L);
	m_L = NULL;
}

SGE_END

