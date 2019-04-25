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

	m_flags = 0;
	m_internal_flags = 0;

	if (flags & FLAG_NO_TITLE)
		m_flags |= ImGuiWindowFlags_NoTitleBar;
	if (flags & FLAG_NO_MOVE)
		m_flags |= ImGuiWindowFlags_NoMove;
	if (flags & FLAG_NO_RESIZE)
		m_flags |= ImGuiWindowFlags_NoResize;
	if (flags & FLAG_NO_SCROLL)
		m_flags |= ImGuiWindowFlags_NoScrollbar;
	if (flags & FLAG_NO_COLLAPSE)
		m_flags |= ImGuiWindowFlags_NoCollapse;
	if (flags & FLAG_NO_BACKGROUND)
		m_flags |= ImGuiWindowFlags_NoBackground;
	if (flags & FLAG_MENUBAR)
		m_flags |= ImGuiWindowFlags_MenuBar;
	if (flags & FLAG_NO_FRONT)
		m_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
	if (flags & FLAG_VISIBLED)
		m_internal_flags |= INTERNAL_FLAG_VISIBLED;

	m_rect[0] = x;
	m_rect[1] = y;
	m_rect[2] = width;
	m_rect[3] = height;

	set_parent(parent);

	m_created = true;

	if (!do_create()) {
		set_parent(NULL);
		return false;
	}

	return true;
}

void window::destroy(void)
{
	SGE_ASSERT(m_created);

	do_destroy();

	set_parent(NULL);

	m_created = false;
}

void window::update(float elapsed)
{
	SGE_ASSERT(m_created);

	if (!(m_internal_flags & INTERNAL_FLAG_VISIBLED))
		return;

	ImGui::BeginChild(m_name.c_str(), ImVec2(m_rect[2], m_rect[3]), false, m_flags);
	ImGui::SetWindowPos(ImVec2(m_rect[0], m_rect[1]));

	do_update(elapsed);

	for (window *child = m_first_child; child != NULL; child = child->m_next)
		child->update(elapsed);

	ImGui::EndChild();
}

bool window::do_create(void)
{
	return true;
}

void window::do_destroy(void)
{
}

void window::do_update(float elapsed)
{
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

