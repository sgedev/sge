//
//
#include <sge/gl/view.hpp>

SGE_GL_BEGIN

view::view(void)
	: m_owner(NULL)
	, m_enabled(true)
	, m_viewport(0, 0, -1, -1)
	, m_clear_enabled(true)
	, m_clear_color(0.0f, 0.0f, 0.0f)
{
}

view::~view(void)
{
	if (m_owner != NULL)
		shutdown();
}

bool view::init(void)
{
	SGE_ASSERT(m_owner == NULL);

	m_owner = context::get_current();
	if (m_owner == NULL)
		return false;

	return true;
}

void view::shutdown(void)
{
	SGE_ASSERT(m_owner != NULL);

	m_owner = NULL;
}

void view::update(void)
{
	SGE_ASSERT(m_owner != NULL);
	SGE_ASSERT(m_owner == context::get_current());

	if (!m_enabled)
		return;

	glViewport(m_viewport[0], m_viewport[1], m_viewport[2], m_viewport[3]);

	if (m_clear_enabled) {
		glScissor(m_viewport[0], m_viewport[1], m_viewport[2], m_viewport[3]);
		glEnable(GL_SCISSOR_TEST);
		glClearColor(m_clear_color.r, m_clear_color.g, m_clear_color.b, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	// TODO
}

SGE_GL_END

