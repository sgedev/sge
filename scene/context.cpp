//
//
#include <sge/scene/context.hpp>

SGE_SCENE_BEGIN

context::context(void)
{
}

context::~context(void)
{
}

bool context::init(void)
{
	return true;
}

void context::shutdown(void)
{
}

void context::update(float elapsed)
{
}

void context::draw(renderer::context &r)
{
}

SGE_SCENE_END

