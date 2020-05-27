/*
 *
 */
#include <string.h>

#include <sge/lua.h>
#include <sge/object.h>

#define SGE_OBJECT_TYPE "sge.object"

#define SGE_OBJECT_BUILTIN_PROP_NAME "name"
#define SGE_OBJECT_BUILTIN_PROP_ENABLED "enabled"
#define SGE_OBJECT_BUILTIN_PROP_VISIBLED "visibled"
#define SGE_OBJECT_BUILTIN_PROP_POS "pos"
#define SGE_OBJECT_BUILTIN_PROP_SCALE "scale"
#define SGE_OBJECT_BUILTIN_PROP_ROTATION "rotatoin"
#define SGE_OBJECT_BUILTIN_PROP_MASS "mass"

struct sge_object_new_watch {
	cx_list_node_t node;
	sge_object_new_watch_func_t func;
	void *data;
};

struct sge_object_watch {
	cx_list_node_t node;
	const sge_object_watch_funcs_t *funcs;
	void *data;
};

struct sge_object {
	lua_State *L;
	int ref;
	cx_list_t watch_list;
};

static cx_list_t sge_object_new_watch_list;

static bool sge_object_is_builtin_property(const char *prop)
{
	if (strcmp(prop, SGE_OBJECT_BUILTIN_PROP_POS) == 0)
		return true;

	if (strcmp(prop, SGE_OBJECT_BUILTIN_PROP_SCALE) == 0)
		return true;

	if (strcmp(prop, SGE_OBJECT_BUILTIN_PROP_ROTATION) == 0)
		return true;

	if (strcmp(prop, SGE_OBJECT_BUILTIN_PROP_ENABLED) == 0)
		return true;

	if (strcmp(prop, SGE_OBJECT_BUILTIN_PROP_VISIBLED) == 0)
		return true;

	if (strcmp(prop, SGE_OBJECT_BUILTIN_PROP_NAME) == 0)
		return true;

	if (strcmp(prop, SGE_OBJECT_BUILTIN_PROP_MASS) == 0)
		return true;

	return false;
}

static void sge_object_set_enabled_internal(sge_object_t *obj, bool v)
{
	cx_list_node_t *node;
	sge_object_watch_t *watch;

	SGE_ASSERT(obj != NULL);

	CX_LIST_FOREACH(node, &obj->watch_list) {
		watch = CX_MEMBEROF(node, sge_object_watch_t, node);
		watch->funcs->set_enabled(obj, v, watch->data);
	}
}

static void sge_object_set_visibled_internal(sge_object_t *obj, bool v)
{
	cx_list_node_t *node;
	sge_object_watch_t *watch;

	SGE_ASSERT(obj != NULL);

	CX_LIST_FOREACH(node, &obj->watch_list) {
		watch = CX_MEMBEROF(node, sge_object_watch_t, node);
		watch->funcs->set_visibled(obj, v, watch->data);
	}
}

static void sge_object_set_name_internal(sge_object_t *obj, const char *name)
{
	cx_list_node_t *node;
	sge_object_watch_t *watch;

	SGE_ASSERT(obj != NULL);

	CX_LIST_FOREACH(node, &obj->watch_list) {
		watch = CX_MEMBEROF(node, sge_object_watch_t, node);
		watch->funcs->set_name(obj, name, watch->data);
	}
}

static void sge_object_set_pos_internal(sge_object_t *obj, const cx_vec3_t *p)
{
	cx_list_node_t *node;
	sge_object_watch_t *watch;

	SGE_ASSERT(obj != NULL);

	CX_LIST_FOREACH(node, &obj->watch_list) {
		watch = CX_MEMBEROF(node, sge_object_watch_t, node);
		watch->funcs->set_pos(obj, p, watch->data);
	}
}

