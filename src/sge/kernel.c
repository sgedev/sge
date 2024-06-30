/*
 *
 */
#include <sge/kernel.h>

enum {
	SGE_KERNEL_STATE_IDLE = 0,
	SGE_KERNEL_STATE_INITIALIZED,
	SGE_KERNEL_STATE_STARTING,
	SGE_KERNEL_STATE_RUNNING,
	SGE_KERNEL_STATE_STOPPING
};

static uv_mutex_t sge_kernel_mutex_;
static uv_cond_t sge_kernel_cond_;
static uv_loop_t* sge_kernel_loop_;
static uv_async_t sge_kernel_async_trap_;
static int sge_kernel_state_;
static char* sge_kernel_rootfs_;
static char* sge_kernel_initrc_;
static uv_thread_t sge_kernel_thread_;
static cx_list_t sge_kernel_task_list_; 
static uv_async_t sge_kernel_async_stop_;
static uv_prepare_t sge_kernel_scheduler_;
static uv_timer_t sge_kernel_be_frame_timer_;
static unsigned long sge_kernel_frame_count_;
static unsigned long sge_kernel_frame_count_per_second_;

static void sge_kernel_be_frame(uv_timer_t* p) {

}

static void sge_kernel_schedule(uv_prepare_t* p) {

}

static bool sge_kernel_init_apis(lua_State* L) {
	return true;
}

static bool sge_kernel_load_initrc(lua_State* L) {
	return true;
}

static int sge_kernel_be_pmain(lua_State* L) {
	uv_loop_t loop;
	int ret = uv_loop_init(&loop);
	if (ret < 0) {
		goto end0;
	}

	ret = PHYSFS_init("sge");
	if (!ret) {
		goto end1;
	}

	ret = PHYSFS_mount(sge_kernel_rootfs_, "/", 0);
	if (!ret) {
		goto end2;
	}

	const char* user_path = SDL_GetPrefPath(NULL, "sge");
	if (user_path == NULL) {
		goto end2;
	}

	ret = PHYSFS_setWriteDir(user_path);
	if (!ret) {
		goto end2;
	}

	ret = uv_prepare_init(&loop, &sge_kernel_scheduler_);
	if (ret < 0) {
		goto end2;
	}
	sge_kernel_scheduler_.data = L;
	ret = uv_prepare_start(&sge_kernel_scheduler_, &sge_kernel_schedule);
	if (ret < 0) {
		goto end3;
	}

	ret = uv_timer_init(&loop, &sge_kernel_be_frame_timer_);
	if (ret < 0) {
		goto end4;
	}
	ret = uv_timer_start(&sge_kernel_be_frame_timer_, &sge_kernel_be_frame, 0, 16);
	if (ret < 0) {
		goto end5;
	}

	luaL_openlibs(L);
	sge_kernel_init_apis(L);
	sge_kernel_load_initrc(L);

	sge_kernel_state_ = SGE_KERNEL_STATE_RUNNING;
	uv_cond_signal(&sge_kernel_cond_);

	uv_run(&loop, UV_RUN_DEFAULT);

end5:
	uv_close((uv_handle_t*)&sge_kernel_be_frame_timer_, NULL);

end4:
	uv_prepare_stop(&sge_kernel_scheduler_);

end3:
	uv_close((uv_handle_t*)&sge_kernel_scheduler_, NULL);

end2:
	PHYSFS_deinit();

end1:
	uv_loop_close(&loop);

end0:
	return 0;
}

static int sge_kernel_be_main(void* p) {
	uv_mutex_lock(&sge_kernel_mutex_);
	int ret = 0;
	lua_State* L = luaL_newstate();
	if (L != NULL) {
		lua_gc(L, LUA_GCSTOP);
		lua_pushcfunction(L, &sge_kernel_be_pmain);
		int ret = lua_pcall(L, 0, 0, 0);
		// TODO
		lua_close(L);
	}
	uv_mutex_unlock(&sge_kernel_mutex_);
	return 0;
}

static void sge_kernel_handle_async_trap(uv_async_t* p) {

}

