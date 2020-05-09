//
//
#include <SGL/Camera.hpp>

SGL_BEGIN

Camera::Camera(void)
{
}

Camera::~Camera(void)
{
}

bool Camera::init(void)
{
}

void Camera::clear(void)
{
}

void Camera::render(void);
void Camera::lookAt(const glm::vec3 &pos, const glm::vec3 &front, const glm::vec3 up);
void Camera::ortho(float left, float right, float bottom, float top);
void Camera::perspective(float fovy, float aspect, float zNear, float zFar);

SGL_END

#endif // SGL_CAMERA_HPP

