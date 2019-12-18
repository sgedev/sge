//
//
#include <QFile>
#include <QFileInfo>
#include <QMutexLocker>

#include <sge/vm.hpp>

// TODO: luaL_error() will break the C++ class destructor!

SGE_BEGIN

VM::VM(void)
{
}

VM::~VM(void)
{
}

void VM::initLua(lua_State *L)
{
}

void VM::shutdownLua(lua_State *L)
{
}

void VM::addLuaTask(lua_State *L, lua_State *L1)
{
	VM *vm = vmFromLua(L);
	Context *ctx = contextFromLua(L1);

	Task *task = new Task; // TODO use a task pool?
	if (task == Q_NULLPTR)
		luaL_error(L, "Failed to create SGE::Task.");

	task->L = L1;

	connect(&task->sleepTimer, &QTimer::timeout, [=]() { vm->onTaskReady(task); });

	ctx->vm = (void *)vm;
	ctx->task = (void *)task;

	cxResetListNode(&ctx->node);
	cxAddListTail(&vm->m_readyTaskList, &ctx->node);
}

void VM::removeLuaTask(lua_State *L, lua_State *L1)
{
	VM *vm = vmFromLua(L);
	Context *ctx = contextFromLua(L1);

	Task *task = taskFromContext(ctx);
	delete task;

	ctx->task = Q_NULLPTR;
	ctx->vm = Q_NULLPTR;

	cxUnlinkListNode(&ctx->node);
}

void VM::resumeLuaTask(lua_State *L, int n)
{
}

void VM::yieldLuaTask(lua_State *L, int n)
{
	Context *ctx = contextFromLua(L);
	VM *vm = vmFromContext(ctx);

	cxUnlinkListNode(&ctx->node);
	cxAddListTail(&vm->m_sleepTaskList, &ctx->node);
}

void VM::poll(void)
{
	if (m_trap == TrapNull)
		return;

	QMutexLocker locker(&m_mutex);

	Q_ASSERT(m_trap != TrapNull);
	Q_ASSERT(m_trapStack != Q_NULLPTR);

	switch (m_trap) {
	case TrapInfo:
		m_trapResult = trapInfo(m_trapStack);
		break;
	case TrapWarning:
		m_trapResult = trapWarning(m_trapStack);
		break;
	case TrapError:
		m_trapResult = trapError(m_trapStack);
		break;
	case TrapFps:
		m_trapResult = trapFps(m_trapStack);
		break;
	case TrapMaxFps:
		m_trapResult = trapMaxFps(m_trapStack);
		break;
	case TrapSetMaxFps:
		m_trapResult = trapSetMaxFps(m_trapStack);
		break;
	default:
		break;
	}

	m_trap = TrapNull;
	m_trapStack = Q_NULLPTR;

	m_cond.notify_all();
}

void VM::run(void)
{
	QMutexLocker locker(&m_mutex);

	lua_State *L = luaL_newstate();
	if (L == Q_NULLPTR)
		return;

	Context *ctx = contextFromLua(L);
	cxResetListNode(&ctx->node);
	ctx->vm = (void *)this;
	ctx->task = Q_NULLPTR;

	lua_pushlightuserdata(L, this);
	lua_pushcfunction(L, &VM::luaMain);

	int rc = lua_pcall(L, 1, 0, 0);

	lua_close(L);
}

int VM::luaMain(lua_State *L)
{
	luaL_openlibs(L);

	VM *vm = (VM *)lua_touserdata(L, 1);
	Q_ASSERT(vm != Q_NULLPTR);

	vm->main(L);

	return 0;
}

void VM::main(lua_State *L)
{
	initExports(L);

	lua_State *mainTask = lua_newthread(L);
	if (mainTask == NULL)
		return;

	if (!initMainTask(mainTask))
		return;

	m_running = true;

	while (m_running) {
		scheduleTasks(L);
		exec();
	}
}

void VM::initExports(lua_State *L)
{
	lua_newtable(L);

	lua_pushcfunction(L, &VM::luaVersion);
	lua_setfield(L, -2, "version");

	lua_pushcfunction(L, &VM::luaInfo);
	lua_setfield(L, -2, "info");

	lua_pushcfunction(L, &VM::luaWarning);
	lua_setfield(L, -2, "warning");

	lua_pushcfunction(L, &VM::luaError);
	lua_setfield(L, -2, "error");

	lua_pushcfunction(L, &VM::luaTask);
	lua_setfield(L, -2, "task");

	lua_pushcfunction(L, &VM::luaSleep);
	lua_setfield(L, -2, "sleep");

	lua_pushcfunction(L, &VM::luaFps);
	lua_setfield(L, -2, "fps");

	lua_pushcfunction(L, &VM::luaMaxFps);
	lua_setfield(L, -2, "fpsMax");

	lua_pushcfunction(L, &VM::luaSetMaxFps);
	lua_setfield(L, -2, "setFpsMax");

	lua_pushcfunction(L, &VM::luaName);
	lua_setfield(L, -2, "name");

	lua_pushcfunction(L, &VM::luaSetName);
	lua_setfield(L, -2, "setName");

	lua_setglobal(L, "sge");
}

