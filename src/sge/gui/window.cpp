//
//
#include <imgui.h>

#include <sge/gui/window.hpp>

SGE_GUI_BEGIN

window::window(void)
	: m_rect(0, 0, 0, 0)
	, m_flags(0)
	, m_imgui_flags(0)
	, m_internal_flags(0)
	, m_created(false)
{
}

window::~window(void)
{
	if (m_created)
		destroy();
}

bool window::create(const char *name, int x, int y, int width, int height, window *parent, int flags)
{
	SGE_ASSERT(!m_created);

	m_name = name;

	m_rect[0] = x;
	m_rect[1] = y;
	m_rect[2] = width;
	m_rect[3] = height;

	set_flags(flags);
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

void window::update(void)
{
	SGE_ASSERT(m_created);

	if (!(m_internal_flags & INTERNAL_FLAG_VISIBLED))
		return;

	ImGui::SetNextWindowPos(ImVec2(m_rect[0], m_rect[1]), ImGuiSetCond_Once);
	ImGui::SetNextWindowSize(ImVec2(m_rect[2], m_rect[3]), ImGuiSetCond_Once);

	if (parent() == NULL)
		ImGui::Begin(m_name.c_str(), NULL, m_imgui_flags);
	else
		ImGui::BeginChild(m_name.c_str());

	node::update();

	ImVec2 pos = ImGui::GetWindowPos();
	ImVec2 size = ImGui::GetWindowSize();

	if (parent() == NULL)
		ImGui::End();
	else
		ImGui::EndChild();
}

void window::set_flags(int flags)
{
	m_flags = flags;
	m_imgui_flags = 0;
	m_internal_flags = 0;

	if (flags & FLAG_NO_TITLE)
		m_imgui_flags |= ImGuiWindowFlags_NoTitleBar;
	if (flags & FLAG_NO_MOVE)
		m_imgui_flags |= ImGuiWindowFlags_NoMove;
	if (flags & FLAG_NO_RESIZE)
		m_imgui_flags |= ImGuiWindowFlags_NoResize;
	if (flags & FLAG_NO_SCROLL)
		m_imgui_flags |= ImGuiWindowFlags_NoScrollbar;
	if (flags & FLAG_NO_COLLAPSE)
		m_imgui_flags |= ImGuiWindowFlags_NoCollapse;
	if (flags & FLAG_NO_BACKGROUND)
		m_imgui_flags |= ImGuiWindowFlags_NoBackground;
	if (flags & FLAG_MENUBAR)
		m_imgui_flags |= ImGuiWindowFlags_MenuBar;
	if (flags & FLAG_NO_FRONT)
		m_imgui_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
	if (flags & FLAG_VISIBLED)
		m_internal_flags |= INTERNAL_FLAG_VISIBLED;
}

SGE_GUI_END

