//
//
#include <sge/entity.hpp>

SGE_BEGIN

entity::entity(void)
	: m_dirty(false)
{
	set_transform(glm::mat4(1.0f));
}

entity::entity(const entity &that)
	: m_dirty(that.m_dirty)
	, m_name(that.m_name)
	// TODO
{

}

entity::~entity(void)
{
}

SGE_END

