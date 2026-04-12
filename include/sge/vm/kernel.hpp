//
//
#ifndef SGE_VM_KERNEL_HPP
#define SGE_VM_KERNEL_HPP

#include <map>
#include <string>
#include <chrono>

#include <sge/vm/common.hpp>
#include <sge/vm/context.hpp>

SGE_VM_BEGIN

class Kernel {
public:
    using Clock = Context::Clock;

public:
    Kernel(Context& context);
    Kernel(const Kernel&) = delete;
    Kernel(Kernel&&) = delete;
    virtual ~Kernel();

public:
    Kernel& operator=(const Kernel&) = delete;
    Kernel& operator=(Kernel&&) = delete;
    void run();
    void stop();

public: // for hooks
    static Kernel* fromTask(Task* task) noexcept {
        return reinterpret_cast<Kernel*>(task->data);
    }

    static Kernel* fromState(lua_State* L) noexcept {
        return fromTask(taskFromState(L));
    }

    void shutdownHook(lua_State* L);
    void taskAddedHook(lua_State* L, lua_State* T);
    void taskRemovedHook(lua_State* L, lua_State* T);
    void taskResumeHook(lua_State* T, int n);
    void taskYieldHook(lua_State* T, int n);

protected:
	void registerEnv(const char* name, std::function<int(lua_State*)> func);
	void unregisterEnv(const char* name);

    virtual void initSyscalls(lua_State* L);
    virtual void frame(Clock::duration elapsed) noexcept;

private:
    int pmain(lua_State* L);
    void schedule(lua_State* L) noexcept;

private:
    int sysSleep(lua_State* T);
    int sysStart(lua_State* T);
    int sysWait(lua_State* T);

private:
    Context& context_;
    uv_loop_t loop_;
    uv_async_t stop_async_;
    uv_prepare_t schedule_prepare_;
    uv_timer_t frame_timer_;
    sge_List task_list_;
    Clock::time_point frame_last_;
	std::map<std::string, lua_CFunction> env_map_;
};

SGE_VM_END

#endif // SGE_VM_KERNEL_HPP
