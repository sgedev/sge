/*
 *
 */
#include <cx/list.h>

#include <sge/object.h>
#include <sge/renderer.h>

typedef struct {
	cx_list_node_t node;
	cx_list_node_t node_material;
	GLEXMesh *mesh;
} sge_renderer_mesh_t;

typedef struct {
	GLEXMaterial *material;
	cx_list_t mesh_list;
} sge_renderer_material_t;

typedef struct {
	sge_object_t *obj;
	sge_object_watch_t *watch;
	cx_list_t mesh_list;
} sge_renderer_object_t;

static GLEXContext *sge_renderer_glex;
static sge_object_new_watch_t *sge_renderer_object_new_watch;

static void sge_renderer_object_destroy(sge_object_t *obj, void *data)
{
	sge_renderer_object_t *robj = (sge_renderer_object_t *)data;

	sge_object_remove_watch(obj, robj->watch);
	free(robj);
}

static void sge_renderer_object_set_enabled(sge_object_t *obj, bool v, void *data)
{
	sge_renderer_object_t *robj = (sge_renderer_object_t *)data;
}

static void sge_renderer_object_set_visibled(sge_object_t *obj, bool v, void *data)
{
	sge_renderer_object_t *robj = (sge_renderer_object_t *)data;
}

static void sge_renderer_object_set_pos(sge_object_t *obj, const cx_vec3_t *p, void *data)
{
	sge_renderer_object_t *robj = (sge_renderer_object_t *)data;
}

static void sge_renderer_object_set_scale(sge_object_t *obj, const cx_vec3_t *p, void *data)
{
	sge_renderer_object_t *robj = (sge_renderer_object_t *)data;
}

static void sge_renderer_object_set_rotation(sge_object_t *obj, const cx_quat_t *p, void *data)
{
	sge_renderer_object_t *robj = (sge_renderer_object_t *)data;
}

static void sge_renderer_object_set_property(sge_object_t *obj, const char *prop, const char *value, void *data)
{
	sge_renderer_object_t *robj = (sge_renderer_object_t *)data;
}

static void sge_renderer_object_new(sge_object_t *obj, void *data)
{
	static const sge_object_watch_funcs_t watch_funcs = {
		.destroy = sge_renderer_object_destroy,
		.set_enabled = sge_renderer_object_set_enabled,
		.set_visibled = sge_renderer_object_set_visibled,
		.set_pos = sge_renderer_object_set_pos,
		.set_scale = sge_renderer_object_set_scale,
		.set_rotation = sge_renderer_object_set_rotation,
		.set_property = sge_renderer_object_set_property
	};

	sge_renderer_object_t *robj = (sge_renderer_object_t *)malloc(sizeof(sge_renderer_object_t));
	if (robj == NULL) {
		// TODO
		return;
	}

	robj->obj = obj;
	robj->watch = sge_object_add_watch(obj, &watch_funcs, robj);

	cx_list_reset(&robj->mesh_list);
}

void sge_renderer_export(lua_State *L)
{
	lua_newtable(L);

	lua_setglobal(L, "renderer");
}

bool sge_renderer_init(void)
{
	SGE_ASSERT(sge_renderer_glex == NULL);
	sge_renderer_glex = glexCreateContext();
	if (sge_renderer_glex == NULL) {
		SGE_LOGE("Failed to init GLEX.\n");
		return false;
	}

	SGE_ASSERT(sge_renderer_object_new_watch == NULL);
	sge_renderer_object_new_watch = sge_object_add_new_watch(sge_renderer_object_new, NULL);

	return true;

}

void sge_renderer_shutdown(void)
{
	SGE_ASSERT(sge_renderer_object_new_watch != NULL);
	sge_object_remove_new_watch(sge_renderer_object_new_watch);
	sge_renderer_object_new_watch = NULL;

	SGE_ASSERT(sge_renderer_glex != NULL);
	glexDeleteContext(sge_renderer_glex);
	sge_renderer_glex = NULL;
}

void sge_renderer_draw(float elapsed)
{
	SGE_ASSERT(sge_renderer_glex != NULL);

	glexMakeCurrent(sge_renderer_glex);
	glexBeginFrame();

	// TODO

	glexEndFrame();
}
