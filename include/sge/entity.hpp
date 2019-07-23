//
//
#ifndef SGE_ENTITY_HPP
#define SGE_ENTITY_HPP

#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/matrix_decompose.hpp>

#include <sge/common.hpp>

SGE_BEGIN

class entity {
public:
	entity(void);
	entity(const entity &that);
	virtual ~entity(void);

public:
	const char *name(void) const;
	void rename(const char *name);
	const glm::vec3 &scale(void) const;
	void scaling(const glm::vec3 &xyz);
	const glm::vec3 &position(void) const;
	void move(const glm::vec3 &xyz);
	void move_to(const glm::vec3 &xyz);
	const glm::quat &rotation(void) const;
	void rotate(float angle, const glm::vec3 &axis);
	const glm::mat4 &transform(void);
	void set_transform(const glm::mat4 &v);

private:
	glm::vec3 m_scale;
	glm::vec3 m_position;
	glm::quat m_rotation;
	glm::mat4 m_transform;
	bool m_dirty;
	std::string m_name;
};

inline const char *entity::name(void) const
{
	return m_name.c_str();
}

inline void entity::rename(const char *name)
{
	m_name = name;
}

inline const glm::vec3 &entity::scale(void) const
{
	return m_scale;
}

inline void entity::scaling(const glm::vec3 &xyz)
{
	m_scale = xyz;
	m_dirty = true;
}

inline const glm::vec3 &entity::position(void) const
{
	return m_position;
}

inline void entity::move(const glm::vec3 &xyz)
{
	m_position = xyz;
	m_dirty = true;
}

inline void entity::move_to(const glm::vec3 &xyz)
{
	m_position += xyz;
	m_dirty = true;
}

inline const glm::quat &entity::rotation(void) const
{
	return m_rotation;
}

inline void entity::rotate(float angle, const glm::vec3 &axis)
{
}

inline const glm::mat4 &entity::transform(void)
{
	if (m_dirty) {
		// TODO
		m_dirty = false;
	}

	return m_transform;
}

inline void entity::set_transform(const glm::mat4 &v)
{
	m_transform = v;
	glm::vec3 skew;
	glm::vec4 perspective;
	glm::decompose(m_transform, m_scale, m_rotation, m_position, skew, perspective);
}

SGE_END

#endif // SGE_ENTITY_HPP

