//
//
#include <sge/scene/node.hpp>

SGE_SCENE_BEGIN

node::node(void)
	: m_position(0.0f, 0.0f, 0.0f)
	, m_scale(1.0f, 1.0f, 1.0f)
	, m_transform(1.0f)
{
}

node::~node(void)
{
}

SGE_SCENE_END

