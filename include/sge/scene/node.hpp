//
//
#ifndef SGE_SCENE_NODE_HPP
#define SGE_SCENE_NODE_HPP

#include <string>

#include <sge/scene/common.hpp>

SGE_SCENE_BEGIN

class node {
public:
	node(void);
	virtual ~node(void);

public:
	void move(float x, float y, float z);
	void move(const glm::vec3 &v);
	void move_to(float x, float y, float z);
	void move_to(const glm::vec3 &v);
	const glm::vec3 &get_position(void) const;
	void scale(float x, float y, float z);
	void scale(const glm::vec3 &v);
	const glm::vec3 &get_scale(void) const;
	void rotate(float angle, float x, float y, float z);
	void rotate(float angle, const glm::vec3 &axis);
	void rotate(const glm::quat &v);
	const glm::quat &get_rotation(void) const;
	const glm::mat4 &get_transform(void);

private:
	std::string m_name;
	glm::vec3 m_position;
	glm::vec3 m_scale;
	glm::quat m_rotation;
	glm::mat4 m_transform;
	bool m_dirty;
};

inline void node::move(float x, float y, float z)
{
	m_position = glm::vec3(x, y, z);
	m_dirty = true;
}

inline void node::move(const glm::vec3 &v)
{
	m_position = v;
	m_dirty = true;
}

inline void node::move_to(float x, float y, float z)
{
	m_position += glm::vec3(x, y, z);
	m_dirty = true;
}

inline void node::move_to(const glm::vec3 &v)
{
	m_position += v;
	m_dirty = true;
}

inline const glm::vec3 &node::get_position(void) const
{
	return m_position;
}

inline void node::scale(float x, float y, float z)
{
	m_scale = glm::vec3(x, y, z);
	m_dirty = true;
}

inline void node::scale(const glm::vec3 &v)
{
	m_scale = v;
	m_dirty = true;
}

inline const glm::vec3 &node::get_scale(void) const
{
	return m_scale;
}

inline void node::rotate(float angle, float x, float y, float z)
{
	m_rotation = glm::angleAxis(angle, glm::vec3(x, y, z));
	m_dirty = true;
}

inline void node::rotate(float angle, const glm::vec3 &axis)
{
	m_rotation = glm::angleAxis(angle, axis);
	m_dirty = true;
}

inline void node::rotate(const glm::quat &v)
{
	m_rotation = v;
	m_dirty = true;
}

inline const glm::quat &node::get_rotation(void) const
{
	return m_rotation;
}

inline const glm::mat4 &node::get_transform(void)
{
	if (m_dirty) {
		m_transform = glm::scale(glm::mat4(1.0f), m_scale);
		m_transform = glm::translate(m_transform, m_position);
		m_transform = glm::rotate(m_transform, glm::angle(m_rotation), glm::axis(m_rotation));
		m_dirty = false;
	}

	return m_transform;
}

SGE_SCENE_END

#endif // SGE_SCENE_NODE_HPP

