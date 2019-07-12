//
//
#include <SGE/Renderer/Base.h>

SGE_RENDERER_BEGIN

Base::Base(QObject *parent)
	: QObject(parent)
{
}

Base::~Base(void)
{
}

void Base::reset(void)
{
}

void Base::render(void)
{
}

SGE_RENDERER_END