static void sge_object_set_scale_internal(sge_object_t *obj, const cx_vec3_t *p)
{
	cx_list_node_t *node;
	sge_object_watch_t *watch;

	SGE_ASSERT(obj != NULL);

	CX_LIST_FOREACH(node, &obj->watch_list) {
		watch = CX_MEMBEROF(node, sge_object_watch_t, node);
		watch->funcs->set_scale(obj, p, watch->data);
	}
}

static void sge_object_set_rotation_internal(sge_object_t *obj, const cx_quat_t *p)
{
	cx_list_node_t *node;
	sge_object_watch_t *watch;

	SGE_ASSERT(obj != NULL);

	CX_LIST_FOREACH(node, &obj->watch_list) {
		watch = CX_MEMBEROF(node, sge_object_watch_t, node);
		watch->funcs->set_rotation(obj, p, watch->data);
	}
}

static void sge_object_set_mass_internal(sge_object_t *obj, float v)
{
	cx_list_node_t *node;
	sge_object_watch_t *watch;

	SGE_ASSERT(obj != NULL);

	CX_LIST_FOREACH(node, &obj->watch_list) {
		watch = CX_MEMBEROF(node, sge_object_watch_t, node);
		watch->funcs->set_mass(obj, v, watch->data);
	}
}

static void sge_object_set_property_internal(sge_object_t *obj, const char *prop, const char *value)
{
	cx_list_node_t *node;
	sge_object_watch_t *watch;

	SGE_ASSERT(obj != NULL);
	SGE_ASSERT(prop != NULL);
	SGE_ASSERT(!sge_object_is_builtin_property(prop));

	CX_LIST_FOREACH(node, &obj->watch_list) {
		watch = CX_MEMBEROF(node, sge_object_watch_t, node);
		watch->funcs->set_property(obj, prop, value, watch->data);
	}
}

static int sge_object_gc(lua_State *L)
{
	sge_object_t *obj = luaL_checkudata(L, 1, SGE_OBJECT_TYPE);
	cx_list_node_t *node;
	sge_object_watch_t *watch;
	
	// FIXME: dengerous iterate, destroy will remove itself from watch_list!
	CX_LIST_FOREACH(node, &obj->watch_list) {
		watch = CX_MEMBEROF(node, sge_object_watch_t, node);
		watch->funcs->destroy(obj, watch->data);
	}

	luaL_unref(L, LUA_REGISTRYINDEX, obj->ref);
	obj->ref = LUA_NOREF;
	obj->L = NULL;

	return 0;
}

static int sge_object_newindex(lua_State *L)
{
	sge_object_t *obj = luaL_checkudata(L, 1, SGE_OBJECT_TYPE);
	const char *prop = luaL_checkstring(L, 2);

	if (strcmp(prop, SGE_OBJECT_BUILTIN_PROP_POS) == 0) {
		cx_vec3_t v;
		sge_lua_check_vec3(L, 3, &v);
		sge_object_set_pos_internal(obj, &v);
	} else if (strcmp(prop, SGE_OBJECT_BUILTIN_PROP_SCALE) == 0) {
		cx_vec3_t v;
		sge_lua_check_vec3(L, 3, &v);
		sge_object_set_scale_internal(obj, &v);
	} else if (strcmp(prop, SGE_OBJECT_BUILTIN_PROP_ROTATION) == 0) {
		cx_quat_t v;
		sge_lua_check_quat(L, 3, &v);
		sge_object_set_rotation_internal(obj, &v);
	} else if (strcmp(prop, SGE_OBJECT_BUILTIN_PROP_ENABLED) == 0) {
		sge_object_set_enabled_internal(obj, lua_toboolean(L, 3));
	} else if (strcmp(prop, SGE_OBJECT_BUILTIN_PROP_VISIBLED) == 0) {
		sge_object_set_visibled_internal(obj, lua_toboolean(L, 3));
	} else if (strcmp(prop, SGE_OBJECT_BUILTIN_PROP_NAME) == 0) {
		sge_object_set_name_internal(obj, luaL_checkstring(L, 3));
	} else if (strcmp(prop, SGE_OBJECT_BUILTIN_PROP_MASS) == 0) {
		sge_object_set_mass_internal(obj, luaL_checknumber(L, 3));
	} else {
		sge_object_set_property_internal(obj, prop, luaL_checkstring(L, 3));
	}

	lua_rawset(L, 1);

	return 0;
}

