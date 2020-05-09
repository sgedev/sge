//
//
#ifndef SGL_RENDERER_HPP
#define SGL_RENDERER_HPP

#include <SGL/Common.hpp>
#include <SGL/Scene.hpp>

SGL_BEGIN

class Renderer: public Scene::Listener {
public:
	Renderer(void);
	virtual ~Renderer(void);

public:
	bool init(void);

protected:
	void objectAdded(ObjectPtr obj) override;
	void objectRemoved(ObjectPtr obj) override;
	void cameraAdded(CameraPtr cam) override;
	void cameraRemoved(CameraPtr cam) override;
	void update(float elapsed) override;
	void draw(void) override;
};

SGL_END

#endif // SGL_RENDERER_HPP

