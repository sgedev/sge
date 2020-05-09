//
//
#include <algorithm>

#include <SGL/Scene.hpp>

SGL_BEGIN

Scene::Listener::Listener(void)
{
}

Scene::Listener::~Listener(void)
{
}

Scene::Scene(void)
{
}

Scene::~Scene(void)
{
}

bool Scene::init(void)
{
	return true;
}

void Scene::clear(bool objs, bool cams, bool listeners)
{
	if (objs) {
		for (auto it(m_objects.begin(); it != m_objects.end(); ++it)
			p->objectRemoved(*it);
		m_objects.clear();
	}

	if (cams) {
		for (auto it(m_cameras.begin(); it != m_cameras.end(); ++it)
			p->cameraRemoved(*it);
		m_cameras.clear();
	}

	if (listeners)
		m_listeners.clear();
}

void Scene::addListener(Listener *p)
{
	SGL_ASSERT(p != NULL);

	auto it(std::find(m_listeners.begin(), m_listeners.end(), p));
	if (it == m_listeners.end())
		m_listeners.push_back(p);

	for (auto oit(m_objects.begin(); oit != m_objects.end(); ++oit) {
		for (auto lit(m_listeners.begin()); lit != m_listeners.end(); ++lit)
			(*lit)->objectRemoved(*oit);
	}
}

void Scene::removeListener(Listener *p)
{
	SGL_ASSERT(p != NULL);

	auto it(std::find(m_listeners.begin(), m_listeners.end(), p));
	if (it != m_listeners.end())
		m_listeners.erase(it);

	for (auto it(m_objects.begin(); it != m_objects.end(); ++it)
		p->objectRemoved(*it);
}

void Scene::addObject(ObjectPtr obj)
{
	SGL_ASSERT(obj);

	auto it(std::find(m_objects.begin(), m_objects.end(), obj));
	if (it == m_objects.end())
		m_listeners.push_back(p);

	for (auto it(m_listeners.begin()); it != m_listeners.end(); ++it)
		(*it)->objectAdded(obj);
}

void Scene::removeObject(ObjectPtr obj)
{
	SGL_ASSERT(obj);

	auto it(std::find(m_objects.begin(), m_objects.end(), obj));
	if (it != m_objects.end())
		m_listeners.earse(it);

	for (auto it(m_listeners.begin()); it != m_listeners.end(); ++it)
		(*it)->objectRemoved(obj);
}

void Scene::addCamera(CameraPtr cam)
{
	SGL_ASSERT(cam);

	auto it(std::find(m_cameras.begin(), m_cameras.end(), obj));
	if (it == m_cameras.end())
		m_cameras.push_back(cam);

	for (auto it(m_listeners.begin()); it != m_listeners.end(); ++it)
		(*it)->cameraAdded(obj);
}

void Scene::removeCamera(CameraPtr cam)
{
	SGL_ASSERT(obj);

	auto it(std::find(m_cameras.begin(), m_cameras.end(), obj));
	if (it != m_cameras.end())
		m_cameras.earse(it);

	for (auto it(m_listeners.begin()); it != m_listeners.end(); ++it)
		(*it)->cameraRemoved(cam);
}

void Scene::update(float elapsed)
{
	for (auto it(m_objects.begin(); it != m_objects.end(); ++it)
		(*it)->update(elapsed);

	for (auto it(m_listeners.begin()); it != m_listeners.end(); ++it)
		(*it)->update(elapsed);
}

void Scene::draw(void)
{
	for (auto it(m_listeners.begin()); it != m_listeners.end(); ++it)
		it->draw();
}

SGL_END

