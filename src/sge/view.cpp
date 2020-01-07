//
//
#include <sge/view.hpp>

SGE_BEGIN

View::View(void)
{
}

View::~View(void)
{
}

void View::reset(void)
{
	m_clearEnabled = false;
	m_clearColor.setRgb(0, 0, 0);
	m_viewMatrix.setToIdentity();
	m_projectMatrix.setToIdentity();
}

SGE_END

