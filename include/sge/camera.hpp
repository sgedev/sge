//
//
#ifndef SGE_CAMERA_HPP
#define SGE_CAMERA_HPP

#include <glm/glm.hpp>

#include <sge/common.hpp>
#include <sge/entity.hpp>

SGE_BEGIN

class camera: public entity {
public:
	camera(void);
	virtual ~camera(void);

public:
	void clear(void);
	void render(void);
	void look_at(glm::vec3 const &eye, glm::vec3 const &center, glm::vec3 const &up);
	const glm::mat4 &projection(void) const;
	void set_projection(const glm::mat4 &v);
	void ortho(float left, float right, float bottom, float top, float znear, float zfar);
	void frustum(float left, float right, float bottom, float top, float znear, float zfar);
	void perspective(float fovy, float aspect, float znear, float zfar);
	void set_fov(float fov);

private:
	glm::mat4 m_projection;
};

inline void camera::look_at(glm::vec3 const &eye, glm::vec3 const &center, glm::vec3 const &up)
{
	set_transform(glm::lookAtLH(eye, center, up));
}

inline const glm::mat4 &camera::projection(void) const
{
	return m_projection;
}

inline void camera::set_projection(const glm::mat4 &v)
{
	m_projection = v;
}

inline void camera::ortho(float left, float right, float bottom, float top, float znear, float zfar)
{
	m_projection = glm::ortho(left, right, bottom, top, znear, zfar);
}

inline void camera::frustum(float left, float right, float bottom, float top, float znear, float zfar)
{
	m_projection = glm::frustum(left, right, bottom, top, znear, zfar);
}

inline void camera::perspective(float fovy, float aspect, float znear, float zfar)
{
	m_projection = glm::perspectiveLH(fovy, aspect, znear, zfar);
}

SGE_END

#endif // SGE_CAMERA_HPP

