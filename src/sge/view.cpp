//
//
#include <sge/view.hpp>

SGE_BEGIN

view::view(void)
	: m_context(NULL)
{
}

view::~view(void)
{
	if (m_context != NULL)
		shutdown();
}

bool view::init(GLEX::Context *context)
{
	SGE_ASSERT(m_context == NULL);
	SGE_ASSERT(context != NULL);

	m_context = context;

	return true;
}

void view::shutdown(void)
{
	SGE_ASSERT(m_context != NULL);

	m_context = NULL;
}

SGE_END

