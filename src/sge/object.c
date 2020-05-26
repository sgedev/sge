/*
 *
 */
#include <string.h>

#include <lauxlib.h>

#include <sge/object.h>

#define SGE_OBJECT_TYPE "sge.object"

cx_sig(sge_object) sge_object_added;
cx_sig(sge_object) sge_object_removed;

static sge_object_t sge_object_root;

static void slot_test(void)
{

}

static void sge_object_ctor(sge_object_t *object)
{
	SGE_ASSERT(object != NULL);

	memset(object, 0, sizeof(sge_object_t));

	cx_sig(void) sig;
	cx_sig_init(void, sig);

	cx_slot(void) slot;
	cx_slot_init(void, slot, slot_test);

	cx_sig_connect(void, sig, slot);
	cx_sig_disconnect(void, sig, slot);
	cx_sig_emit(void, sig);

	object->parent = NULL;
	cx_list_reset(&object->children);
	cx_list_node_reset(&object->node);
}

static int sge_object_new(lua_State *L)
{
	const char *name;
	sge_object_t *parent = &sge_object_root;
	sge_object_t *object;

	switch (lua_gettop(L)) {
	case 2:
		parent = (sge_object_t *)luaL_checkudata(L, 2, SGE_OBJECT_TYPE);
	case 1:
		name = luaL_checkstring(L, 1);
		break;
	default:
		luaL_error(L, "invalid param.");
		break;
	}

	object = lua_newuserdata(L, sizeof(sge_object_t));

	sge_object_ctor(object);

	luaL_setmetatable(L, SGE_OBJECT_TYPE);

	return 1;
}

static int sge_object_destroy(lua_State *L)
{
	sge_object_t *object = (sge_object_t *)luaL_checkudata(L, 1, SGE_OBJECT_TYPE);
	sge_object_t *child;
	cx_list_node_t *node;

	CX_LIST_FOREACH(node, &object->children) {
		child = CX_MEMBEROF(node, sge_object_t, node);
		/* TODO */
	}

	cx_list_node_unlink(&object->node);
	object->parent = NULL;

	return 0;
}

static int sge_object_index(lua_State *L)
{
	lua_pushnil(L);
	return 1;
}

static int sge_object_newindex(lua_State *L)
{
	return 0;
}

void sge_object_export(lua_State *L)
{
	lua_pushcfunction(L, &sge_object_new);
	lua_setfield(L, -2, "Object");

	luaL_newmetatable(L, SGE_OBJECT_TYPE);

	lua_pushcfunction(L, &sge_object_destroy);
	lua_setfield(L, -2, "__gc");

	lua_pushcfunction(L, &sge_object_index);
	lua_setfield(L, -2, "__index");

	lua_pushcfunction(L, &sge_object_newindex);
	lua_setfield(L, -2, "__newindex");

	lua_pop(L, 1);
}

bool sge_object_init(void)
{
	sge_object_ctor(&sge_object_root);

	return true;
}

void sge_object_shutdown(void)
{
}

sge_object_t *sge_object_get_root(void)
{
	return &sge_object_root;
}

void sge_object_set_parent(sge_object_t *object, sge_object_t *parent)
{
	SGE_ASSERT(object != NULL);
	SGE_ASSERT(object != &sge_object_root);

	cx_list_node_unlink(&object->node);

	if (parent == NULL)
		parent = &sge_object_root;

	object->parent = parent;
	cx_list_add_tail(&parent->children, &object->node);
}

sge_object_t *sge_object_get_first_child(sge_object_t *object)
{
	cx_list_node_t *node;

    SGE_ASSERT(object != NULL);

    node = cx_list_head(&object->children);
	if (node == cx_list_knot(&object->children))
		return NULL;

	return CX_MEMBEROF(node, sge_object_t, node);
}

sge_object_t *sge_object_get_next_sibling(sge_object_t *object)
{
	cx_list_node_t *node;

    SGE_ASSERT(object != NULL);

	if (object == &sge_object_root)
		return NULL;

	SGE_ASSERT(object->parent != NULL);

    node = cx_list_node_next(&object->node);
	if (node == cx_list_knot(&object->parent->children))
		return NULL;

	return CX_MEMBEROF(node, sge_object_t, node);
}
