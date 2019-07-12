//
//
#include <SGE/Scene/Base.h>

SGE_SCENE_BEGIN

Base::Base(QObject *parent)
	: QObject(parent)
{
}

Base::~Base(void)
{
}

void Base::update(float elapsed)
{
}

void Base::draw(Renderer::Base *target)
{
}

SGE_SCENE_END

