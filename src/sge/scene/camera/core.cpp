//
//
#include <sge/renderer.hpp>
#include <sge/scene/camera.hpp>

SGE_SCENE_CAMERA_BEGIN

static glm::vec3 s_pos;
static glm::vec3 s_direction;
static glm::vec3 s_up;
static glm::vec2 s_rotate;
static float s_fov;
static float s_move_speed;

bool init(void)
{
	s_pos = glm::vec3(0.0f, 0.0f, 0.0f);
	s_direction = glm::vec3(0.0f, 0.0f, -1.0f);
	s_up = glm::vec3(0.0f, 1.0f, 0.0f);
	s_rotate = glm::vec2(0.0f, 0.0f);

	set_fov(90.0f);
	set_move_speed(0.1f);

	return true;
}

void shutdown(void)
{

}

void reset(void)
{

}

bool load(void)
{
	return true;
}

void update(void)
{
	glm::vec2 size = renderer::window_size();
	renderer::set_projection_matrix(glm::perspective(s_fov, size.x / size.y, 0.1f, 999.0f));
	renderer::set_view_matrix(glm::lookAt(s_pos, s_pos + s_direction, s_up));
}

void mouse_look(float dx, float dy)
{
	s_rotate.x += dx;
	s_rotate.y -= dy;

	s_rotate.y = glm::clamp(s_rotate.y, -89.0f, 98.0f);

	glm::vec3 direction;

	direction.x = glm::cos(glm::radians(s_rotate.x)) * glm::cos(glm::radians(s_rotate.y));
	direction.y = glm::sin(glm::radians(s_rotate.y));
	direction.z = glm::sin(glm::radians(s_rotate.x)) * glm::cos(glm::radians(s_rotate.y));

	s_direction = glm::normalize(direction);
}

float move_speed(void)
{
	return s_move_speed;
}

void set_move_speed(float v)
{
	s_move_speed = v;
}

void move_forward(float elapsed)
{
	float d = s_move_speed * elapsed;
	s_pos += d * s_direction;
}

void move_backward(float elapsed)
{
	float d = s_move_speed * elapsed;
	s_pos -= d * s_direction;
}

void move_left(float elapsed)
{
	float d = s_move_speed * elapsed;
	s_pos -= d * glm::normalize(glm::cross(s_direction, s_up));
}

void move_right(float elapsed)
{
	float d = s_move_speed * elapsed;
	s_pos += d * glm::normalize(glm::cross(s_direction, s_up));
}

float fov(void)
{
	return s_fov;
}

void set_fov(float v)
{
	s_fov = glm::clamp(v, 1.0f, 170.0f);
}

SGE_SCENE_CAMERA_END

