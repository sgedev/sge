//
//
#include <SGL/Renderer.hpp>

SGL_BEGIN

Renderer::Renderer(void)
{
}

Renderer::~Renderer(void)
{
}

bool Renderer::init(void)
{
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
	// for each cameras:
	//  setup viewport from camera
	//	beginFrame();
	//	endFrame();
}

void Renderer::beginFrame(void)
{
}

void Renderer::endFrame(void)
{
}

SGL_END

#endif // SGL_CAMERA_HPP

