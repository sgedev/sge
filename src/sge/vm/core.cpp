//
//
#include <string>
#include <thread>
#include <mutex>
#include <condition_variable>

#include <lua.hpp>
#include <physfs.h>

#include <sge/vm.hpp>

#define SGE_VM_OUTPUT_NUM 3

SGE_VM_BEGIN

static const char initrc_path[] = "/init.lua";

typedef enum {
	STATE_NOT_START = 0,
	STATE_RUNNING,
	STATE_EXITING,
	STATE_ERROR
} state_t;

typedef sge_vm_task_t task_t;

static std::mutex mutex;
static std::condition_variable_any cond;
static std::thread thread;
static std::string rootfs_path;
static state_t state;
static uv_async_t quit_async;
static cx_list_t task_list;
static int frame_count;
static int frame_per_second;
static uv_async_t output_async;
static output_func_t output_func;
static output outputs[SGE_VM_OUTPUT_NUM];
static int output_rslot;
static int output_wslot;

static SGE_INLINE task_t *task_from_lua(lua_State *L)
{
	return (task_t *)lua_getextraspace(L);
}

static SGE_INLINE lua_State *task_to_lua(task_t *task)
{
	return (lua_State *)CX_PMOVB(task, LUA_EXTRASPACE);
}

static SGE_INLINE void task_added(lua_State *L, lua_State *L1)
{
	task_t *main = task_from_lua(L);
	task_t *task = task_from_lua(L1);

	uv_timer_init(main->sleep_timer.loop, &task->sleep_timer);
	task->sleep_timer.data = task;
	cx_list_node_reset(&task->node);
	cx_list_add_tail(&task_list, &task->node);
}

static SGE_INLINE void task_removed(lua_State *L, lua_State *L1)
{
	task_t *task = task_from_lua(L1);

	uv_timer_stop(&task->sleep_timer);
	cx_list_node_unlink(&task->node);
}

static SGE_INLINE void task_resume(lua_State *L, int n)
{
}

static SGE_INLINE void task_yield(lua_State *L, int n)
{
}

static SGE_INLINE void set_state(state_t st)
{
	if (state != st) {
		state = st;
		cond.notify_one();
	}
}

static int trap_task(lua_State *L)
{
	lua_State *L1 = lua_newthread(L);

	return 1;
}

static void trap_sleep_done(uv_timer_t *p)
{
	std::lock_guard locker(mutex);

	task_t *task = (task_t *)(p->data);
	cx_list_node_unlink(&task->node);
	cx_list_add_tail(&task_list, &task->node);
}

static int trap_sleep(lua_State *L)
{
	int ms = (int)luaL_checkinteger(L, 1);
	task_t *task = task_from_lua(L);

	if (ms > 0)
		uv_timer_start(&task->sleep_timer, trap_sleep_done, ms, 0);
	else
		cx_list_add_tail(&task_list, &task->node);

	return lua_yield(L, 0);
}

static int trap_mount(lua_State *L)
{
	const char *src = luaL_checkstring(L, 1);
	const char *dst = luaL_checkstring(L, 2);

	lua_pushboolean(L, PHYSFS_mount(src, dst, 0));

	return 1;
}

static int trap_unmount(lua_State *L)
{
	const char *dir = luaL_checkstring(L, 1);

	lua_pushboolean(L, PHYSFS_unmount(dir));

	return 1;
}

static void init_exports(lua_State *L)
{
	lua_newtable(L);

	gui::init_exports(L);
	scene::init_exports(L);

	lua_pushcfunction(L, &trap_task);
	lua_setfield(L, -2, "Task");

	lua_pushcfunction(L, &trap_sleep);
	lua_setfield(L, -2, "sleep");

	lua_pushcfunction(L, &trap_mount);
	lua_setfield(L, -2, "mount");

	lua_pushcfunction(L, &trap_unmount);
	lua_setfield(L, -2, "unmount");

	lua_setglobal(L, "sge");
}

static bool init_rootfs(void)
{
	PHYSFS_Version ver;
	PHYSFS_getLinkedVersion(&ver);
	SGE_LOGI("PhysicsFS: %d.%d.%d", ver.major, ver.minor, ver.patch);

	int ret = PHYSFS_init("sge");
	if (!ret) {
		SGE_LOGE("Failed to init PhysicsFS.");
		goto bad0;
	}

	SGE_LOGI("Mount rootfs to '%s'...", rootfs_path.c_str());

	ret = PHYSFS_setWriteDir(rootfs_path.c_str());
	if (!ret) {
		SGE_LOGE("Failed to setup PhyFS write dir to '%s'.", rootfs_path.c_str());
		goto bad1;
	}

	ret = PHYSFS_mount(rootfs_path.c_str(), "/", 0);
	if (!ret) {
		SGE_LOGE("Failed to mount rootfs to '%s'.", rootfs_path.c_str());
		goto bad1;
	}

	return true;

bad1:
	PHYSFS_deinit();

bad0:
	return false;
}

