//
//
#include <sge/vm.hpp>

SGE_BEGIN

static inline vm::task_t *vm_task_from_lua(lua_State *L)
{
	return (vm::task_t *)lua_getextraspace(L);
}

static inline lua_State *vm_task_to_lua(vm::task_t *T)
{
	return (lua_State *)CX_PMOVB(T, LUA_EXTRASPACE);
}

static inline void vm_attach(vm *V, lua_State *L)
{
	vm_task_from_lua(L)->data = V;
}

static inline vm *vm_from_task(vm::task_t *T)
{
	return (vm *)(T->data);
}

static inline vm *vm_from_lua(lua_State *L)
{
	return vm_from_task(vm_task_from_lua(L));
}

vm::vm(void)
	: m_L(NULL)
{
}

vm::~vm(void)
{
}

bool vm::init(void)
{
	SGE_ASSERT(m_L == NULL);

	return true;

	std::promise<bool> init_result;
	m_thread = std::thread(&vm::tmain, this, &init_result);
	if (!m_thread.joinable())
		return false;

	auto init_done = init_result.get_future();
	init_done.wait();

	if (!init_done.get())
		return false;

	m_state = STATE_LOADING;

	return true;
}

void vm::shutdown(void)
{
	return;

	SGE_ASSERT(m_L != NULL);

	uv_async_send(&m_quit_async);

	if (m_thread.joinable()) {
		m_thread.join();
	}
}

bool vm::handle_event(const event *evt)
{
	return false;
}

void vm::update(float elapsed)
{
	m_scene.update(elapsed);
}

void vm::draw(view *v)
{
	switch (m_state) {
	case STATE_IDLE:
		break;
	case STATE_LOADING:
		break;
	case STATE_READY:
		break;
	case STATE_PLAYING:
		break;
	}

	m_scene.draw(v);
}

void vm::quit_async_cb(uv_async_t* p)
{
	//m_running = false;
	//uv_stop(&m_loop);
}

void vm::schedule(void)
{
}

void vm::gmain(std::promise<bool> *init_result)
{
	SGE_ASSERT(init_result != NULL);

	uv_loop_init(&m_loop);
	uv_async_init(&m_loop, &m_quit_async, &vm::quit_async_cb);
	m_running = true;

	init_traps();

	init_result->set_value(true);

	while (m_running) {
		uv_run(&m_loop, UV_RUN_DEFAULT);
		schedule();
	}

	uv_loop_close(&m_loop);
}

int vm::pmain(lua_State *L)
{
	auto gp = (vm *)lua_touserdata(L, 1);
	auto init_result = (std::promise<bool>*)lua_touserdata(L, 2);

	SGE_ASSERT(init_result != NULL);

	gp->gmain(init_result);

	return 0;
}

void vm::tmain(std::promise<bool> *init_result)
{
	int ret;

	SGE_ASSERT(m_L == NULL);

	m_L = luaL_newstate();
	if (m_L == NULL) {
		init_result->set_value(false);
		return;
	}

	vm_attach(this, m_L);

	lua_pushcfunction(m_L, &pmain);
	lua_pushlightuserdata(m_L, this);
	lua_pushlightuserdata(m_L, init_result);

	ret = lua_pcall(m_L, 2, 1, 0);
	// TODO

	ret = lua_toboolean(m_L, -1);
	// TODO

	lua_close(m_L);
	m_L = NULL;
}

void vm::init_traps(void)
{
	lua_newtable(m_L);

	lua_pushcfunction(m_L, &vm::trap_version_fe);
	lua_setfield(m_L, -2, "version");

	lua_pushcfunction(m_L, &vm::trap_task_fe);
	lua_setfield(m_L, -2, "task");

	lua_pushcfunction(m_L, &vm::trap_current_fe);
	lua_setfield(m_L, -2, "current");

	lua_pushcfunction(m_L, &vm::trap_sleep_fe);
	lua_setfield(m_L, -2, "sleep");

	lua_pushcfunction(m_L, &vm::trap_fps_fe);
	lua_setfield(m_L, -2, "fps");

	lua_newtable(m_L);

	lua_pushcfunction(m_L, &vm::trap_editor_enabled_fe);
	lua_setfield(m_L, -2, "enabled");

	lua_setfield(m_L, -2, "editor");

	lua_setglobal(m_L, "sge");
}

