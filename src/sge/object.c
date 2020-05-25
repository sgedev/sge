/*
 *
 */
#include <sge/object.h>

struct sge_object {
	sge_list_node_t node;
	sge_list_t children;
	sge_object_t *parent;
};

void sge_object_init(lua_State *L)
{
}

void sge_object_shutdown(void)
{

}

sge_object_t *sge_object_new(const char *name, sge_object_t *parent)
{
	return NULL;
}

sge_object_t *sge_object_ref(sge_object_t *object)
{
	return NULL;
}

sge_object_t *sge_object_dup(sge_object_t *object)
{
	return NULL;
}

void sge_object_destroy(sge_object_t *object)
{

}

const char *sge_object_get_name(sge_object_t *object)
{
	return NULL;
}

void sge_object_set_name(sge_object_t *object, const char *name)
{

}
