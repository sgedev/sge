//
//
#include <vector>
#include <filesystem>

#include <physfs.h>

#include <sge/list.h>
#include <sge/scopeguard.hpp>
#include <sge/vm/kernel.hpp>

SGE_VM_BEGIN

Kernel::Kernel(Context& context)
    : context_(context) {
    int ret = uv_loop_init(&loop_);
    if (ret < 0) {
        throw std::runtime_error("Failed to create uv loop.");
    }
    auto loop_guard = scopeGuard([this] {
        uv_loop_close(&loop_);
    });
    loop_.data = this;

    ret = uv_async_init(&loop_, &stop_async_, [](uv_async_t* p) {
        uv_stop(p->loop);
    });
    if (ret < 0) {
        throw std::runtime_error("Failed to create stopper.");
    }
    auto stopper_guard = scopeGuard([this] {
        uv_close(reinterpret_cast<uv_handle_t*>(&stop_async_), nullptr);
    });
    stop_async_.data = this;

    ret = uv_prepare_init(&loop_, &schedule_prepare_);
    if (ret < 0) {
        throw std::runtime_error("Failed to create scheduler.");
    }
    auto scheduler_guard = scopeGuard([this] {
        uv_close(reinterpret_cast<uv_handle_t*>(&schedule_prepare_), nullptr);
    });

    ret = uv_timer_init(&loop_, &frame_timer_);
    if (ret < 0) {
        throw std::runtime_error("Failed to create frame timer.");
    }
    auto frame_timer_guard = scopeGuard([this] {
        uv_close(reinterpret_cast<uv_handle_t*>(&frame_timer_), nullptr);
    });
    frame_timer_.data = this;

    sge_List_reset(&task_list_);

    loop_guard.dismiss();
    stopper_guard.dismiss();
    scheduler_guard.dismiss();
    frame_timer_guard.dismiss();
}

Kernel::~Kernel() {
    uv_close(reinterpret_cast<uv_handle_t*>(&schedule_prepare_), nullptr);
    uv_close(reinterpret_cast<uv_handle_t*>(&stop_async_), nullptr);
    uv_close(reinterpret_cast<uv_handle_t*>(&frame_timer_), nullptr);
    uv_loop_close(&loop_);
}

void Kernel::run() {
    auto L = luaL_newstate();
    if (L == nullptr) {
        throw std::runtime_error("Failed to creat lua state.");
    }
    auto state_guard = scopeGuard([L] {
        lua_close(L);
    });

    auto main_task = taskFromState(L);
    sge_ListNode_reset(&main_task->node);
    sge_List_reset(&main_task->wait_list);
    main_task->data = this;

    lua_gc(L, LUA_GCSTOP);
    lua_pushcfunction(L, [](lua_State* L) {
        lua_gc(L, LUA_GCRESTART);
        lua_gc(L, LUA_GCGEN);
        fromState(L)->pmain(L);
        return 0;
    });
    int ret = lua_pcall(L, 1, 0, 0);
    // TODO
}

void Kernel::stop() {
    uv_async_send(&stop_async_);
}

void Kernel::shutdownHook(lua_State* L) {
}

void Kernel::taskAddedHook(lua_State* L, lua_State* T) {
    SGE_ASSERT(fromState(L) == this);
    auto main = taskFromState(L);
    auto task = taskFromState(T);

	memset(task, 0, sizeof(Task));
    task->data = main->data;

	int ret = uv_timer_init(&loop_, &task->sleep_timer);
    if (ret < 0) {
        luaL_error(L, "Failed to create task sleep timer.");
        return;
	}

    sge_List_reset(&task->wait_list);
    sge_ListNode_reset(&task->node);
    sge_List_append(&task_list_, &task->node);
}

void Kernel::taskRemovedHook(lua_State* L, lua_State* T) {
    SGE_ASSERT(fromState(L) == this);
    auto task = taskFromState(T);
    sge_ListNode_unlink(&task->node);
    // TODO
}

void Kernel::taskResumeHook(lua_State* T, int n) {
    SGE_ASSERT(fromState(T) == this);
}

void Kernel::taskYieldHook(lua_State* T, int n) {
    SGE_ASSERT(fromState(T) == this);
}

void Kernel::registerEnv(const char* name, std::function<int(lua_State*)> func) {
	env_map_[name] = std::move(func);
}

