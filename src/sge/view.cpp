//
//
#include <sge/view.hpp>

SGE_BEGIN

View::View(void)
	: m_context(NULL)
{
}

View::~View(void)
{
	if (m_context != NULL)
		shutdown();
}

bool View::init(GLEX::Context *context)
{
	SGE_ASSERT(m_context == NULL);
	SGE_ASSERT(context != NULL);

	m_context = context;

	return true;
}

void View::shutdown(void)
{
	SGE_ASSERT(m_context != NULL);

	m_context = NULL;
}

SGE_END

