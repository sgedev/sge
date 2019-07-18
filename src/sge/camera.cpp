//
//
#include <sge/camera.hpp>

SGE_BEGIN

camera::camera(void)
	: m_move_speed(1.0f)
	, m_mouse_sensitivity(1.0f)
	, m_eye(0.0f, 0.0f, 0.0f)
	, m_center(0.0f, 0.0f, -1.0f)
	, m_up(0.0f, 1.0f, 0.0f)
{
}

camera::~camera(void)
{
}

void camera::clear(void)
{
	
}

void camera::move_forward(void)
{
	m_eye += m_move_speed * m_center;
	look_at(m_eye, m_center, m_up);
}

void camera::move_backward(void)
{
	m_eye -= m_move_speed * m_center;
	look_at(m_eye, m_center, m_up);
}

void camera::move_left(void)
{
	m_eye -= glm::normalize(glm::cross(m_center, m_up)) * m_move_speed;
	look_at(m_eye, m_center, m_up);
}

void camera::move_right(void)
{
	m_eye += glm::normalize(glm::cross(m_center, m_up)) * m_move_speed;
	look_at(m_eye, m_center, m_up);
}

void camera::mouse_view(int dx, int dy)
{
	float xoffset = dx;
	float yoffset = dy;

	xoffset *= m_mouse_sensitivity;
	yoffset *= m_mouse_sensitivity;
}

SGE_END