void VM::scheduleTasks(lua_State *L)
{
	CXListNode *node;
	Context *ctx;
	Task *task;

	for (;;) {
		node = cxDeleteListHead(&m_readyTaskList);
		if (node == cxGetListKnot(&m_readyTaskList))
			break;
		ctx = CX_MEMBEROF(node, Context, node);
		task = taskFromContext(ctx);
		int ret = lua_resume(task->L, L, 0);
		if (ret != LUA_YIELD && ret != LUA_OK)
			luaL_error(L, "LUA_ERROR: %s\n", luaL_checkstring(L, -1));
	}
}

int VM::doTrap(lua_State *L, Trap trap)
{
	Q_ASSERT(L != Q_NULLPTR);
	Q_ASSERT(trap != TrapNull);

	VM *vm = vmFromLua(L);
	Q_ASSERT(QThread::currentThread() == static_cast<QThread *>(vm));

	Q_ASSERT(vm->m_trap == TrapNull);
	Q_ASSERT(vm->m_trapStack == Q_NULLPTR);

	vm->m_trap = trap;
	vm->m_trapStack = L;

	while (vm->m_trap != TrapNull)
		vm->m_cond.wait(&vm->m_mutex);

	return vm->m_trapResult;
}

int VM::luaVersion(lua_State *L)
{
	lua_newtable(L);

	lua_pushinteger(L, SGE_VERSION_MAJOR);
	lua_rawseti(L, -1, 0);

	lua_pushinteger(L, SGE_VERSION_MINOR);
	lua_rawseti(L, -1, 1);

	lua_pushinteger(L, SGE_VERSION_PATCH);
	lua_rawseti(L, -1, 2);

	return 1;
}

int VM::luaInfo(lua_State *L)
{
	luaL_checkstring(L, 1);

	return doTrap(L, TrapInfo);
}

int VM::luaWarning(lua_State *L)
{
	luaL_checkstring(L, 1);

	return doTrap(L, TrapWarning);
}

int VM::luaError(lua_State *L)
{
	luaL_checkstring(L, 1);

	return doTrap(L, TrapError);
}

int VM::luaTask(lua_State *L)
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

int VM::luaCurrent(lua_State *L)
{
	lua_pushthread(L);
	return 1;
}

int VM::luaSleep(lua_State *L)
{
	Task *task = taskFromLua(L);
	
	task->sleepTimer.start(luaL_checkinteger(L, 1));

	return lua_yield(L, 0);
}

int VM::luaFps(lua_State *L)
{
	return doTrap(L, TrapFps);
}

int VM::luaMaxFps(lua_State *L)
{
	return doTrap(L, TrapMaxFps);
}

int VM::luaSetMaxFps(lua_State *L)
{
	luaL_checkinteger(L, 1);

	return doTrap(L, TrapSetMaxFps);
}

int VM::luaName(lua_State *L)
{
	return doTrap(L, TrapName);
}

int VM::luaSetName(lua_State *L)
{
	luaL_checkstring(L, 1);

	return doTrap(L, TrapSetName);
}

void VM::onTaskReady(Task *task)
{
	Context *ctx= contextFromLua(task->L);
	VM *vm = vmFromContext(ctx);

	cxUnlinkListNode(&ctx->node);
	cxAddListTail(&vm->m_readyTaskList, &ctx->node);

	QThread::exit(0);
}

SGE_END

extern "C" void SGEVMInitLua(lua_State *L)
{
	SGE::VM::initLua(L);
}

extern "C" void SGEVMShutdownLua(lua_State *L)
{
	SGE::VM::shutdownLua(L);
}

extern "C" void SGEVMAddLuaTask(lua_State *L, lua_State *L1)
{
	SGE::VM::addLuaTask(L, L1);
}

extern "C" void SGEVMRemoveLuaTask(lua_State *L, lua_State *L1)
{
	SGE::VM::removeLuaTask(L, L1);
}

extern "C" void SGEVMResumeLuaTask(lua_State *L, int n)
{
	SGE::VM::resumeLuaTask(L, n);
}

extern "C" void SGEVMYieldLuaTask(lua_State *L, int n)
{
	SGE::VM::yieldLuaTask(L, n);
}
