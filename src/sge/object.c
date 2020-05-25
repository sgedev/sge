/*
 *
 */
#include <string.h>

#include <sge/object.h>

#define SGE_OBJECT_TYPE "sge.object"

struct sge_object {
	sge_list_node_t node;
	sge_list_t children;
	sge_object_t *parent;
	char name[SGE_OBJECT_NAME_MAX];
};

static sge_object_t sge_object_root;
static sge_list_t sge_object_watch_list;

static bool sge_object_watch_exists(const sge_object_watch_t *watch)
{
	sge_list_node_t *node;
	sge_object_watch_t *w;

	SGE_LIST_FOREACH(node, &sge_object_watch_list) {
		w = SGE_MEMBEROF(node, sge_object_watch_t, node);
		if (w == watch)
			return true;
	}

	return false;
}

static int sge_object_destroy(lua_State *L)
{
	return 0;
}

static int sge_object_new(lua_State *L)
{
	return 0;
}

void sge_object_export(lua_State *L)
{
	lua_pushcfunction(L, &sge_object_new);
	lua_setfield(L, -2, "Object");
}

bool sge_object_init(void)
{
	sge_object_root.parent = NULL;
	sge_list_reset(&sge_object_root.children);
	sge_list_node_reset(&sge_object_root.node);

	sge_list_reset(&sge_object_watch_list);

	return true;
}

void sge_object_shutdown(void)
{
}

const char *sge_object_get_name(sge_object_t *object)
{
	return NULL;
}

void sge_object_set_name(sge_object_t *object, const char *name)
{

}

sge_object_t *sge_object_get_parent(sge_object_t *object)
{
	return NULL;
}

void sge_object_set_parent(sge_object_t *object, sge_object_t *parent)
{

}

void sge_object_watch_add(sge_object_watch_t *watch)
{
	SGE_ASSERT(watch != NULL);
	SGE_ASSERT(!sge_object_watch_exists(watch));

	sge_list_node_reset(&watch->node);
	sge_list_add_tail(&sge_object_watch_list, &watch->node);

	/* TODO make object callbacks... */
}

void sge_object_watch_remove(sge_object_watch_t *watch)
{
	SGE_ASSERT(watch != NULL);
	SGE_ASSERT(sge_object_watch_exists(watch));

	/* TODO make object callbacks... */

	sge_list_node_unlink(&watch->node);
}
