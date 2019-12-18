//
//
#ifndef SGE_VM_HPP
#define SGE_VM_HPP

#include <QString>
#include <QList>
#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QTimer>

#include <lua.hpp>

#include <sge/common.hpp>

SGE_BEGIN

class VM: public QThread {
	Q_OBJECT

public:
	VM(void);
	virtual ~VM(void);

public: // for lua internal
	static void initLua(lua_State *L);
	static void shutdownLua(lua_State *L);
	static void addLuaTask(lua_State *L, lua_State *L1);
	static void removeLuaTask(lua_State *L, lua_State *L1);
	static void resumeLuaTask(lua_State *L, int n);
	static void yieldLuaTask(lua_State *L, int n);

protected:
	void poll(void);

	virtual bool initMainTask(lua_State *L) = 0;

	virtual int trapInfo(lua_State *L) = 0;
	virtual int trapWarning(lua_State *L) = 0;
	virtual int trapError(lua_State *L) = 0;
	virtual int trapName(lua_State *L) = 0;
	virtual int trapSetName(lua_State *L) = 0;
	virtual int trapFps(lua_State *L) = 0;
	virtual int trapMaxFps(lua_State *L) = 0;
	virtual int trapSetMaxFps(lua_State *L) = 0;

private:
	enum Trap {
		TrapNull = 0,
		TrapInfo,
		TrapWarning,
		TrapError,
		TrapFps,
		TrapMaxFps,
		TrapSetMaxFps,
		TrapName,
		TrapSetName
	};

	typedef SGEVMContext Context;

	struct Task {
		lua_State *L;
		QTimer sleepTimer;
	};

	static Context *contextFromLua(lua_State *L);
	static lua_State *contextToLua(Context *ctx);
	static VM *vmFromContext(Context *ctx);
	static VM *vmFromLua(lua_State *L);
	static Task *taskFromContext(Context *ctx);
	static Task *taskFromLua(lua_State *L);

	void run(void) override;
	void initExports(lua_State *L);
	static int luaMain(lua_State *L);
	void main(lua_State *L);
	void scheduleTasks(lua_State *L);

	static int doTrap(lua_State *L, Trap trap);

	static int luaVersion(lua_State *L);
	static int luaInfo(lua_State *L);
	static int luaWarning(lua_State *L);
	static int luaError(lua_State *L);
	static int luaTask(lua_State *L);
	static int luaCurrent(lua_State *L);
	static int luaSleep(lua_State *L);
	static int luaFps(lua_State *L);
	static int luaMaxFps(lua_State *L);
	static int luaSetMaxFps(lua_State *L);
	static int luaName(lua_State *L);
	static int luaSetName(lua_State *L);

protected slots:
	void onTaskReady(Task *task);

private:
	QMutex m_mutex;
	QWaitCondition m_cond;
	CXList m_readyTaskList;
	CXList m_sleepTaskList;
	Task *m_mainTask;
	bool m_running;
	Trap m_trap;
	lua_State *m_trapStack;
	int m_trapResult;
};

inline VM::Context *VM::contextFromLua(lua_State *L)
{
	Q_ASSERT(L != Q_NULLPTR);
	return (Context *)lua_getextraspace(L);
}

inline lua_State *VM::contextToLua(Context *ctx)
{
	Q_ASSERT(ctx != Q_NULLPTR);
	return (lua_State *)(qintptr(ctx) + LUA_EXTRASPACE);
}

inline VM *VM::vmFromContext(Context *ctx)
{
	VM *vm = (VM *)(ctx->vm);
	Q_ASSERT(vm != Q_NULLPTR);
	return vm;
}

inline VM *VM::vmFromLua(lua_State *L)
{
	return vmFromContext(contextFromLua(L));
}

inline VM::Task *VM::taskFromContext(Context *ctx)
{
	Task *task = (Task *)(ctx->task);
	Q_ASSERT(task != Q_NULLPTR);
	return task;
}

inline VM::Task *VM::taskFromLua(lua_State *L)
{
	return taskFromContext(contextFromLua(L));
}

SGE_END

#endif // SGE_VM_HPP
