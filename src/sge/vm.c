/*
 *
 */
#include <stdarg.h>

#include <sge/vm.h>

#define SGE_VM_MAIN_TASK_FILENAME "/main.lua"
#define SGE_VM_OBJECT_TYPE "sge-object"

typedef enum {
	SGE_VM_TRAP_TYPE_NONE = 0,
	SGE_VM_TRAP_TYPE_EXIT,
	SGE_VM_TRAP_TYPE_GET_FPS,
	SGE_VM_TRAP_TYPE_SET_FPS_MAX,
	SGE_VM_TRAP_TYPE_GET_FPS_MAX,
	SGE_VM_TRAP_TYPE_TOGGLE_SHOW_FPS,
	SGE_VM_TRAP_TYPE_CREATE_OBJECT
} sge_vm_trap_type_t;

typedef struct {
	sge_vm_trap_type_t type;
	lua_State *stack;
	int result;
} sge_vm_trap_t;

typedef struct {
	lua_State *L;
	int ref;
	sge_object_t *obj;
} sge_vm_luaobj_t;

static SDL_Thread *sge_vm_thread;
static sge_vm_state_t sge_vm_state;
static sge_vm_loading_state_t sge_vm_loading_state;
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
	lua_rawseti(L, -2, 0);

	lua_pushinteger(L, SGE_VERSION_MINOR);
	lua_rawseti(L, -2, 1);

	lua_pushinteger(L, SGE_VERSION_PATCH);
	lua_rawseti(L, -2, 2);

	return 1;
}

static int sge_vm_trap_task(lua_State *L)
{
	int type;
	lua_State *L1;
	
	type = lua_type(L, 1);
	switch (type) {
	case LUA_TFUNCTION:
		L1 = lua_newthread(L);
		if (L1 != NULL) {
			lua_pushvalue(L, 1);
			lua_xmove(L, L1, 1);
		} else
			lua_pushnil(L);
		break;
	case LUA_TSTRING:
		L1 = lua_newthread(L);
		if (L1 != NULL)
			luaL_loadfile(L1, lua_tostring(L, 1));
		else
			lua_pushnil(L1);
		break;
	default:
		luaL_error(L, "unknown type.");
		break;
	}

	return 1;
}

static void sge_vm_trap_sleep_done(uv_timer_t *p)
{
	sge_vm_task_t *task;

	CX_ASSERT(p != NULL);
	CX_ASSERT(p->DATA != NULL);

	task = p->data;

	cx_list_node_unlink(&task->node);
	cx_list_add_tail(&sge_vm_task_ready_list, &task->node);

	uv_timer_stop(p);
}

static int sge_vm_trap_sleep(lua_State *L)
{
	int ms = (int)luaL_checkinteger(L, 1);
	if (ms > 0) {
		sge_vm_task_t *T = SGE_VM_TASK_FROM_LUA(L);
		uv_timer_start(&T->sleep_timer, sge_vm_trap_sleep_done, ms, 0);
		return lua_yield(L, 0);
	}

	return 0;
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
	CX_ASSERT(traps != NULL);

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
	CX_ASSERT(traps != NULL);

	if (traps->get_fps_max == NULL)
		return sge_vm_trap_error(L, "not impl.");

	lua_pushinteger(L, traps->get_fps_max());

	return 1;
}

static int sge_vm_trap_set_fps_max(lua_State *L)
{
	int v = (int)luaL_checkinteger(L, 1);
	luaL_argcheck(L, v > 0, 1, "fps max must great than 0.");

	return sge_vm_do_trap(L, SGE_VM_TRAP_TYPE_SET_FPS_MAX);
}

static int sge_vm_trap_set_fps_max_be(lua_State *L, const sge_vm_traps_t *traps)
{
	CX_ASSERT(traps != NULL);

	if (traps->set_fps_max == NULL)
		return sge_vm_trap_error(L, "not impl.");

	traps->set_fps_max((int)lua_tointeger(L, 1));

	return 0;
}

static int sge_vm_trap_toggle_show_fps(lua_State *L)
{
	return sge_vm_do_trap(L, SGE_VM_TRAP_TYPE_TOGGLE_SHOW_FPS);
}

