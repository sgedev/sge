//
//
#include <SGL/Window.hpp>

SGL_BEGIN

Window::Window(void):
	m_window(NULL)
{
}

Window::~Window(void)
{
	release();
}

bool Window::init(const char *name, int x, int y, int width, int height, int flags)
{
	SGL_ASSERT(m_window == NULL);

	SGE_LOGD("Creating main window...");

	m_window = SDL_CreateWindow("SGE", x, y, width, height, flags);
	if (sge_window == NULL) {
		SGL_LOGE("Failed to create main window.");
		return false;
	}
	
	m_id = SDL_GetWindowID(m_window);

	m_rect[0] = x;
	m_rect[1] = y;
	m_rect[2] = width;
	m_rect[3] = height;
	m_visibled = true;

	return true;
}

bool Window::handleEvent(const SDL_WindowEvent &event)
{
	if (event.windowID != m_id)
		return false;

	switch (event.event) {
	case SDL_WINDOWEVENT_SHOWN:
		m_visibled = true;
		break;
	case SDL_WINDOWEVENT_HIDDEN:
		m_visibled = false;
		break;
	case SDL_WINDOWEVENT_RESIZED:
		m_rect[2] = event->data1;
		m_rect[3] = event->data2;
		break;
	case SDL_WINDOWEVENT_FOCUS_GAINED:
		break;
	case SDL_WINDOWEVENT_FOCUS_LOST:
		break;
	default:
		return false;
	}

	return true;
}

void Window::update(float elapsed)
{
	SGL_ASSERT(m_window != NULL);

	paint();
}

void Window::release(void)
{
	if (m_window != NULL)
		SDL_DestroyWindow(m_window);
}

void Window::paint(void)
{
}

SGL_END

