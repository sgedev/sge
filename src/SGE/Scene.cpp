//
//
#include <algorithm>

#include <SGE/Scene.hpp>

SGE_BEGIN

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

void Scene::update(float elapsed)
{
	for (auto it(m_objects.begin()); it != m_objects.end(); ++it)
		(*it)->update(elapsed);

	for (auto it(m_listeners.begin()); it != m_listeners.end(); ++it)
		(*it)->update(elapsed);
}

void Scene::clear(bool objs, bool cams, bool listeners)
{
	if (objs) {
		for (auto lit(m_listeners.begin()); lit != m_listeners.end(); ++lit) {
			for (auto oit(m_objects.begin()); oit != m_objects.end(); ++oit)
				(*lit)->objectRemoved(*oit);
		}
		m_objects.clear();
	}

	if (cams) {
		for (auto lit(m_listeners.begin()); lit != m_listeners.end(); ++lit) {
			for (auto cit(m_cameras.begin()); cit != m_cameras.end(); ++cit)
				(*lit)->cameraRemoved(*cit);
		}
		m_cameras.clear();
	}

	if (listeners)
		m_listeners.clear();
}

void Scene::addListener(Listener *p)
{
	SGE_ASSERT(p != NULL);

	auto it(std::find(m_listeners.begin(), m_listeners.end(), p));
	if (it != m_listeners.end())
		return;

	m_listeners.push_back(p);

	for (auto oit(m_objects.begin()); oit != m_objects.end(); ++oit) {
		for (auto lit(m_listeners.begin()); lit != m_listeners.end(); ++lit)
			(*lit)->objectRemoved(*oit);
	}
}

void Scene::removeListener(Listener *p)
{
	SGE_ASSERT(p != NULL);

	auto it(std::find(m_listeners.begin(), m_listeners.end(), p));
	if (it == m_listeners.end())
		return;

	m_listeners.erase(it);

	for (auto it(m_objects.begin()); it != m_objects.end(); ++it)
		p->objectRemoved(*it);
}

void Scene::addObject(ObjectPtr obj)
{
	SGE_ASSERT(obj);

	auto it(std::find(m_objects.begin(), m_objects.end(), obj));
	if (it != m_objects.end())
		return;

	m_objects.push_back(obj);

	for (auto it(m_listeners.begin()); it != m_listeners.end(); ++it)
		(*it)->objectAdded(obj);
}

void Scene::removeObject(ObjectPtr obj)
{
	SGE_ASSERT(obj);

	auto it(std::find(m_objects.begin(), m_objects.end(), obj));
	if (it == m_objects.end())
		return;

	m_objects.erase(it);

	for (auto it(m_listeners.begin()); it != m_listeners.end(); ++it)
		(*it)->objectRemoved(obj);
}

void Scene::addCamera(CameraPtr cam)
{
	SGE_ASSERT(cam);

	auto it(std::find(m_cameras.begin(), m_cameras.end(), cam));
	if (it != m_cameras.end())
		return;

	m_cameras.push_back(cam);

	for (auto it(m_listeners.begin()); it != m_listeners.end(); ++it)
		(*it)->cameraAdded(cam);
}

void Scene::removeCamera(CameraPtr cam)
{
	SGE_ASSERT(cam);

	auto it(std::find(m_cameras.begin(), m_cameras.end(), cam));
	if (it == m_cameras.end())
		return;

	m_cameras.erase(it);

	for (auto it(m_listeners.begin()); it != m_listeners.end(); ++it)
		(*it)->cameraRemoved(cam);
}

SGE_END
