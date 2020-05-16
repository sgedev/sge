//
//
#include <SGE/Graphics/Renderer.hpp>

SGE_GRAPHICS_BEGIN

Renderer::Renderer(void)
{
}

Renderer::~Renderer(void)
{
}

bool Renderer::init(void)
{
	return true;
}

void Renderer::objectAdded(ObjectPtr obj)
{
}

void Renderer::objectRemoved(ObjectPtr obj)
{
}

void Renderer::cameraAdded(CameraPtr cam)
{
}

void Renderer::cameraRemoved(CameraPtr cam)
{
}

void Renderer::update(float elapsed)
{
}

void Renderer::draw(void)
{
}

SGE_GRAPHICS_END
