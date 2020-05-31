//
//
#include <SGE/Graphics/Mesh.hpp>
#include <SGE/Graphics/Renderer.hpp>

SGE_GRAPHICS_BEGIN

Renderer::Renderer(void):
	m_scene(NULL)
{
}

Renderer::~Renderer(void)
{
	if (m_scene != NULL)
		shutdown();
}

bool Renderer::init(Scene &scene)
{
	SGE_ASSERT(m_scene == NULL);

	m_scene = &scene;

	m_scene->addObject.connect<&Renderer::objectAdded>(this);
	m_scene->removeObject.connect<&Renderer::objectRemoved>(this);
	m_scene->addCamera.connect<&Renderer::cameraAdded>(this);
	m_scene->removeCamera.connect<&Renderer::cameraRemoved>(this);

	return true;
}

void Renderer::shutdown(void)
{
	SGE_ASSERT(m_scene != NULL);

	m_scene = NULL;
}

void Renderer::draw(void)
{
}

void Renderer::objectAdded(ObjectPtr obj)
{
	// mesh = new Mesh(m_memory);
	// mesh->
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

SGE_GRAPHICS_END