static int sge_vm_trap_toggle_show_fps_be(lua_State *L, const sge_vm_traps_t *traps)
{
	if (traps->toggle_show_fps == NULL)
		return sge_vm_trap_error(L, "not impl.");

	traps->toggle_show_fps();
	return 0;
}

static int sge_vm_trap_object(lua_State *L)
{
	int ret;
	sge_vm_object_t *obj;
	sge_vm_luaobj_t *luaobj;
	sge_vm_luaobj_t *parent = NULL;
	const char *name = luaL_checkstring(L, 1);

	sge_vm_do_trap(L, SGE_VM_TRAP_TYPE_CREATE_OBJECT);
	//void *obj = lua_toudata(L, -1);
	lua_pop(L, 1);
	if (obj != NULL) {
		object = lua_newuserdata(L, sizeof(sge_vm_luaobj_t));
		object->obj = obj;
		object->L = L;
		/* TODO ref? */
	} else
		lua_pushnil(L);

	return 1;
}

static int sge_vm_trap_create_object_be(lua_State *L, const sge_vm_traps_t *traps)
{
	if (traps->create_object == NULL)
		return sge_vm_trap_error(L, "not impl.");

	traps->create_object(lua_touserdata(L, 1));

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

	lua_pushcfunction(L, &sge_vm_trap_toggle_show_fps);
	lua_setfield(L, -2, "toggle_show_fps");

	lua_pushcfunction(L, &sge_vm_trap_object);
	lua_setfield(L, -2, "object");

	lua_setglobal(L, "sge");
}

static int sge_vm_load_main_task(lua_State *L)
{
	lua_State *main_task;
	int ret;

	SGE_LOGI("Loading " SGE_VM_MAIN_TASK_FILENAME "...");

	main_task = lua_newthread(L);
	if (main_task == NULL) {
		SGE_LOGE("Failed to create main task.");
		return -1;
	}

	ret = luaL_loadfile(main_task, SGE_VM_MAIN_TASK_FILENAME);
	if (ret != LUA_OK) {
		SGE_LOGE("Failed to load main task.");
		return -1;
	}

	return 0;
}

static void sge_vm_quit_cb(uv_async_t *handle)
{
	sge_vm_state = SGE_VM_STATE_EXITING;
	uv_stop(&sge_vm_loop);
}

static void sge_vm_schedule(lua_State *L)
{
	cx_list_node_t *node;
	sge_vm_task_t *task;
	lua_State *L1;
	int ret;

	while (!cx_list_empty(&sge_vm_task_ready_list)) {
		node = cx_list_del_head(&sge_vm_task_ready_list);
		task = CX_MEMBEROF(node, sge_vm_task_t, node);
		L1 = SGE_VM_TASK_TO_LUA(task);
		ret = lua_resume(L1, L, 0);
		if (ret != LUA_OK && ret != LUA_YIELD)
			SGE_LOGE("LUAERR: %s", lua_tostring(L1, -1));
	}
}

