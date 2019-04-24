//
//
#include <imgui.h>

#include <sge/gui/window.hpp>

SGE_GUI_BEGIN

window::window(void)
	: m_rect(0, 0, 0, 0)
	, m_parent(NULL)
	, m_prev(NULL)
	, m_next(NULL)
	, m_first_child(NULL)
	, m_flags(0)
	, m_created(false)
{
}

window::~window(void)
{
	if (m_created)
		destroy();
}

bool window::create(const char *name, int x, int y, int width, int height, int flags, window *parent)
{
	SGE_ASSERT(!m_created);

	m_name = name;
	m_flags = flags;

	m_rect[0] = x;
	m_rect[1] = y;
	m_rect[2] = width;
	m_rect[3] = height;

	set_parent(parent);

	m_created = true;

	return true;
}

void window::destroy(void)
{
	SGE_ASSERT(m_created);

	set_parent(NULL);

	m_created = false;
}

void window::update(float elapsed)
{
	SGE_ASSERT(m_created);

	ImGui::BeginChild(m_name.c_str());

	ImGui::SetWindowPos(ImVec2(m_rect[0], m_rect[1]));
	ImGui::SetWindowSize(ImVec2(m_rect[2], m_rect[3]));

	for (window *child = m_first_child; child != NULL; child = child->m_next)
		child->update(elapsed);

	ImGui::EndChild();
}

void window::set_parent(window *parent)
{
	if (m_parent != NULL) {
		if (m_prev == NULL) {
			SGE_ASSERT(m_parent->m_first_child == this);
			m_parent->m_first_child = m_next;
		} else
			m_prev->m_next = m_next;
		if (m_next != NULL)
			m_next->m_prev = m_prev;
	}

	m_parent = parent;

	if (m_parent != NULL) {
		m_prev = NULL;
		m_next = m_parent->m_first_child;
		m_parent->m_first_child = this;
	}
}

SGE_GUI_END

