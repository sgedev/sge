//
//
#include <sge/renderer/view.hpp>

SGE_RENDERER_BEGIN

view::view(void)
	: m_enabled(true)
	, m_clear(false)
{
}

view::~view(void)
{
}

bool view::init(void)
{
	return true;
}

void view::shutdown(void)
{
}

void view::draw(void)
{
	SGE_ASSERT(SDL_GL_GetCurrentContext() != NULL);

	if (!is_enabled())
		return;

	glViewport(m_viewport[0], m_viewport[1], m_viewport[2], m_viewport[3]);

	if (m_clear) {
		glClearColor(m_background.r, m_background.g, m_background.b, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
	}
}

SGE_RENDERER_END

