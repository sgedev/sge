//
//
#include <sge/scene.hpp>

SGE_BEGIN

Scene::Scene(QObject *parent)
	: QObject(parent)
{
}

Scene::~Scene(void)
{
}

bool Scene::init(void)
{
	return true;
}

void Scene::shutdown(void)
{
}

void Scene::update(float elapsed)
{
}

void Scene::draw(Renderer::Context *rc)
{
}

SGE_END

