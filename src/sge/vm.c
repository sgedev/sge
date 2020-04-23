/*
 *
 */
#include <stdarg.h>

#include <sge/vm.h>

typedef enum {
	SGE_VM_STATE_NONE = 0,
	SGE_VM_STATE_INITIALIZING,
	SGE_VM_STATE_RUNNING,
	SGE_VM_STATE_EXITING,
	SGE_VM_STATE_ERROR
} sge_vm_state_t;

typedef enum {
	SGE_VM_TRAP_TYPE_NONE = 0,
	SGE_VM_TRAP_TYPE_GET_FPS,
	SGE_VM_TRAP_TYPE_SET_FPS_MAX,
	SGE_VM_TRAP_TYPE_GET_FPS_MAX
} sge_vm_trap_type_t;

typedef struct {
	sge_vm_trap_type_t type;
	lua_State *stack;
	int result;
} sge_vm_trap_t;

static SDL_Thread *sge_vm_thread;
static sge_vm_state_t sge_vm_state;
static uv_loop_t sge_vm_loop;
static uv_async_t sge_vm_quit;
static cx_list_t sge_vm_task_ready_list;
static cx_list_t sge_vm_task_sleep_list;
static SDL_mutex *sge_vm_mutex;
static SDL_cond *sge_vm_cond;
static sge_vm_trap_t sge_vm_trap;

#define SGE_VM_TASK_FROM_LUA(L) \
	((sge_vm_task_t *)lua_getextraspace(L))

#define SGE_VM_TASK_TO_LUA(T) \
	((lua_State *)CX_PMOVB(T, LUA_EXTRASPACE))

static int sge_vm_do_trap(lua_State *L, sge_vm_trap_type_t type)
{
	int ret;

	CX_ASSERT(L != NULL);
	CX_ASSERT(type != SGE_VM_TRAP_TYPE_NONE);

	CX_ASSERT(SDL_ThreadID() == SDL_GetThreadID(sge_vm_thread));
	CX_ASSERT(sge_vm_trap.type == SGE_VM_TRAP_TYPE_NONE);
	CX_ASSERT(sge_vm_trap.stack == NULL);

	sge_vm_trap.type = type;
	sge_vm_trap.result = 0;
	sge_vm_trap.stack = L;

	SDL_CondWait(sge_vm_cond, sge_vm_mutex);

	ret = sge_vm_trap.result;

	sge_vm_trap.type = SGE_VM_TRAP_TYPE_NONE;
	sge_vm_trap.result = 0;
	sge_vm_trap.stack = NULL;

	if (ret < 0)
		lua_error(L);

	return ret;
}

static int sge_vm_trap_error(lua_State *L, const char *fmt, ...)
{
	va_list args;

	CX_ASSERT(L != NULL);
	CX_ASSERT(SDL_ThreadID() != SDL_GetThreadID(sge_vm_thread));

	va_start(args, fmt);
	lua_pushvfstring(L, fmt, args);
	va_end(args);

	return -1;
}

static int sge_vm_trap_get_version(lua_State *L)
{
	lua_newtable(L);

	lua_pushinteger(L, SGE_VERSION_MAJOR);
	lua_rawset(L, 0);

	lua_pushinteger(L, SGE_VERSION_MINOR);
	lua_rawset(L, 1);

	lua_pushinteger(L, SGE_VERSION_PATCH);
	lua_rawset(L, 2);

	return 1;
}

static int sge_vm_trap_task(lua_State *L)
{
	return 1;
}

static void sge_vm_trap_sleep_done(uv_timer_t *p)
{
	sge_vm_task_t *T;

	CX_ASSERT(p != NULL);
	CX_ASSERT(p->DATA != NULL);

	T = p->data;

	cx_list_node_unlink(&T->node);
	cx_list_add_tail(&sge_vm_task_ready_list, &T->node);
}

static int sge_vm_trap_sleep(lua_State *L)
{
	int ms = luaL_checkinteger(L, 1);
	if (ms > 0) {
		sge_vm_task_t *T = SGE_VM_TASK_FROM_LUA(L);
		uv_timer_start(&T->sleep_timer, sge_vm_trap_sleep_done, ms, 0);
	}

	return lua_yield(L, 0);
}

static int sge_vm_trap_current(lua_State *L)
{
	lua_pushthread(L);
	return 1;
}

static int sge_vm_trap_get_fps(lua_State *L)
{
	return sge_vm_do_trap(L, SGE_VM_TRAP_TYPE_GET_FPS);
}

