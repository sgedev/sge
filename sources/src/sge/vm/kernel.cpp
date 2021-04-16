//
//
#include <future>
#include <vector>

#include <sge/scope_guard.hpp>
#include <sge/vm/kernel.hpp>

SGE_VM_BEGIN

traps_type traps;

static bool running;
static SDL_Thread *thread_handle;
static uv_async_t trap_async;
static uv_async_t quit_async;

static void trap_cb(uv_async_t *ptr)
{

}

static void quit_cb(uv_async_t *ptr)
{
	running = false;
}

static void schedule(lua_State *L)
{
	SGE_ASSERT(L != nullptr);
}

static void init_exports(lua_State *L)
{
	lua_newtable(L);

	lua_setglobal(L, "sge");
}

static bool load_initrc(lua_State *L)
{
	SGE_ASSERT(L != nullptr);

	PHYSFS_File *fp = PHYSFS_openRead("/init.lua");
	if (fp == nullptr) {
		SGE_LOGE("failed to open '/init.lua'.");
		return false;
	}

	scope_guard fp_guard([=] { PHYSFS_close(fp); });

	PHYSFS_sint64 size = PHYSFS_fileLength(fp);
	if (size < 1) {
		SGE_LOGE("'/init.lua' file is too small.");
		return false;
	}

	std::vector<uint8_t> buf(size);
	PHYSFS_sint64 ret = PHYSFS_readBytes(fp, buf.data(), size);
	if (ret != size) {
		SGE_LOGE("failed to read '/init.rc'.");
		return false;
	}

	// TODO load to lua...

	return true;
}

static int thread_main(void *ptr)
{
	std::promise<bool> *init_result = (std::promise<bool> *)ptr;
	SGE_ASSERT(init_result != nullptr);

	int ret;

	ret = PHYSFS_init("sge");
	if (!ret) {
		SGE_LOGE("failed to init physfs.");
		init_result->set_value(false);
		return 0;
	}

	scope_guard physfs_guard([] { PHYSFS_deinit(); });

	lua_State *L = luaL_newstate();
	if (L == nullptr) {
		SGE_LOGE("failed to create lua state.");
		init_result->set_value(false);
		return 0;
	}

	scope_guard lua_guard([&] { lua_close(L); });
	luaL_openlibs(L);
	init_exports(L);

	uv_loop_t loop;
	ret = uv_loop_init(&loop);
	if (ret < 0) {
		SGE_LOGE("failed to init vm loop.");
		init_result->set_value(false);
		return 0;
	}

	// TODO scope_guard uv_guard([&loop] { uv_close(&loop); });

	if (!load_initrc(L)) {
		SGE_LOGE("failed to load initrc.");
		return 0;
	}

	uv_async_init(&loop, &quit_async, &quit_cb);

	running = true;
	init_result->set_value(true);

	while (running) {
		schedule(L);
		uv_run(&loop, UV_RUN_ONCE);
	}

	return 0;
}

bool start(uv_loop_t *loop)
{
	SGE_ASSERT(loop != nullptr);

	uv_async_init(loop, &trap_async, trap_cb);

	std::promise<bool> init_result;
	auto init_future = init_result.get_future();

	thread_handle = SDL_CreateThread(&thread_main, "sge-vm", &init_result);
	if (thread_handle == nullptr) {
		SGE_LOGE("failed to create vm thread.");
		return false;
	}

	return init_future.get();
}

void stop(void)
{
	SGE_ASSERT(thread_handle != nullptr);
	uv_async_send(&quit_async);
	SDL_WaitThread(thread_handle, nullptr);
}

bool is_inside(void)
{
	if (thread_handle != nullptr)
		return (SDL_GetThreadID(thread_handle) == SDL_ThreadID());
	return false;
}

SGE_VM_END
