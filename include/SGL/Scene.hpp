//
//
#ifndef SGL_SCENE_HPP
#define SGL_SCENE_HPP

#include <memory>
#include <list>

#include <SGL/Common.hpp>
#include <SGL/Object.hpp>
#include <SGL/Camera.hpp>

SGL_BEGIN

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
	void addObject(ObjectPtr obj);
	void removeObject(ObjectPtr obj);
	void addCamera(CameraPtr cam);
	void removeCamera(CameraPtr cam);
	void update(float elapsed);
	void draw(void);

private:
	typedef std::list<Listener *> ListenerList;

private:
	ListenerList m_listeners;
	ObjectList m_objects;
	CameraList m_cameras;
};

SGL_END

#endif // SGL_SCENE_HPP

