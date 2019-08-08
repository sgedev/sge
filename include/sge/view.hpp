//
//
#ifndef SGE_VIEW_HPP
#define SGE_VIEW_HPP

#include <glm/glm.hpp>

#include <sge/common.hpp>
#include <sge/entity.hpp>

SGE_BEGIN

class view: public entity {
public:
	view(void);
	virtual ~view(void);

public:
	void clear(void);
	void look_at(glm::vec3 const &eye, glm::vec3 const &center, glm::vec3 const &up);
	const glm::mat4 &projection(void) const;
	void set_projection(const glm::mat4 &v);
	void ortho(float left, float right, float bottom, float top, float znear, float zfar);
	void frustum(float left, float right, float bottom, float top, float znear, float zfar);
	void perspective(float fovy, float aspect, float znear, float zfar);

private:
	glm::mat4 m_projection;
};

inline void view::look_at(glm::vec3 const &eye, glm::vec3 const &center, glm::vec3 const &up)
{
	set_transform(glm::lookAtLH(eye, center, up));
}

inline const glm::mat4 &view::projection(void) const
{
	return m_projection;
}

inline void view::set_projection(const glm::mat4 &v)
{
	m_projection = v;
}

inline void view::ortho(float left, float right, float bottom, float top, float znear, float zfar)
{
	m_projection = glm::ortho(left, right, bottom, top, znear, zfar);
}

inline void view::frustum(float left, float right, float bottom, float top, float znear, float zfar)
{
	m_projection = glm::frustum(left, right, bottom, top, znear, zfar);
}

inline void view::perspective(float fovy, float aspect, float znear, float zfar)
{
	m_projection = glm::perspectiveLH(fovy, aspect, znear, zfar);
}

SGE_END

#endif // SGE_CAMERA_HPP

