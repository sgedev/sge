//
//
#include <sge/gui/node.hpp>

SGE_GUI_BEGIN

node *node::c_first_node = NULL;

node::node(void)
	: m_parent(NULL)
	, m_prev(NULL)
	, m_next(NULL)
	, m_first_child(NULL)
{
}

node::~node(void)
{
	detach();
}

void node::set_parent(node *parent)
{
	detach();

	m_parent = parent;

	if (m_parent != NULL) {
		m_next = m_parent->m_first_child;
		m_parent->m_first_child = this;
	} else {
		m_next = c_first_node;
		c_first_node = this;
	}
}

void node::update(void)
{
	for (node *p = m_first_child; p != NULL; p = p->m_next)
		p->update();
}

void node::update_all(void)
{
	for (node *p = c_first_node; p != NULL; p = p->m_next)
		p->update();
}

void node::detach(void)
{
	if (m_prev == NULL) {
		if (m_parent != NULL) {
			SGE_ASSERT(m_parent->m_first_child == this);
			m_parent->m_first_child = m_next;
			m_parent = NULL;
		} else {
			if (c_first_node == this)
				c_first_node = m_next;
		}
	} else
		m_prev->m_next = m_next;

	if (m_next != NULL)
		m_next->m_prev = m_prev;

	m_prev = NULL;
	m_next = NULL;
}

SGE_GUI_END

