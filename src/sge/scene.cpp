/*
 *
 */
#include <btBulletDynamicsCommon.h>

#include <cx/list.h>

#include <sge/scene.h>

typedef struct {
	GLEXMesh *meshes;
	GLEXMaterial *materials;
	int count;
} sge_object_surface_t;

struct sge_object {
	cx_list_node_t node_enable;
	cx_list_node_t node_visible;
	char *name;
	int flags;
	hmm_vec3 pos;
	hmm_vec3 scale;
	hmm_quaternion rotation;
	sge_object_surface_t surface;
	float mass;
};

static btDefaultCollisionConfiguration *sge_bt_cc;
static btCollisionDispatcher *sge_bt_cd;
static btBroadphaseInterface *sge_bt_bi;
static btSequentialImpulseConstraintSolver *sge_bt_sics;
static btDiscreteDynamicsWorld *sge_bt_world;

static cx_list_t sge_object_list;
static hmm_mat4 sge_camera_transform;
static hmm_mat4 sge_camera_projection;

static bool sge_bt_init(void)
{
	sge_bt_cc = new btDefaultCollisionConfiguration();
	if (sge_bt_cc == NULL)
		goto bad0;

	sge_bt_cd = new btCollisionDispatcher(sge_bt_cc);
	if (sge_bt_cd == NULL)
		goto bad1;

	sge_bt_bi = new btDbvtBroadphase();
	if (sge_bt_bi == NULL)
		goto bad2;

	sge_bt_sics = new btSequentialImpulseConstraintSolver();
	if (sge_bt_sics == NULL)
		goto bad3;

	sge_bt_world = new btDiscreteDynamicsWorld(
		sge_bt_cd,
		sge_bt_bi,
		sge_bt_sics,
		sge_bt_cc);

	if (sge_bt_world == NULL)
		goto bad4;

	sge_bt_world->setGravity(btVector3(0, -9.8f, 0));

	return 0;

bad4:
	delete sge_bt_sics;

bad3:
	delete sge_bt_bi;

bad2:
	delete sge_bt_cd;

bad1:
	delete sge_bt_cc;

bad0:
	return -1;
}

static void sge_bt_shutdown(void)
{
	delete sge_bt_world;
	delete sge_bt_sics;
	delete sge_bt_bi;
	delete sge_bt_cd;
	delete sge_bt_cc;
}

static void sge_bt_update(float elapsed)
{
	if (sge_bt_world != NULL) {
		sge_bt_world->stepSimulation(elapsed);
		/* TODO */
	}
}

extern "C" sge_object_t *sge_object_new(const char *name)
{
}

extern "C" void sge_object_destroy(sge_object_t *obj)
{
}

extern "C" bool sge_object_is_enabled(sge_object_t *obj)
{
}

extern "C" void sge_object_enable(sge_object_t *obj)
{
}

extern "C" void sge_object_disable(sge_object_t *obj)
{
}

extern "C" bool sge_object_is_visibled(sge_object_t *obj)
{
}

extern "C" void sge_object_show(sge_object_t *obj)
{
}

extern "C" void sge_object_hide(sge_object_t *obj)
{
}

extern "C" const char *sge_object_get_name(sge_object_t *obj)
{
}

extern "C" void sge_object_set_name(sge_object_t *obj, const char *name)
{
}

extern "C" const hmm_vec3 *sge_object_get_pos(sge_object_t *obj)
{
}

extern "C" void sge_object_set_pos(sge_object_t *obj, float x, float y, float z)
{
}

extern "C" const hmm_vec3 *sge_object_get_scale(sge_object_t *obj)
{
}

extern "C" void sge_object_set_scale(sge_object_t *obj, float x, float y, float z)
{
}

extern "C" const hmm_quaternion *sge_object_get_rotation(sge_object_t *obj)
{
}

extern "C" void sge_object_set_rotation(sge_object_t *obj, float angle, float x, float y, float z)
{
}

extern "C" void sge_object_set_surface(sge_object_t *obj, const char *res_path)
{
	// Load from res
}

extern "C" const hmm_vec3 *sge_camera_get_pos(void)
{
}

extern "C" void sge_camera_set_pos(float x, float y, float z)
{
}

extern "C" const hmm_vec3 *sge_camera_get_scale(void)
{
}

extern "C" void sge_camera_set_get_scale(float x, float y, float z)
{
}

extern "C" const hmm_quaternion *sge_camera_rotation(void)
{
}

extern "C" void sge_camera_set_rotation(float angle, float x, float y, float z)
{
}

extern "C" const hmm_mat4 *sge_camera_get_projection(void)
{
}

extern "C" void sge_camera_set_projection(const hmm_mat4 *p)
{
}

extern "C" bool sge_scene_init(void)
{
	if (!sge_bt_init())
		goto bad0;

	cx_list_reset(&sge_object_list);

	// sge_camera_transform = HMM_LookAt
	// sge_camera_projection = HMM_Per

	return true;

bad0:
	return false;
}

extern "C" void sge_scene_shutdown(void)
{
	sge_bt_shutdown();
}

extern "C" void sge_scene_update(float elapsed)
{
	sge_bt_update(elapsed);
}

extern "C" void sge_scene_draw_glex(void)
{
	glexViewMatrix((const GLfloat *)sge_camera_transform.Elements);
	glexProjectionMatrix((const GLfloat *)sge_camera_projection.Elements);

	/* TODO draw all visibled objects here. */
}

