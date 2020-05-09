//
//
#ifndef SGL_SCENE_HPP
#define SGL_SCENE_HPP

#include <memory>
#include <list>
#include <vector>

#include <SGL/Common.hpp>
#include <SGL/Object.hpp>
#include <SGL/Camera.hpp>

SGL_BEGIN

typedef std::shared_ptr<Object> ObjectPtr;
typedef std::shared_ptr<Camera> CameraPtr;

typedef std::list<ObjectPtr> ObjectList;
typedef std::list<CameraPtr> CameraList;

typedef Uint32 ObjectID;
typedef Uint32 PropertyID;

class Scene {
public:
	Listener {
	public:
		Listener(void);
		virtual ~Listener(void);

	protected:
		virtual void objectAdded(ObjectID obj) = 0;
		virtual void objectRemoved(ObjectID obj) = 0;
		virtual void cameraAdded(CameraPtr cam) = 0;
		virtual void cameraRemoved(CameraPtr cam) = 0;
		virtual void update(float elapsed) = 0;
		virtual void draw(void) { };
	};

public:
	Scene(void);
	virtual ~Scene(void);

public:
	bool init(void);
	void clear(bool objs = true, bool cams = false, bool listeners = false);
	void addListener(Listener *p);
	void removeListener(Listener *p);

	PropertyID createProperty(const char *name);
	void destroyProperty(PropertyID prop);

	ObjectID createObject(const char *name);
	void destroyObject(ObjectID obj);
	const ObjectList &objects(void) const;
		


	void addCamera(CameraPtr cam);
	void removeCamera(CameraPtr cam);
	const CameraList &cameras(void) const;
	void update(float elapsed);
	void draw(void);

private:
	typedef std::vector<Listener *> ListenerVector;

private:
	ListenerVector m_listeners;
	ObjectList m_objects;
	CameraList m_cameras;
};

inline const ObjectList &Scene::objects(void) const
{
	return m_objects;
}

inline const CameraList &Scene::cameras(void) const
{
	return m_cameras;
}

SGL_END

#endif // SGL_SCENE_HPP

