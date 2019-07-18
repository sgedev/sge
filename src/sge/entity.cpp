//
//
#include <sge/entity.hpp>

SGE_BEGIN

entity::entity(void)
	: m_dirty(false)
{
	set_transform(glm::mat4(1.0f));
}

entity::~entity(void)
{
}

SGE_END