static int sge_vm_trap_get_fps_be(lua_State *L, const sge_vm_traps_t *traps)
{
	if (traps->get_fps == NULL)
		return sge_vm_trap_error(L, "not impl.");

	lua_pushinteger(L, traps->get_fps());
	return 1;
}

static int sge_vm_trap_get_fps_max(lua_State *L)
{
	return sge_vm_do_trap(L, SGE_VM_TRAP_TYPE_GET_FPS_MAX);
}

static int sge_vm_trap_get_fps_max_be(lua_State *L, const sge_vm_traps_t *traps)
{
	if (traps->get_fps_max == NULL)
		return sge_vm_trap_error(L, "not impl.");

	lua_pushinteger(L, traps->get_fps_max());
	return 1;
}

static int sge_vm_trap_set_fps_max(lua_State *L)
{
	luaL_checkinteger(L, 1);
	return sge_vm_do_trap(L, SGE_VM_TRAP_TYPE_SET_FPS_MAX);
}

static int sge_vm_trap_set_fps_max_be(lua_State *L, const sge_vm_traps_t *traps)
{
	if (traps->set_fps_max == NULL)
		return sge_vm_trap_error(L, "not impl.");

	traps->set_fps_max(lua_tointeger(L, 1));
	return 0;
}

static void sge_vm_init_traps(lua_State *L)
{
	lua_newtable(L);

	lua_pushcfunction(L, &sge_vm_trap_get_version);
	lua_setfield(L, -2, "get_version");

	lua_pushcfunction(L, &sge_vm_trap_task);
	lua_setfield(L, -2, "task");

	lua_pushcfunction(L, &sge_vm_trap_sleep);
	lua_setfield(L, -2, "sleep");

	lua_pushcfunction(L, &sge_vm_trap_current);
	lua_setfield(L, -2, "current");

	lua_pushcfunction(L, &sge_vm_trap_get_fps);
	lua_setfield(L, -2, "get_fps");

	lua_pushcfunction(L, &sge_vm_trap_get_fps_max);
	lua_setfield(L, -2, "get_fps_max");

	lua_pushcfunction(L, &sge_vm_trap_set_fps_max);
	lua_setfield(L, -2, "set_fps_max");

	lua_setglobal(L, "sge");
}

static int sge_vm_load_main_task(lua_State *L)
{
	return 0;
}

static void sge_vm_quit_cb(uv_async_t* handle)
{
	sge_vm_state = SGE_VM_STATE_EXITING;
	uv_stop(&sge_vm_loop);
}

static void sge_vm_schedule(lua_State *L)
{
	cx_list_node_t *node;
	sge_vm_task_t *T;
	lua_State *L1;
	int ret;

	while (!cx_list_empty(&sge_vm_task_ready_list)) {
		node = cx_list_del_head(&sge_vm_task_ready_list);
		T = CX_MEMBEROF(node, sge_vm_task_t, node);
		L1 = SGE_VM_TASK_TO_LUA(T);
		ret = lua_resume(L, L1, 0);
	}
}

static int sge_vm_pmain(lua_State *L)
{
	int ret;

	ret = uv_loop_init(&sge_vm_loop);
	if (ret < 0)
		goto bad0;

	uv_async_init(&sge_vm_loop, &sge_vm_quit, sge_vm_quit_cb);

	luaL_openlibs(L);
	sge_vm_init_traps(L);

	ret = sge_vm_load_main_task(L);
	if (ret < 0)
		goto bad1;

	sge_vm_state = SGE_VM_STATE_RUNNING;
	SDL_CondSignal(sge_vm_cond);

	while (sge_vm_state == SGE_VM_STATE_RUNNING) {
		sge_vm_schedule(L);
		SDL_UnlockMutex(sge_vm_mutex);
		uv_run(&sge_vm_loop, UV_RUN_ONCE);
		SDL_LockMutex(sge_vm_mutex);
	}

	uv_loop_close(&sge_vm_loop);

	return 0;

bad1:
	uv_loop_close(&sge_vm_loop);

bad0:
	sge_vm_state = SGE_VM_STATE_ERROR;
	SDL_CondSignal(sge_vm_cond);

	return 0;
}

