//
//
#include <imgui.h>

#include <sge/gui/widget.hpp>

SGE_GUI_BEGIN

widget *widget::c_tops = NULL;

widget::widget(widget *parent)
	: m_flags(0)
	, m_parent(NULL)
	, m_prev_sibling(NULL)
	, m_next_sibling(NULL)
	, m_first_child(NULL)
{
	m_pos.x = 0;
	m_pos.y = 0;
	m_size.x = 400;
	m_size.y = 300;

	char buf[128];
	snprintf(buf, sizeof(buf), "widget:%p", this);
	m_name = buf;

	set_parent(parent);
}

widget::~widget(void)
{
	detach();
}

void widget::set_parent(widget *p)
{
	if (m_parent != NULL && m_parent == p)
		return;

	detach();

	m_parent = p;

	if (m_parent != NULL) {
		m_prev_sibling = NULL;
		m_next_sibling = m_parent->m_first_child;
		if (m_next_sibling != NULL)
			m_parent->m_first_child = this;
	} else {
		m_prev_sibling = NULL;
		m_next_sibling = c_tops;
		c_tops = this;
	}
}

void widget::update_all(float elapsed)
{
	for (auto p = c_tops; p != NULL; p = p->next_sibling())
		p->update(elapsed);
}

void widget::update(float elapsed)
{
	for (auto p = first_child(); p != NULL; p = p->next_sibling())
		p->update(elapsed);
}

void widget::detach(void)
{
	if (m_parent != NULL) {
		if (m_prev_sibling == NULL) {
			if (m_parent->m_first_child == this)
				m_parent->m_first_child = m_next_sibling;
		} else
			m_prev_sibling->m_next_sibling = m_next_sibling;
	} else {
		if (m_prev_sibling == NULL) {
			if (c_tops == this)
				c_tops = m_next_sibling;
		} else
			m_prev_sibling->m_next_sibling = m_next_sibling;
	}

	if (m_next_sibling != NULL)
		m_next_sibling->m_prev_sibling = m_prev_sibling;

	m_parent = NULL;
	m_prev_sibling = NULL;
	m_next_sibling = NULL;
}

SGE_GUI_END
