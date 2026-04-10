//
//
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
    lua_close(state_);
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
    task->data = main->data;
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

void Kernel::frame(Clock::duration elapsed) noexcept {
}

int Kernel::pmain(lua_State* L) {
    initSyscalls(L);
    loadInitTask(L);

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

void Kernel::initSyscalls(lua_State* L) {
    context_.initSyscalls(L);
}

void Kernel::loadInitTask(lua_State* L) {

}

void Kernel::schedule(lua_State* L) noexcept {

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
