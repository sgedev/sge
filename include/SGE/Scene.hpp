//
//
#ifndef SGE_SCENE_HPP
#define SGE_SCENE_HPP

#include <memory>
#include <list>
#include <string>

#include <SGE/Common.hpp>
#include <SGE/Object.hpp>
#include <SGE/Camera.hpp>

SGE_BEGIN

typedef std::shared_ptr<Object> ObjectPtr;
typedef std::shared_ptr<Camera> CameraPtr;

typedef std::list<ObjectPtr> ObjectList;
typedef std::list<CameraPtr> CameraList;

class Scene {
public:
	class Listener {
	public:
		Listener(void);
		virtual ~Listener(void);

	public:
		virtual void objectAdded(ObjectPtr obj) = 0;
		virtual void objectRemoved(ObjectPtr obj) = 0;
		virtual void cameraAdded(CameraPtr cam) = 0;
		virtual void cameraRemoved(CameraPtr cam) = 0;
		virtual void update(float elapsed) = 0;
	};

public:
	Scene(void);
	virtual ~Scene(void);

public:
	bool init(void);
	void shutdown(void);
	virtual void update(float elapsed);
	const std::string &name(void) const;
	void rename(const std::string &name);
	void clear(bool objs = true, bool cams = false, bool listeners = false);
	void addListener(Listener *p);
	void removeListener(Listener *p);
	void addObject(ObjectPtr obj);
	void removeObject(ObjectPtr obj);
	void addCamera(CameraPtr cam);
	void removeCamera(CameraPtr cam);

private:
	typedef std::list<Listener *> ListenerList;

private:
	std::string m_name;
	ListenerList m_listeners;
	ObjectList m_objects;
	CameraList m_cameras;
};

SGE_INLINE const std::string &Scene::name(void) const
{
	return m_name;
}

SGE_INLINE void Scene::rename(const std::string &name)
{
	m_name = name;
}

SGE_END

#endif // SGE_SCENE_HPP
