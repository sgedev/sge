//
//
#ifndef SGL_CAMERA_HPP
#define SGL_CAMERA_HPP

#include <SGL/Common.hpp>

SGL_BEGIN

class Camera {
public:
	Camera(void);
	virtual ~Camera(void);

public:
	const glm::ivec4 &viewport(void) const;
	void setViewport(int x, int y, int width, int height);
	void lookAt(const glm::vec3 &pos, const glm::vec3 &front, const glm::vec3 up);
	void ortho(float left, float right, float bottom, float top);
	void perspective(float fovy, float aspect, float zNear, float zFar);

private:
	glm::ivec4 m_viewport;
	glm::vec3 m_pos;
	glm::vec3 m_front;
	glm::vec3 m_up;
	float m_fov;
	float m_aspect;
	float m_zNear;
};

SGL_END

#endif // SGL_CAMERA_HPP