static int sge_object_new(lua_State *L)
{
	const char *name = luaL_checkstring(L, 1);
	sge_object_t *obj = lua_newuserdata(L, sizeof(sge_object_t));
	cx_list_node_t *node;
	sge_object_new_watch_t *new_watch;

	obj->L = L;
	obj->ref = luaL_ref(L, LUA_REGISTRYINDEX);
	cx_list_reset(&obj->watch_list);
	luaL_setmetatable(L, SGE_OBJECT_TYPE);

	CX_LIST_FOREACH(node, &sge_object_new_watch_list) {
		new_watch = CX_MEMBEROF(node, sge_object_new_watch_t, node);
		new_watch->func(obj, new_watch->data);
	}

	return 1;
}

void sge_object_export(lua_State *L)
{
	lua_pushcfunction(L, &sge_object_new);
	lua_setfield(L, -2, "Object");

	luaL_newmetatable(L, SGE_OBJECT_TYPE);

	lua_pushcfunction(L, &sge_object_gc);
	lua_setfield(L, -2, "__gc");

	lua_pushcfunction(L, &sge_object_newindex);
	lua_setfield(L, -2, "__newindex");

	lua_pop(L, 1);
}

bool sge_object_init(void)
{
	cx_list_reset(&sge_object_new_watch_list);

	return true;
}

void sge_object_shutdown(void)
{
	SGE_ASSERT(cx_list_empty(&sge_object_new_watch_list));
}

static sge_object_new_watch_t *sge_object_find_new_watch(sge_object_new_watch_func_t func, void *data)
{
	cx_list_node_t *node;
	sge_object_new_watch_t *new_watch;

	SGE_ASSERT(func != NULL);

	CX_LIST_FOREACH(node, &sge_object_new_watch_list) {
		new_watch = CX_MEMBEROF(node, sge_object_new_watch_t, node);
		if (new_watch->func == func && new_watch->data == data)
			return new_watch;
	}

	return NULL;
}

sge_object_new_watch_t *sge_object_add_new_watch(sge_object_new_watch_func_t func, void *data)
{
	sge_object_new_watch_t *new_watch;

	SGE_ASSERT(func != NULL);

	new_watch = sge_object_find_new_watch(func, data);
	if (new_watch != NULL)
		return NULL;

	new_watch = malloc(sizeof(sge_object_new_watch_t));
	if (new_watch == NULL)
		return NULL;

	new_watch->func = func;
	new_watch->data = data;

	cx_list_node_reset(&new_watch->node);
	cx_list_add_tail(&sge_object_new_watch_list, &new_watch->node);

	return new_watch;
}

void sge_object_remove_new_watch(sge_object_new_watch_t *new_watch)
{
	SGE_ASSERT(new_watch != NULL);

	cx_list_del_node(&sge_object_new_watch_list, &new_watch->node);
	free(new_watch);
}

static sge_object_watch_t *sge_object_find_watch(
	sge_object_t *obj, const sge_object_watch_funcs_t *funcs, void *data)
{
	cx_list_node_t *node;
	sge_object_watch_t *watch;

	SGE_ASSERT(obj != NULL);

	CX_LIST_FOREACH(node, &obj->watch_list) {
		watch = CX_MEMBEROF(node, sge_object_watch_t, node);
		if (watch->funcs == funcs && watch->data == data)
			return watch;
	}

	return NULL;
}

sge_object_watch_t *sge_object_add_watch(sge_object_t *obj, const sge_object_watch_funcs_t *funcs, void *data)
{
	sge_object_watch_t *watch;

	SGE_ASSERT(obj != NULL);
	SGE_ASSERT(funcs != NULL);

	watch = sge_object_find_watch(obj, funcs, data);
	if (watch != NULL)
		return NULL;

	watch = malloc(sizeof(sge_object_watch_t));
	if (watch == NULL)
		return NULL;

	watch->funcs = funcs;
	watch->data = data;

	cx_list_node_reset(&watch->node);
	cx_list_add_tail(&obj->watch_list, &watch->node);

	return watch;
}