static int sge_vm_tmain(void *p)
{
	lua_State *L;

	SDL_LockMutex(sge_vm_mutex);

	L = luaL_newstate();
	if (L == NULL) {
		sge_vm_state = SGE_VM_STATE_ERROR;
		SDL_CondSignal(sge_vm_cond);
		SDL_UnlockMutex(sge_vm_mutex);
		return 0;
	}

	lua_pushcfunction(L, sge_vm_pmain);
	lua_pcall(L, 1, 0, 0);
	lua_close(L);

	sge_vm_state = SGE_VM_STATE_NONE;
	SDL_CondSignal(sge_vm_cond);
	SDL_UnlockMutex(sge_vm_mutex);

	return 0;
}

void sge_vm_start(lua_State *L)
{
}

void sge_vm_stop(lua_State *L)
{
}

void sge_vm_task_add(lua_State *L, lua_State *L1)
{
	sge_vm_task_t *T = SGE_VM_TASK_FROM_LUA(L1);
	uv_timer_init(&sge_vm_loop, &T->sleep_timer);
	T->sleep_timer.data = T;
	cx_list_node_reset(&T->node);
	cx_list_add_tail(&sge_vm_task_ready_list, &T->node);	
}

void sge_vm_task_remove(lua_State *L, lua_State *L1)
{
	sge_vm_task_t *T = SGE_VM_TASK_FROM_LUA(L1);
	cx_list_node_unlink(&T->node);
}

void sge_vm_task_resume(lua_State *L, int n)
{
}

void sge_vm_task_yield(lua_State *L, int n)
{
	sge_vm_task_t *T = SGE_VM_TASK_FROM_LUA(L);
	cx_list_node_unlink(&T->node);
	cx_list_add_tail(&sge_vm_task_sleep_list, &T->node);
}

int sge_vm_init(void)
{
	int ret;

	sge_vm_mutex = SDL_CreateMutex();
	if (sge_vm_mutex == NULL)
		goto bad0;

	sge_vm_cond = SDL_CreateCond();
	if (sge_vm_cond == NULL)
		goto bad1;

	cx_list_reset(&sge_vm_task_ready_list);
	cx_list_reset(&sge_vm_task_sleep_list);

	sge_vm_trap.type = SGE_VM_TRAP_TYPE_NONE;
	sge_vm_trap.result = 0;
	sge_vm_trap.stack = NULL;

	sge_vm_state = SGE_VM_STATE_INITIALIZING;

	SDL_LockMutex(sge_vm_mutex);

	sge_vm_thread = SDL_CreateThread(sge_vm_tmain, "sge_vm", NULL);
	if (sge_vm_thread == NULL)
		goto bad2;

	while (sge_vm_state != SGE_VM_STATE_INITIALIZING)
		ret = SDL_CondWait(sge_vm_cond, sge_vm_mutex);

	if (sge_vm_state != SGE_VM_STATE_RUNNING) {
		SDL_WaitThread(sge_vm_thread, NULL);
		goto bad2;
	}

	return 0;

bad2:
	SDL_UnlockMutex(sge_vm_mutex);
	SDL_DestroyCond(sge_vm_cond);

bad1:
	SDL_DestroyMutex(sge_vm_mutex);

bad0:
	return -1;
}

void sge_vm_shutdown(void)
{
	uv_async_send(&sge_vm_quit);

	SDL_WaitThread(sge_vm_thread, NULL);

	SDL_DestroyCond(sge_vm_cond);
	SDL_DestroyMutex(sge_vm_mutex);
}

void sge_vm_update(float elapsed, const sge_vm_traps_t *traps)
{
	int ret;

	CX_ASSERT(traps != NULL);

	if (sge_vm_trap.type == SGE_VM_TRAP_TYPE_NONE)
		return;

	SDL_LockMutex(sge_vm_mutex);

	switch (sge_vm_trap.type) {
	case SGE_VM_TRAP_TYPE_GET_FPS:
		sge_vm_trap.result = sge_vm_trap_get_fps_be(sge_vm_trap.stack, traps);
		break;
	case SGE_VM_TRAP_TYPE_GET_FPS_MAX:
		sge_vm_trap.result = sge_vm_trap_get_fps_be(sge_vm_trap.stack, traps);
		break;
	case SGE_VM_TRAP_TYPE_SET_FPS_MAX:
		sge_vm_trap.result = sge_vm_trap_get_fps_be(sge_vm_trap.stack, traps);
		break;
	case SGE_VM_TRAP_TYPE_NONE:
		sge_vm_trap.result = sge_vm_trap_error(sge_vm_trap.stack, "invalid trap.");
		break;
	}

	SDL_CondSignal(sge_vm_cond);
	SDL_UnlockMutex(sge_vm_mutex);
}