static int sge_vm_pmain(lua_State *L)
{
	int ret;

	SGE_LOGI("VM start.");

	ret = uv_loop_init(&sge_vm_loop);
	if (ret < 0)
		goto bad0;

	uv_async_init(&sge_vm_loop, &sge_vm_quit, sge_vm_quit_cb);

	luaL_openlibs(L);
	sge_vm_init_traps(L);

	luaL_newmetatable(L, SGE_VM_OBJECT_TYPE);

	ret = sge_vm_load_main_task(L);
	if (ret < 0)
		goto bad1;

	sge_vm_state = SGE_VM_STATE_RUNNING;
	SDL_CondSignal(sge_vm_cond);

	while (sge_vm_state == SGE_VM_STATE_RUNNING) {
		if (cx_list_empty(&sge_vm_task_ready_list) &&
			cx_list_empty(&sge_vm_task_sleep_list))
			break;
		sge_vm_schedule(L);
		uv_update_time(&sge_vm_loop);
		SDL_UnlockMutex(sge_vm_mutex);
		uv_run(&sge_vm_loop, UV_RUN_ONCE);
		SDL_LockMutex(sge_vm_mutex);
	}

	uv_loop_close(&sge_vm_loop);

	sge_vm_trap.type = SGE_VM_TRAP_TYPE_EXIT;
	SGE_LOGI("VM exit.");

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
	lua_pcall(L, 0, 0, 0);
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

LUA_FILE *sge_vm_io_fopen(const char *filename, const char *mode)
{
	char *p = strchr(mode, 'r');
	if (p == NULL)
		return NULL;
	return PHYSFS_openRead(filename);
}

LUA_FILE *sge_vm_io_freopen(const char *filename, const char *mode, LUA_FILE **fp)
{
	PHYSFS_close(*fp);
	*fp = sge_vm_io_fopen(filename, mode);
	return *fp;
}

int sge_vm_io_fclose(LUA_FILE *fp)
{
	return PHYSFS_close(fp);
}

int sge_vm_io_feof(LUA_FILE *fp)
{
	return PHYSFS_eof(fp);
}

size_t sge_vm_io_fread(void *buf, int n, int size, LUA_FILE *fp)
{
	return PHYSFS_readBytes(fp, buf, n * size);
}

int sge_vm_io_getc(LUA_FILE *fp)
{
	char c;

	if (PHYSFS_eof(fp))
		return EOF;

	PHYSFS_readBytes(fp, &c, 1);
	return c;
}

int sge_vm_io_ferror(LUA_FILE *fp)
{
	return 0;
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

	sge_vm_loading_state.message = NULL;
	sge_vm_loading_state.percentage = 0;

	SDL_LockMutex(sge_vm_mutex);

	sge_vm_thread = SDL_CreateThread(sge_vm_tmain, "sge_vm", NULL);
	if (sge_vm_thread == NULL)
		goto bad2;

	while (sge_vm_state == SGE_VM_STATE_INITIALIZING)
		ret = SDL_CondWait(sge_vm_cond, sge_vm_mutex);

	if (sge_vm_state != SGE_VM_STATE_RUNNING) {
		SDL_WaitThread(sge_vm_thread, NULL);
		goto bad2;
	}

	SDL_UnlockMutex(sge_vm_mutex);

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
#if 0 /* TODO */
	SDL_LockMutex(sge_vm_mutex);
	if (sge_vm_trap.type != SGE_VM_TRAP_TYPE_NONE) {
		sge_vm_trap.type = SGE_VM_TRAP_TYPE_NONE;
		SDL_CondSignal(sge_vm_cond);
	}
	SDL_UnlockMutex(sge_vm_mutex);
#endif

	uv_async_send(&sge_vm_quit);

	SDL_WaitThread(sge_vm_thread, NULL);

	SDL_DestroyCond(sge_vm_cond);
	SDL_DestroyMutex(sge_vm_mutex);
}

void sge_vm_update(float elapsed, const sge_vm_traps_t *traps)
{
	CX_ASSERT(traps != NULL);

	if (sge_vm_trap.type == SGE_VM_TRAP_TYPE_NONE)
		return;

	SDL_LockMutex(sge_vm_mutex);

	switch (sge_vm_trap.type) {
	case SGE_VM_TRAP_TYPE_EXIT:
		traps->exit();
		break;
	case SGE_VM_TRAP_TYPE_GET_FPS:
		sge_vm_trap.result = sge_vm_trap_get_fps_be(sge_vm_trap.stack, traps);
		break;
	case SGE_VM_TRAP_TYPE_GET_FPS_MAX:
		sge_vm_trap.result = sge_vm_trap_get_fps_be(sge_vm_trap.stack, traps);
		break;
	case SGE_VM_TRAP_TYPE_SET_FPS_MAX:
		sge_vm_trap.result = sge_vm_trap_get_fps_be(sge_vm_trap.stack, traps);
		break;
	case SGE_VM_TRAP_TYPE_TOGGLE_SHOW_FPS:
		sge_vm_trap.result = sge_vm_trap_toggle_show_fps_be(sge_vm_trap.stack, traps);
		break;
	case SGE_VM_TRAP_TYPE_NONE:
	default:
		sge_vm_trap.result = sge_vm_trap_error(sge_vm_trap.stack, "invalid trap.");
		break;
	}

	sge_vm_trap.type = SGE_VM_TRAP_TYPE_NONE;
	SDL_CondSignal(sge_vm_cond);

	SDL_UnlockMutex(sge_vm_mutex);
}

sge_vm_state_t sge_vm_get_state(void)
{
	return sge_vm_state;
}

const sge_vm_loading_state_t *sge_vm_get_loading_state(void)
{
	return &sge_vm_loading_state;
}
