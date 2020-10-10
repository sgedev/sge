//
//
#include <sge/graphics/window.hpp>

SGE_GRAPHICS_BEGIN

window::window(void):
	m_handle(nullptr),
	m_gl_context(nullptr)
{
}

window::~window(void)
{
	if (m_gl_context != nullptr)
		SDL_GL_DeleteContext(m_gl_context);

	if (m_handle != nullptr)
		SDL_DestroyWindow(m_handle);
}

bool window::init(const char *name, int x, int y, int width, int height)
{
	SGE_ASSERT(m_handle == nullptr);

	return true;
}

void window::handle_event(const SDL_WindowEvent &evt)
{
	SGE_ASSERT(m_handle != nullptr);

	if (evt.windowID != m_id)
		return;

	switch (evt.event) {
	case SDL_WINDOWEVENT_SHOWN:
		m_visible = true;
		break;
	case SDL_WINDOWEVENT_HIDDEN:
		m_visible = false;
	case SDL_WINDOWEVENT_RESIZED:
		resize(evt.data1, evt.data2);
		break;
	}
}

void window::resize(int width, int height)
{
	// TODO

	canvas::resize(width, height);
}

SGE_GRAPHICS_END
