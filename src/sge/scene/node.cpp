//
//
#include <sge/scene/node.hpp>

SGE_SCENE_BEGIN

node::node(node *parent)
	: m_parent(NULL)
	, m_prev_sibling(NULL)
	, m_next_sibling(NULL)
	, m_first_child(NULL)
	, m_position(0.0f, 0.0f, 0.0f)
	, m_scale(1.0f, 1.0f, 1.0f)
	, m_transform(1.0f)
{
	set_parent(parent);
}

node::~node(void)
{
}

void node::set_parent(node *parent)
{
	if (m_parent == parent)
		return;

	unlink();

	m_parent = parent;
	if (m_parent == NULL)
		return;

	m_prev_sibling = NULL;
	m_next_sibling = m_parent->m_first_child;
	m_parent->m_first_child = this;
}

void node::unlink(void)
{
	if (m_parent == NULL)
		return;

	if (m_prev_sibling == NULL) {
		if (m_parent != NULL) {
			SGE_ASSERT(m_parent->m_first_child == this);
			m_parent->m_first_child = m_next_sibling;
		}
	} else
		m_prev_sibling->m_next_sibling = m_next_sibling;

	if (m_next_sibling != NULL)
		m_next_sibling->m_prev_sibling = m_prev_sibling;
}

SGE_SCENE_END

