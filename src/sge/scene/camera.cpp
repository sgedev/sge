//
//
#include <sge/scene/camera.hpp>

SGE_SCENE_CAMERA_BEGIN

static bool s_drawing;
static glm::mat4 s_view_matrix;
static glm::mat4 s_project_matrix;

bool init(void)
{
	s_drawing = false;

	s_view_matrix = glm::mat4(1.0f);
	s_project_matrix = glm::mat4(0.0f);

	return true;
}

void shutdown(void)
{
	SGE_ASSERT(!s_drawing);
}

void ortho(float left, float right, float bottom, float top, float znear, float zfar)
{
	s_project_matrix = glm::orthoLH(left, right, bottom, top, znear, zfar);
}

void frustum(float left, float right, float bottom, float top, float znear, float zfar)
{
	s_project_matrix = glm::frustum(left, right, bottom, top, znear, zfar);
}

void look_at(const glm::vec3 &eye, const glm::vec3 &target, const glm::vec3 &up)
{
	s_project_matrix = glm::lookAt(eye, target, up);
}

void look_to(const glm::vec3 &eye, const glm::vec3 &direction, const glm::vec3 &up)
{
	s_project_matrix = glm::lookAt(eye, eye + direction, up);
}

void reset(void)
{
	SGE_ASSERT(!s_drawing);

	s_drawing = true;
}

void render(void)
{
	SGE_ASSERT(s_drawing);

	s_drawing = false;
}

SGE_SCENE_CAMERA_END

