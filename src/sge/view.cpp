//
//
#include <sge/view.hpp>

SGE_BEGIN

View::View(void)
	: m_glex(NULL)
	, m_nanovg(NULL)
{
}

View::~View(void)
{
	if (m_glex != NULL)
		shutdown();
}

bool View::init(void)
{
	SGE_ASSERT(m_glex == NULL);
	SGE_ASSERT(m_nanovg == NULL);

	m_glex = new GLEX::Context();
	if (m_glex == NULL)
		return false;

	if (!m_glex->init()) {
		delete m_glex;
		m_glex = NULL;
		return false;
	}

	m_nanovg = nvgCreateGL3(0);
	if (m_nanovg == NULL) {
		delete m_glex;
		m_glex = NULL;
		return false;
	}

	return true;
}

void View::shutdown(void)
{
	SGE_ASSERT(m_glex != NULL);
	SGE_ASSERT(m_nanovg != NULL);

	delete m_glex;
	m_glex = NULL;

	nvgDeleteGL3(m_nanovg);
	m_nanovg = NULL;
}

void View::beginFrame(void)
{
	SGE_ASSERT(m_glex != NULL);
	SGE_ASSERT(m_nanovg != NULL);

	//glViewport(m_viewport[0], m_viewport[1], m_viewport[2], m_viewport[3]);

	m_glex->beginFrame();
	//nvgBeginFrame(m_nanovg, float(m_viewport[2]), float(m_viewport[3]), 1.0f);
}

void View::endFrame(void)
{
	SGE_ASSERT(m_glex != NULL);
	SGE_ASSERT(m_nanovg != NULL);

	//nvgEndFrame(m_nanovg);
	m_glex->endFrame();
}

SGE_END

