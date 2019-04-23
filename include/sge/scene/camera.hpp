//
//
#ifndef SGE_SCENE_CAMERA_HPP
#define SGE_SCENE_CAMERA_HPP

#include <sge/gl.hpp>
#include <sge/scene/common.hpp>
#include <sge/scene/node.hpp>

SGE_SCENE_BEGIN

class camera: public node {
public:
	camera(void);
	virtual ~camera(void);

public:
	bool init(void);
	void shutdown(void);
	void update(void);
	void enable_background(void);
	void disable_background(void);
	void set_background(float red, float green, float blue);
	void set_background(const glm::vec3 &v);
	void ortho(float left, float right, float bottom, float top, float znear, float zfar);
	void frustum(float left, float right, float bottom, float top, float znear, float nfar);
	void look_at(const glm::vec3 &eye, const glm::vec3 &target, const glm::vec3 &up);
	void look_to(const glm::vec3 &eye, const glm::vec3 &direction, const glm::vec3 &up);

private:
	gl::view m_view;
};

inline void camera::enable_background(void)
{
	m_view.enable_clear();
}

inline void camera::disable_background(void)
{
	m_view.disable_clear();
}

inline void camera::set_background(float red, float green, float blue)
{
	m_view.set_clear_color(red, green, blue);
}

inline void camera::set_background(const glm::vec3 &v)
{
	m_view.set_clear_color(v);
}

inline void camera::ortho(float left, float right, float bottom, float top, float znear, float zfar)
{
	m_view.set_projection(glm::orthoLH(left, right, bottom, top, znear, zfar));
}

inline void camera::frustum(float left, float right, float bottom, float top, float znear, float zfar)
{
	m_view.set_projection(glm::frustum(left, right, bottom, top, znear, zfar));
}

inline void camera::look_at(const glm::vec3 &eye, const glm::vec3 &target, const glm::vec3 &up)
{
	m_view.set_projection(glm::lookAtLH(eye, target, up));
}

inline void camera::look_to(const glm::vec3 &eye, const glm::vec3 &direction, const glm::vec3 &up)
{
	m_view.set_projection(glm::lookAtLH(eye, eye + direction, up));
}

SGE_SCENE_END

#endif // SGE_SCENE_CAMERA_HPP