void Kernel::unregisterEnv(const char* name) {
	auto it = env_map_.find(name);
    if (it != env_map_.end()) {
        env_map_.erase(it);
	}
}

void Kernel::initSyscalls(lua_State* L) {
    //luaL_openlibs(L);

    luaopen_base(L);
    //luaopen_package(L);
    luaopen_coroutine(L);
    luaopen_debug(L);
    //luaopen_io(L);
    luaopen_math(L);
    luaopen_string(L);
    luaopen_table(L);
    luaopen_utf8(L);

    lua_pushcfunction(L, [](lua_State* L) { return fromState(L)->sysSleep(L); });
	lua_setglobal(L, "sleep");

	lua_pushcfunction(L, [](lua_State* L) { return fromState(L)->sysStart(L); });
	lua_setglobal(L, "start");

	lua_pushcfunction(L, [](lua_State* L) { return fromState(L)->sysWait(L); });
	lua_setglobal(L, "wait");

    context_.initSyscalls(L);
}

void Kernel::frame(Clock::duration elapsed) noexcept {
}

int Kernel::pmain(lua_State* L) {
    initSyscalls(L);

    lua_pushstring(L, "/init.lua");
    sysStart(L);

    int ret = uv_prepare_start(&schedule_prepare_, [](uv_prepare_t* p) {
        auto L = reinterpret_cast<lua_State*>(p->data);
        fromState(L)->schedule(L);
    });
    schedule_prepare_.data = L;
    if (ret < 0) {
        return luaL_error(L, "Failed to start scheduler.");
    }
    auto scheduler_guard = scopeGuard([this] {
        uv_prepare_stop(&schedule_prepare_);
    });

    ret = uv_timer_start(&frame_timer_, [](uv_timer_t* p) {
        auto curr = Clock::now();
        auto k = reinterpret_cast<Kernel*>(p->data);
        k->frame(curr - k->frame_last_);
        k->frame_last_ = curr;
    }, 0, 20);
    frame_timer_.data = this;
    if (ret < 0) {
        return luaL_error(L, "Failed to start frame timer.");
    }
    auto frame_timer_guard = scopeGuard([this] {
        uv_timer_stop(&frame_timer_);
    });

    frame_last_ = Clock::now();

    uv_run(&loop_, UV_RUN_DEFAULT);

    return 0;
}

void Kernel::schedule(lua_State* L) noexcept {
    while (!sge_List_isEmpty(&task_list_)) {
        auto node = sge_List_removeFirst(&task_list_);
		auto task = SGE_MEMBEROF(node, sge_vm_Task, node);
		auto T = taskToState(task);
        int ret = lua_resume(T, L, 0, nullptr);
        if (ret != LUA_YIELD && ret != LUA_OK) {
            // TODO
        }
	}
}

int Kernel::sysSleep(lua_State* T) {
	auto ms = luaL_checknumber(T, 1);
	luaL_argcheck(T, ms >= 0, 1, "non-negative number expected");
	auto task = taskFromState(T);
    if (ms > 0) {
        int ret = uv_timer_start(&task->sleep_timer, [](uv_timer_t* p) {
            auto task = SGE_MEMBEROF(p, Task, sleep_timer);
            sge_List_append(&fromTask(task)->task_list_, &task->node);
        }, ms, 0);
        if (ret < 0) {
			return luaL_error(T, "Failed to start sleep timer.");
        }
    } else {
        sge_List_append(&task_list_, &task->node);
    }
    return lua_yield(T, 0);
}