int vm::trap_fe(lua_State *L, trap_type tt)
{
	SGE_ASSERT(m_current_trap == TRAP_TYPE_INVALID);
	SGE_ASSERT(m_current_trap_L == NULL);
	SGE_ASSERT(L != NULL);
	SGE_ASSERT(tt != TRAP_TYPE_INVALID);

	m_current_trap = tt;
	m_current_trap_L = L;
	m_current_trap_result = TRAP_RESULT_NOT_SET;

	m_cond.wait(m_mutex);

	SGE_ASSERT(m_current_trap_result != TRAP_RESULT_NOT_SET);

	switch (m_current_trap_result) {
	case TRAP_RESULT_NOT_IMPL:
		luaL_error(L, "trap %d not impl.", tt);
		break;
	}

	return m_current_trap_result;
}

int vm::trap_be(lua_State *L, trap_type tt)
{
	int ret;

	switch (tt) {
	case TRAP_TYPE_FPS:
		ret = trap_fps_be(L);
		break;
	case TRAP_TYPE_EDITOR_ENABLED:
		ret = trap_editor_enabled_be(L);
		break;
	default:
		ret = TRAP_RESULT_NOT_IMPL;
		break;
	}

	return ret;
}

int vm::trap_version_fe(lua_State *L)
{
	lua_newtable(L);

	lua_pushinteger(L, SGE_VERSION_MAJOR);
	lua_rawseti(L, -2, 0);

	lua_pushinteger(L, SGE_VERSION_MAJOR);
	lua_rawseti(L, -2, 1);

	lua_pushinteger(L, SGE_VERSION_MAJOR);
	lua_rawseti(L, -2, 2);

	return 1;
}

int vm::trap_task_fe(lua_State *L)
{
	luaL_argcheck(L, lua_isfunction(L, 1), 1, "not a function");

	lua_State *L1 = lua_newthread(L);
	if (L1 != NULL) {
		lua_pushvalue(L, 1);
		lua_xmove(L, L1, 1);
	} else
		lua_pushnil(L);

	return 1;
}

int vm::trap_current_fe(lua_State *L)
{
	if (L == vm_from_lua(L)->m_L)
		luaL_error(L, "Not in a thread");

	lua_pushthread(L);

	return 1;
}

void vm::trap_sleep_done(uv_timer_t *timer)
{
	task_t *T = (task_t *)(timer->data);
	SGE_ASSERT(T != NULL);

	vm *V = vm_from_task(T);
	SGE_ASSERT(V != NULL);

	cx_list_del_node(&V->m_task_list_sleep, &T->node);
	cx_list_add_tail(&V->m_task_list, &T->node);
}

int vm::trap_sleep_fe(lua_State *L)
{
	int ms = luaL_checkinteger(L, 1);
	if (ms < 0)
		luaL_error(L, "bad arg");

	if (ms > 0) {
		task_t *T = vm_task_from_lua(L);
		SGE_ASSERT(T != NULL);
		vm *V = vm_from_task(T);
		SGE_ASSERT(V != NULL);
		uv_timer_start(&T->sleep_timer, &vm::trap_sleep_done, ms, 0);
		cx_list_add_tail(&V->m_task_list_sleep, &T->node);
	}

	return lua_yield(L, 0);
}

int vm::trap_fps_fe(lua_State *L)
{
	vm *V = vm_from_lua(L);
	SGE_ASSERT(V != NULL);

	return V->trap_fe(L, TRAP_TYPE_FPS);
}

int vm::trap_fps_be(lua_State *L)
{
	SGE_ASSERT(L != NULL);

	if (!trap_fps)
		return TRAP_RESULT_NOT_IMPL;

	lua_pushinteger(L, trap_fps());

	return 1;
}

int vm::trap_editor_enabled_fe(lua_State *L)
{
	vm *V = vm_from_lua(L);
	SGE_ASSERT(V != NULL);

	return V->trap_fe(L, TRAP_TYPE_EDITOR_ENABLED);
}

int vm::trap_editor_enabled_be(lua_State *L)
{
	SGE_ASSERT(L != NULL);

	if (!trap_editor_enabled)
		return TRAP_RESULT_NOT_IMPL;

	lua_pushboolean(L, trap_editor_enabled());

	return 1;
}

SGE_END

extern "C" void sge_vm_init(lua_State *L)
{
}

extern "C" void sge_vm_shutdown(lua_State *L)
{
}

extern "C" void sge_vm_add_task(lua_State *L, lua_State *L1)
{
}

extern "C" void sge_vm_remove_task(lua_State *L, lua_State *L1)
{
}

extern "C" void sge_vm_resume_task(lua_State *L, int n)
{
}

extern "C" void sge_vm_yield_task(lua_State *L, int n)
{
}

