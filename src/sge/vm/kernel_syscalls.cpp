//
//
#include <string>

#include <sge/graphics/directional_light.hpp>
#include <sge/graphics/point_light.hpp>
#include <sge/graphics/spot_light.hpp>
#include <sge/graphics/mesh.hpp>

#include <sge/vm/kernel.hpp>

SGE_VM_BEGIN

static const char *const DIRECTION_LIGHT_TYPE = "sge.directon_light";
static const char *const POINT_LIGHT_TYPE = "sge.point_light";
static const char *const SPOT_LIGHT_TYPE = "sge.spot_light";
static const char *const STATIC_MESH_TYPE = "sge.static_mesh";
static const char *const DYNAMIC_MESH_TYPE = "sge.dynamic_mesh";

void kernel::init_exports(lua_State *L)
{
    luaL_newmetatable(L, DIRECTION_LIGHT_TYPE);

    lua_pushstring(L, "__gc");
    lua_pushcfunction(L, &kernel::sys_light_destroy);
    lua_rawset(L, -2);

    lua_pop(L, 1);

    luaL_newmetatable(L, POINT_LIGHT_TYPE);
    lua_pop(L, 1);

    luaL_newmetatable(L, SPOT_LIGHT_TYPE);
    lua_pop(L, 1);

    luaL_newmetatable(L, STATIC_MESH_TYPE);
    lua_pop(L, 1);

    lua_newtable(L);

    lua_pushcfunction(L, &kernel::sys_task_new);
    lua_setfield(L, -2, "task_new");

    lua_pushcfunction(L, &kernel::sys_current);
    lua_setfield(L, -2, "current");

    lua_pushcfunction(L, &kernel::sys_wait);
    lua_setfield(L, -2, "wait");

    lua_pushcfunction(L, &kernel::sys_sleep);
    lua_setfield(L, -2, "sleep");

    lua_pushcfunction(L, &kernel::sys_directional_light_new);
    lua_setfield(L, -2, "directional_light_new");

    lua_pushcfunction(L, &kernel::sys_point_light_new);
    lua_setfield(L, -2, "point_light_new");

    lua_pushcfunction(L, &kernel::sys_spot_light_new);
    lua_setfield(L, -2, "spot_light_new");

    lua_pushcfunction(L, &kernel::sys_static_mesh_new);
    lua_setfield(L, -2, "static_mesh_new");

    lua_pushcfunction(L, &kernel::sys_dynamic_mesh_new);
    lua_setfield(L, -2, "dynamic_mesh_new");

    lua_setglobal(L, "sge");
}

int kernel::sys_current(lua_State *L)
{
    lua_pushthread(L);
    return 1;
}

int kernel::sys_task_new(lua_State *L)
{
    lua_State *T = lua_newthread(L);
    int top = lua_gettop(L);
    if (top != 1)
        luaL_error(L, "one parameter.");

    int type = lua_type(L, 1);
    switch (type) {
    case LUA_TSTRING:
        luaL_loadstring(T, lua_tostring(L, 1));
        break;
    case LUA_TFUNCTION:
        lua_xmove(L, T, 1);
        break;
    default:
        luaL_error(L, "invalid paramter type.");
        break;
    }

    return 1;
}

int kernel::sys_sleep(lua_State *L)
{
    int ms = (int)luaL_checkinteger(L, 1);
    task_t *task = task_from_lua(L);
    kernel *k = from_task(task);

    std::unique_lock locker(k->m_task_list_lock);
    sge_list_node_unlink(&task->node);

    if (ms < 1)
        sge_list_add_tail(&k->m_task_list_ready, &task->node);
    else
        uv_timer_start(&task->sleep_timer, sys_sleep_done, ms, 0);

    return lua_yield(L, 0);
}

void kernel::sys_sleep_done(uv_timer_t *p)
{
    task_t *task = (task_t *)(p->data);
    kernel *k = from_task(task);

    std::unique_lock locker(k->m_task_list_lock);
    sge_list_node_unlink(&task->node);
    sge_list_add_tail(&k->m_task_list_ready, &task->node);
}

int kernel::sys_wait(lua_State *L)
{
    lua_State *T = nullptr;
    unsigned int timeout = 0;

    switch (lua_gettop(L)) {
    case 2:
        timeout = luaL_checkinteger(L, 2);
        luaL_argcheck(L, timeout >= 0, 2, "invalid timeout.");
    case 1:
        luaL_checktype(L, 1, LUA_TTHREAD);
        T = lua_tothread(L, 1);
        break;
    default:
        luaL_error(L, "too many parameters.");
        break;
    }

    task_t *curr = task_from_lua(L);
    task_t *task = task_from_lua(T);

    if (timeout > 0)
        uv_timer_start(&curr->sleep_timer, &kernel::sys_wait_timeout, timeout, 0);

    task->sys_result = SYS_OK;

    std::unique_lock locker(from_task(task)->m_task_list_lock);
    sge_list_node_unlink(&task->node);
    sge_list_add_tail(&task->wait_list, &curr->node);

    return lua_yieldk(L, 0, 0, &kernel::sys_wait_done);
}

void kernel::sys_wait_timeout(uv_timer_t *p)
{
    task_t *task = reinterpret_cast<task_t *>(p->data);
    kernel *k = from_task(task);

    task->sys_result = SYS_TIMEOUT;

    std::unique_lock locker(k->m_task_list_lock);
    sge_list_node_unlink(&task->node);
    sge_list_add_tail(&k->m_task_list_ready, &task->node);
}

int kernel::sys_wait_done(lua_State *L, int status, lua_KContext ctx)
{
    SGE_UNUSED(status);
    SGE_UNUSED(ctx);

    task_t *task = task_from_lua(L);
    int ret = task->sys_result;
    task->sys_result = SYS_OK;

    return ret;
}

int kernel::sys_directional_light_new(lua_State *L)
{
    return do_trap(L, [](lua_State *L) {
        return 0;
    });
}

int kernel::sys_point_light_new(lua_State *L)
{
    return do_trap(L, [](lua_State *L) {
        return 0;
    });
}

int kernel::sys_spot_light_new(lua_State *L)
{
    return do_trap(L, [](lua_State *L) {
        return 0;
    });
}

int kernel::sys_light_destroy(lua_State *L)
{
    return do_trap(L, [](lua_State *L) {
        return 0;
    });
}

int kernel::sys_static_mesh_new(lua_State *L)
{
    return do_trap(L, [](lua_State *L) {
        return 0;
    });
}

int kernel::sys_dynamic_mesh_new(lua_State *L)
{
    return do_trap(L, [](lua_State *L) {
        return 0;
    });
}

int kernel::sys_mesh_destroy(lua_State *L)
{
    return do_trap(L, [](lua_State *L) {
        return 0;
    });
}

SGE_VM_END
