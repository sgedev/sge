//
//
#include <sge/entity.hpp>

SGE_BEGIN

Entity::Entity(void)
	: m_dirty(false)
{
	setTransform(glm::mat4(1.0f));
}

Entity::Entity(const Entity &that)
	: m_dirty(that.m_dirty)
	, m_name(that.m_name)
	// TODO
{

}

Entity::~Entity(void)
{
}

SGE_END

