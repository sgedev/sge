//
//
#include <SGE/Graphics/Renderer.hpp>

SGE_GRAPHICS_BEGIN

Renderer::Renderer(void):
	m_staticVertexHeap(GL_ARRAY_BUFFER, GL_STATIC_DRAW),
	m_staticVertexIndexHeap(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW),
	m_dynamicVertexHeap(GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW),
	m_dynamicVertexIndexHeap(GL_ELEMENT_ARRAY_BUFFER, GL_DYNAMIC_DRAW)
{
}

Renderer::~Renderer(void)
{
	shutdown();
}

bool Renderer::init(void)
{
	return true;
}

void Renderer::shutdown(void)
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
}

SGE_GRAPHICS_END
