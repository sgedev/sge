//
//
#include <sge/scene/camera.hpp>

SGE_SCENE_BEGIN

camera::camera(void)
{
}

camera::~camera(void)
{
}

bool camera::create(void)
{
	reset();

	return true;
}

void camera::destroy(void)
{

}

void camera::reset(void)
{
	m_pos = glm::vec3(0.0f, 0.0f, 0.0f);
	m_direction = glm::vec3(0.0f, 0.0f, -1.0f);
	m_up = glm::vec3(0.0f, 1.0f, 0.0f);
	m_rotate = glm::vec2(0.0f, 0.0f);

	set_fov(45.0f);
	set_move_speed(0.1f);
}

bool camera::load(db::node node)
{
	return true;
}

void camera::update(renderer::view &view)
{
#if 0
	glm::vec2 size = window_size();
	renderer::set_projection_matrix(glm::perspective(s_fov, size.x / size.y, 0.1f, 999.0f));
	renderer::set_view_matrix(glm::lookAt(s_pos, s_pos + s_direction, s_up));
#endif
}

void camera::mouse_look(float dx, float dy)
{
#if 0
	s_rotate.x += dx;
	s_rotate.y -= dy;

	s_rotate.y = glm::clamp(s_rotate.y, -89.0f, 98.0f);

	glm::vec3 direction;

	direction.x = glm::cos(glm::radians(s_rotate.x)) * glm::cos(glm::radians(s_rotate.y));
	direction.y = glm::sin(glm::radians(s_rotate.y));
	direction.z = glm::sin(glm::radians(s_rotate.x)) * glm::cos(glm::radians(s_rotate.y));

	s_direction = glm::normalize(direction);
#endif
}

float camera::move_speed(void)
{
	return m_move_speed;
}

void camera::set_move_speed(float v)
{
	m_move_speed = v;
}

void camera::move_forward(void)
{
	float d = m_move_speed * elapsed();
	m_pos += d * m_direction;
}

void camera::move_backward(void)
{
	float d = m_move_speed * elapsed();
	m_pos -= d * m_direction;
}

void camera::move_left(void)
{
	float d = m_move_speed * elapsed();
	m_pos -= d * glm::normalize(glm::cross(m_direction, m_up));
}

void camera::move_right(void)
{
	float d = m_move_speed * elapsed();
	m_pos += d * glm::normalize(glm::cross(m_direction, m_up));
}

float camera::fov(void)
{
	return m_fov;
}

void camera::set_fov(float v)
{
	m_fov = glm::clamp(v, 1.0f, 170.0f);
}

SGE_SCENE_END
