//
//
#ifndef SGE_ENTITY_HPP
#define SGE_ENTITY_HPP

#include <string>

#include <glm.hpp>

#include <sge/common.hpp>

SGE_BEGIN

class Entity {
public:
	Entity(void);
	Entity(const Entity &that);
	virtual ~Entity(void);

public:
	const char *name(void) const;
	void rename(const char *name);
	const glm::vec3 &scale(void) const;
	void scaling(const glm::vec3 &xyz);
	const glm::vec3 &position(void) const;
	void move(const glm::vec3 &xyz);
	void moveTo(const glm::vec3 &xyz);
	const glm::quat &rotation(void) const;
	void rotate(float angle, const glm::vec3 &axis);
	const glm::mat4 &transform(void);
	void setTransform(const glm::mat4 &v);

private:
	glm::vec3 m_scale;
	glm::vec3 m_position;
	glm::quat m_rotation;
	glm::mat4 m_transform;
	bool m_dirty;
	std::string m_name;
};

inline const char *Entity::name(void) const
{
	return m_name.c_str();
}

inline void Entity::rename(const char *name)
{
	m_name = name;
}

inline const glm::vec3 &Entity::scale(void) const
{
	return m_scale;
}

inline void Entity::scaling(const glm::vec3 &xyz)
{
	m_scale = xyz;
	m_dirty = true;
}

inline const glm::vec3 &Entity::position(void) const
{
	return m_position;
}

inline void Entity::move(const glm::vec3 &xyz)
{
	m_position = xyz;
	m_dirty = true;
}

inline void Entity::moveTo(const glm::vec3 &xyz)
{
	m_position += xyz;
	m_dirty = true;
}

inline const glm::quat &Entity::rotation(void) const
{
	return m_rotation;
}

inline void Entity::rotate(float angle, const glm::vec3 &axis)
{
}

inline const glm::mat4 &Entity::transform(void)
{
	if (m_dirty) {
		// TODO
		m_dirty = false;
	}

	return m_transform;
}

inline void Entity::setTransform(const glm::mat4 &v)
{
	m_transform = v;
	glm::vec3 skew;
	glm::vec4 perspective;
	glm::decompose(m_transform, m_scale, m_rotation, m_position, skew, perspective);
}

SGE_END

#endif // SGE_ENTITY_HPP

