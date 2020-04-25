/*
 *
 */
#include <sge/scene.h>

static hmm_mat4 sge_scene_camera_view_matrix;
static hmm_mat4 sge_scene_camera_projection_matrix;

int sge_scene_init(void)
{
    return 0;
}

void sge_scene_shutdown(void)
{
}

void sge_scene_update(float elapsed)
{
}

void sge_scene_draw(void)
{
    glexViewMatrix((const GLfloat *)sge_scene_camera_view_matrix.Elements);
    glexProjectionMatrix((const GLfloat *)sge_scene_camera_projection_matrix.Elements);
}

void sge_scene_set_view_matrix(const hmm_mat4 *m)
{
    memcpy(&sge_scene_camera_view_matrix, m, sizeof(hmm_mat4));
}

void sge_scene_set_projection_matrix(const hmm_mat4 *m)
{
    memcpy(&sge_scene_camera_projection_matrix, m, sizeof(hmm_mat4));
}