int Kernel::sysStart(lua_State* T) {
	auto argc = lua_gettop(T);
	luaL_argcheck(T, argc == 1 || argc == 2, 1, "Invalid arguments.");
	auto type = lua_type(T, 1);
    luaL_argcheck(T, type == LUA_TSTRING || type == LUA_TFUNCTION, 1, "String or Function expected.");

    auto create_task = [](lua_State* T, int status, lua_KContext ctx) {
        auto task = taskFromState(T);
        if (task->status != 0) {
			return luaL_error(T, "I/O failed.");
        }
        auto N = lua_newthread(T);
		taskFromState(N)->env = reinterpret_cast<lua_CFunction>(ctx);
        lua_pushvalue(T, -2);
        lua_xmove(T, N, 1);
        return 1;
    };

	auto task = taskFromState(T);
    task->status = 0;

    std::filesystem::path filepath = lua_tostring(T, 1);
    std::string env_name;
    if (argc == 1) {
        if (filepath.has_extension()) {
            env_name = filepath.extension().string().c_str() + 1;
        }
    } else {
        env_name = luaL_checkstring(T, 2);
    }

    lua_CFunction env;
    auto it = env_map_.find(env_name);
    if (it != env_map_.end()) {
        env = it->second;
    } else {
        env = task->env;
    }

    if (type != LUA_TSTRING) {
        return create_task(T, 0, lua_KContext(env));
    }

	auto file = PHYSFS_openRead(filepath.string().c_str());
    if (file == nullptr) {
		return luaL_error(T, "Failed to open file.");
    }
	task->io_work.data = file;

    int ret = uv_queue_work(&loop_, &task->io_work, [](uv_work_t* req) {
		auto file = reinterpret_cast<PHYSFS_File*>(req->data);
        auto file_guard = scopeGuard([file] {
            PHYSFS_close(file);
	    });
        req->data = nullptr;
        auto file_size = PHYSFS_fileLength(file);
        if (file_size > 0) {
            auto task = SGE_MEMBEROF(req, Task, io_work);
			void* data = malloc(file_size + 4);
            if (data == nullptr) {
                task->status = -1;
                return;
            }
			memset(data, 0, file_size + 4);
			auto ret = PHYSFS_readBytes(file, data, file_size);
            if (ret == file_size) {
                req->data = data;
            } else {
            }
        }
    }, [](uv_work_t* req, int status) {
		auto task = SGE_MEMBEROF(req, Task, io_work);
        if (status == 0 && task->status == 0) {
            auto data = reinterpret_cast<char*>(req->data);
            auto data_guard = scopeGuard([data] {
                free(reinterpret_cast<void*>(data));
            });
            req->data = nullptr;
            luaL_loadstring(taskToState(task), data);
            sge_List_append(&fromTask(task)->task_list_, &task->node);
        }
	});

    if (ret < 0) {
        PHYSFS_close(file);
        task->io_work.data = nullptr;
		return luaL_error(T, "Failed to queue I/O work.");
    }

	return lua_yieldk(T, 0, lua_KContext(env), create_task);
}

int Kernel::sysWait(lua_State* T) {
    int n = lua_gettop(T);
    luaL_argcheck(T, n == 1 || n == 2, 1, "Invalid arguments.");
    int ms = (n == 2) ? luaL_checkinteger(T, 2) : -1;
    luaL_argcheck(T, lua_type(T, 1) == LUA_TTHREAD, 1, "Thread expected.");
    auto task = taskFromState(T);
    auto wait_task = taskFromState(lua_tothread(T, 1));
    sge_List_append(&wait_task->wait_list, &task->node);
    if (ms > 0) {
        int ret = uv_timer_start(&task->sleep_timer, [](uv_timer_t* p) {
            auto task = SGE_MEMBEROF(p, Task, sleep_timer);
            task->status = -1;
            sge_List_append(&fromTask(task)->task_list_, &task->node);
		}, ms, 0);
        if (ret < 0) {
			return luaL_error(T, "Failed to start wait timer.");
        }
        return lua_yieldk(T, 0, 0, [](lua_State* T, int status, lua_KContext ctx) {
            auto task = taskFromState(T);
            lua_pushinteger(T, task->status); // 
            return 1;
		});
    }

    if (ms == 0) {
        lua_pushinteger(T, 0); // TODO
        return 1;
    }

    return lua_yield(T, 1);
}

SGE_VM_END

extern "C" void sge_vm_initHook(lua_State* L) {
}

extern "C" void sge_vm_shutdownHook(lua_State* L) {
    sge::vm::Kernel::fromState(L)->shutdownHook(L);
}

extern "C" void sge_vm_taskAddedHook(lua_State* L, lua_State* T) {
    sge::vm::Kernel::fromState(L)->taskAddedHook(L, T);
}

extern "C" void sge_vm_taskRemovedHook(lua_State* L, lua_State* T) {
    sge::vm::Kernel::fromState(L)->taskRemovedHook(L, T);
}

extern "C" void sge_vm_taskResumeHook(lua_State* T, int n) {
    sge::vm::Kernel::fromState(T)->taskResumeHook(T, n);
}

extern "C" void sge_vm_taskYieldHook(lua_State* T, int n) {
    sge::vm::Kernel::fromState(T)->taskYieldHook(T, n);
}
