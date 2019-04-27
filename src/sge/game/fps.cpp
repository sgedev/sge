//
//
#include <sge/gl.hpp>
#include <sge/scene.hpp>
#include <sge/game/fps.hpp>

SGE_GAME_FPS_BEGIN

static glm::vec3 s_camera_pos;
static glm::vec3 s_camera_direction;
static glm::vec3 s_camera_up;
static glm::vec2 s_camera_rotate;
static float s_camera_fov;
static float s_camera_move_speed;
static float s_mouse_sensitivity;

bool init(void)
{
	s_camera_pos = glm::vec3(0.0f, 0.0f, 0.0f);
	s_camera_direction = glm::vec3(0.0f, 0.0f, -1.0f);
	s_camera_up = glm::vec3(0.0f, 1.0f, 0.0f);
	s_camera_rotate = glm::vec2(0.0f, 0.0f);

	set_fov(90.0f);
	set_move_speed(0.1f);
	set_mouse_sensitivity(0.3f);

	update();
	
	return true;
}

void shutdown(void)
{
}

void update(void)
{
	glm::vec2 size = gl::window_size();

	scene::camera::set_projection(
		glm::perspective(glm::radians(s_camera_fov), size.x / size.y, 0.1f, 100.0f));

	scene::camera::set_transform(
		glm::lookAt(s_camera_pos, s_camera_pos + s_camera_direction, s_camera_up));
}

float mouse_sensitivity(void)
{
	return s_mouse_sensitivity;
}

void set_mouse_sensitivity(float v)
{
	s_mouse_sensitivity = glm::clamp(v, 0.001f, 30.0f);
}

void mouse_look(float dx, float dy)
{
	float xoffset = dx * s_mouse_sensitivity;
	float yoffset = dy * s_mouse_sensitivity;

	s_camera_rotate.x += xoffset;
	s_camera_rotate.y -= yoffset;

	s_camera_rotate.y = glm::clamp(s_camera_rotate.y, -89.0f, 98.0f);

	glm::vec3 direction;

	direction.x = glm::cos(glm::radians(s_camera_rotate.x)) * glm::cos(glm::radians(s_camera_rotate.y));
	direction.y = glm::sin(glm::radians(s_camera_rotate.y));
	direction.z = glm::sin(glm::radians(s_camera_rotate.x)) * glm::cos(glm::radians(s_camera_rotate.y));

	s_camera_direction = glm::normalize(direction);
}

float move_speed(void)
{
	return s_camera_move_speed;
}

void set_move_speed(float v)
{
	s_camera_move_speed = v;
}

void move_forward(void)
{
	float d = s_camera_move_speed * elapsed();
	s_camera_pos += d * s_camera_direction;
}

void move_backward(void)
{
	float d = s_camera_move_speed * elapsed();
	s_camera_pos -= d * s_camera_direction;
}

void move_left(void)
{
	float d = s_camera_move_speed * elapsed();
	s_camera_pos -= d * glm::normalize(glm::cross(s_camera_direction, s_camera_up));
}

void move_right(void)
{
	float d = s_camera_move_speed * elapsed();
	s_camera_pos += d * glm::normalize(glm::cross(s_camera_direction, s_camera_up));
}

float fov(void)
{
	return s_camera_fov;
}

void set_fov(float v)
{
	s_camera_fov = glm::clamp(v, 1.0f, 170.0f);
}

SGE_GAME_FPS_END