void sge_object_remove_watch(sge_object_t *obj, sge_object_watch_t *watch)
{
	SGE_ASSERT(obj != NULL);

	cx_list_del_node(&obj->watch_list, &watch->node);
	free(watch);
}

void sge_object_set_enabled(sge_object_t *obj, bool v)
{
	sge_object_set_enabled_internal(obj, v);

	lua_rawgeti(obj->L, LUA_REGISTRYINDEX, obj->ref);
	lua_pushboolean(obj->L, v);
	lua_setfield(obj->L, -2, SGE_OBJECT_BUILTIN_PROP_ENABLED);
	lua_pop(obj->L, 1);
}

void sge_object_set_visibled(sge_object_t *obj, bool v)
{
	sge_object_set_visibled_internal(obj, v);

	lua_rawgeti(obj->L, LUA_REGISTRYINDEX, obj->ref);
	lua_pushboolean(obj->L, v);
	lua_setfield(obj->L, -2, SGE_OBJECT_BUILTIN_PROP_VISIBLED);
	lua_pop(obj->L, 1);
}

void sge_object_set_name(sge_object_t *obj, const char *name)
{
	sge_object_set_name_internal(obj, name);

	lua_rawgeti(obj->L, LUA_REGISTRYINDEX, obj->ref);
	lua_pushstring(obj->L, name);
	lua_setfield(obj->L, -2, SGE_OBJECT_BUILTIN_PROP_NAME);
	lua_pop(obj->L, 1);
}

void sge_object_set_pos(sge_object_t *obj, const cx_vec3_t *p)
{
	sge_object_set_pos_internal(obj, p);

	lua_rawgeti(obj->L, LUA_REGISTRYINDEX, obj->ref);
	sge_lua_push_vec3(obj->L, p);
	lua_setfield(obj->L, -2, SGE_OBJECT_BUILTIN_PROP_POS);
	lua_pop(obj->L, 1);
}

void sge_object_set_scale(sge_object_t *obj, const cx_vec3_t *p)
{
	sge_object_set_scale_internal(obj, p);

	lua_rawgeti(obj->L, LUA_REGISTRYINDEX, obj->ref);
	sge_lua_push_vec3(obj->L, p);
	lua_setfield(obj->L, -2, SGE_OBJECT_BUILTIN_PROP_SCALE);
	lua_pop(obj->L, 1);
}

void sge_object_set_rotation(sge_object_t *obj, const cx_quat_t *p)
{
	sge_object_set_rotation_internal(obj, p);

	lua_rawgeti(obj->L, LUA_REGISTRYINDEX, obj->ref);
	sge_lua_push_quat(obj->L, p);
	lua_setfield(obj->L, -2, SGE_OBJECT_BUILTIN_PROP_ROTATION);
	lua_pop(obj->L, 1);
}

void sge_object_set_mass(sge_object_t *obj, float v)
{
	sge_object_set_mass_internal(obj, v);

	lua_rawgeti(obj->L, LUA_REGISTRYINDEX, obj->ref);
	lua_pushnumber(obj->L, v);
	lua_setfield(obj->L, -2, SGE_OBJECT_BUILTIN_PROP_MASS);
	lua_pop(obj->L, 1);
}

void sge_object_set_property(sge_object_t *obj, const char *prop, const char *value)
{
	sge_object_set_property_internal(obj, prop, value);

	lua_rawgeti(obj->L, LUA_REGISTRYINDEX, obj->ref);

	if (value != NULL)
		lua_pushstring(obj->L, value);
	else
		lua_pushnil(obj->L);

	lua_setfield(obj->L, -2, prop);
	lua_pop(obj->L, 1);
}