int sge_kernel_init(uv_loop_t* loop, const char* rootfs, const char* initrc) {
	CX_ASSERT(sge_kernel_state_ == SGE_KERNEL_STATE_IDLE);
	CX_ASSERT(sge_kernel_loop_ == NULL);
	CX_ASSERT(loop != NULL);
	CX_ASSERT(rootfs != NULL);
	CX_ASSERT(initrc != NULL);

	int ret = uv_async_init(loop, &sge_kernel_async_trap_, &sge_kernel_handle_async_trap);
	if (ret < 0) {
		goto bad0;
	}

	ret = uv_mutex_init(&sge_kernel_mutex_);
	if (ret < 0) {
		goto bad1;
	}

	ret = uv_cond_init(&sge_kernel_cond_);
	if (ret < 0) {
		goto bad2;
	}

	sge_kernel_rootfs_ = strdup(rootfs);
	if (sge_kernel_rootfs_ == NULL) {
		goto bad3;
	}

	sge_kernel_initrc_ = strdup(initrc);
	if (sge_kernel_rootfs_ == NULL) {
		goto bad4;
	}

	sge_kernel_loop_ = loop;
	sge_kernel_state_ = SGE_KERNEL_STATE_INITIALIZED;

	return 0;

bad4:
	free(sge_kernel_rootfs_);

bad3:
	uv_cond_destroy(&sge_kernel_cond_);

bad2:
	uv_mutex_destroy(&sge_kernel_mutex_);

bad1:
	uv_close((uv_handle_t*)&sge_kernel_async_trap_, NULL);

bad0:
	return -1;
}

void sge_kernel_shutdown() {
	CX_ASSERT(sge_kernel_state_ == SGE_KERNEL_STATE_INITIALIZED);
	free(sge_kernel_rootfs_);
	free(sge_kernel_initrc_);
	uv_cond_destroy(&sge_kernel_cond_);
	uv_mutex_destroy(&sge_kernel_mutex_);
	uv_close((uv_handle_t*)&sge_kernel_async_trap_, NULL);
	PHYSFS_setWriteDir(NULL);
	PHYSFS_unmount("/");
	PHYSFS_deinit();
	sge_kernel_state_ = SGE_KERNEL_STATE_IDLE;
}

int sge_kernel_start(sge_kernel_start_cb cb, void* data) {
	CX_ASSERT(sge_kernel_state_ == SGE_KERNEL_STATE_INITIALIZED);
	CX_ASSERT(sge_kernel_mutex_ != NULL);
	CX_ASSERT(sge_kernel_cond_ != NULL);
	CX_ASSERT(sge_kernel_thread_ == NULL);

	uv_mutex_lock(&sge_kernel_mutex_);

	sge_kernel_state_ = SGE_KERNEL_STATE_STARTING;

	int ret = uv_thread_create(&sge_kernel_thread_, &sge_kernel_be_main, NULL);
	if (ret < 0) {
		goto bad0;
	}

	ret = uv_cond_timedwait(&sge_kernel_cond_, &sge_kernel_mutex_, 5000);
	if (ret != 0) {
		goto bad1;
	}

	int init_result = (sge_kernel_state_ == SGE_KERNEL_STATE_RUNNING) ? 0 : -1;

	uv_mutex_unlock(&sge_kernel_mutex_);

	return init_result;

bad1:
	uv_thread_join(sge_kernel_thread_);

bad0:
	uv_mutex_unlock(&sge_kernel_mutex_);

	return -1;
}

void sge_kernel_stop() {
	CX_ASSERT(sge_kernel_mutex_ != NULL);
	CX_ASSERT(sge_kernel_cond_ != NULL);
	CX_ASSERT(sge_kernel_thread_ == NULL);

	sge_kernel_state_ = SGE_KERNEL_STATE_STOPPING;
	uv_async_send(&sge_kernel_async_stop_);
	SDL_WaitThread(sge_kernel_thread_, NULL);
}

void sge_kernel_feed_event(const SDL_Event* evt) {

}

uv_loop_t* sge_kernel_loop() {
	return sge_kernel_loop_;
}

void sge_kernel_open(lua_State* L) {
	cx_list_reset(&sge_kernel_task_list_);
}

void sge_kernel_close(lua_State* L) {

}

void sge_kernel_thread(lua_State* L, lua_State* T) {

}

void sge_kernel_free(lua_State* L, lua_State* T) {

}

void sge_kernel_resume(lua_State* T, int n) {

}

void sge_kernel_yeild(lua_State* T, int n) {

}
