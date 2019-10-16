//
//
#include <sge/view.hpp>

SGE_BEGIN

View::View(void)
	: m_glex(NULL)
{
}

View::~View(void)
{
	if (m_glex != NULL)
		shutdown();
}

bool View::init(GL3WGetProcAddressProc proc)
{
	SGE_ASSERT(proc != NULL);
	SGE_ASSERT(m_glex == NULL);

	m_glex = glexCreateContext(proc);
	if (m_glex == NULL)
		return false;

	glexMakeCurrent(m_glex);
	// TODO

	return true;
}

void View::shutdown(void)
{
	SGE_ASSERT(m_glex != NULL);

	glexDeleteContext(m_glex);
	m_glex = NULL;
}

void View::beginFrame(void)
{
	SGE_ASSERT(m_glex != NULL);

	glexMakeCurrent(m_glex);
	glexViewport(m_viewport[0], m_viewport[1], m_viewport[2], m_viewport[3]);
	glexBeginFrame();
}

void View::endFrame(void)
{
	SGE_ASSERT(m_glex != NULL);
	SGE_ASSERT(m_glex == glexGetCurrentContext());

	glexEndFrame();
}

SGE_END