static bool load_initrc(lua_State *L)
{
	lua_State *T;
	PHYSFS_File *fp;
	PHYSFS_uint64 size;
	char *rc;
	int ret;

    SGE_LOGI("Loading '%s'...", initrc_path);

	T = lua_newthread(L);
	if (T == NULL) {
		SGE_LOGE("Failed to create init task.");
		goto bad0;
	}

    fp = PHYSFS_openRead(initrc_path);
	if (fp == NULL) {
        SGE_LOGE("Failed to open initrc '%s'.", initrc_path);
		goto bad1;
	}

	size = PHYSFS_fileLength(fp);
	if (size < 1) {
        SGE_LOGE("Empty initrc '%s'.", initrc_path);
		goto bad2;
	}

    rc = (char *)malloc(size + 4);
	if (rc == NULL) {
        SGE_LOGE("No memory for initrc '%s'.", initrc_path);
		goto bad2;
	}

	memset(rc, 0, size + 4);
	PHYSFS_readBytes(fp, rc, size);
	PHYSFS_close(fp);

	ret = luaL_loadstring(T, rc);
	free(rc);
	if (ret != LUA_OK) {
        SGE_LOGE("Invalid initrc '%s'.", initrc_path);
		goto bad1;
	}

	return true;

bad2:
	PHYSFS_close(fp);

bad1:
	lua_pop(L, 1);

bad0:
	return false;
}

static void frame(uv_timer_t *p)
{
	static int64_t last = uv_now(p->loop);
	int64_t curr = uv_now(p->loop);
	int64_t pass = curr - last;
	if (pass < 0)
		return;

	float elapsed = float(pass) / 1000.0f;

	scene::update(elapsed);
	gui::update(elapsed);

	// build view
	// trap_render(view);

	last = curr;
	frame_count += 1;

	uv_update_time(p->loop);
}

static void count(uv_timer_t *p)
{
	frame_per_second = frame_count;
	frame_count = 0;
}

static void quit(uv_async_t *p)
{
    state = STATE_EXITING;
}

static void schedule(lua_State *L)
{
	cx_list_node_t *node;
	task_t *task;
	lua_State *T;
	int ret;
	int nresults;

	while (!cx_list_empty(&task_list)) {
		node = cx_list_del_head(&task_list);
		task = CX_MEMBEROF(node, task_t, node);
		T = task_to_lua(task);
		ret = lua_resume(T, L, 0, &nresults);
		/* TODO ret */
	}
}

static int pmain(lua_State *L)
{
    task_t *task;
    uv_loop_t loop;
    uv_timer_t frame_timer;
    uv_timer_t count_timer;

	luaL_openlibs(L);
	init_exports(L);

	if (uv_loop_init(&loop) < 0) {
		SGE_LOGE("Failed to init uv loop.");
		set_state(STATE_ERROR);
		goto end0;
	}

    task = task_from_lua(L);
	uv_timer_init(&loop, &task->sleep_timer);

	if (!init_rootfs()) {
		set_state(STATE_ERROR);
		goto end1;
	}

	if (!load_initrc(L)) {
		set_state(STATE_ERROR);
		goto end2;
	}

	if (!scene::init()) {
		set_state(STATE_ERROR);
		goto end2;
	}

	if (!gui::init()) {
		set_state(STATE_ERROR);
		goto end3;
	}

    uv_async_init(&loop, &quit_async, &quit);

    uv_timer_init(&loop, &frame_timer);
    uv_timer_init(&loop, &count_timer);

	uv_timer_start(&frame_timer, &frame, 0, 16);
    uv_timer_start(&count_timer, &count, 1000, 1000);

	set_state(STATE_RUNNING);
	while (state == STATE_RUNNING) {
		schedule(L);
		uv_run(&loop, UV_RUN_ONCE);
	}

    uv_timer_stop(&count_timer);
	uv_timer_stop(&frame_timer);

	gui::shutdown();

end3:
	scene::shutdown();

end2:
	PHYSFS_deinit();

end1:
	uv_loop_close(&loop);

end0:
	return 0;
}

static void tmain(void)
{
	std::lock_guard locker(mutex);

	cx_list_reset(&task_list);

	lua_State *L = luaL_newstate();
	if (L == nullptr) {
		set_state(STATE_ERROR);
		return;
	}

	lua_pushcfunction(L, &pmain);
    lua_pcall(L, 0, 0, 0);
	lua_close(L);
}

static void output_handler(uv_async_t *p)
{

}

bool start(uv_loop_t *loop, output_func_t func)
{
	SGE_ASSERT(loop != nullptr);
    SGE_ASSERT(func != nullptr);

	std::lock_guard locker(mutex);

    uv_async_init(loop, &output_async, &output_handler);
    output_func = func;

	state = STATE_NOT_START;
	thread = std::thread(&tmain);

	for (;;) {
		cond.wait(mutex);
		if (state == STATE_NOT_START)
			continue;
		if (state == STATE_ERROR)
			return false;
		if (state == STATE_RUNNING)
			break;
	}

	return true;
}

void stop(void)
{
	uv_async_send(&quit_async);
	thread.join();
}

void post_event(const SDL_Event &evt)
{

}

SGE_VM_END

extern "C" void sge_vm_task_added(lua_State *L, lua_State *L1)
{
	sge::vm::task_added(L, L1);
}

extern "C" void sge_vm_task_removed(lua_State *L, lua_State *L1)
{
	sge::vm::task_removed(L, L1);
}

extern "C" void sge_vm_task_resume(lua_State *L, int n)
{
	sge::vm::task_resume(L, n);
}

extern "C" void sge_vm_task_yield(lua_State *L, int n)
{
	sge::vm::task_yield(L, n);
}
