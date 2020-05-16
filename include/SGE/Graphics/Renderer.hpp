//
//
#ifndef SGE_GRAPHICS_RENDERER_HPP
#define SGE_GRAPHICS_RENDERER_HPP

#include <SGE/Scene.hpp>
#include <SGE/Graphics/Common.hpp>

SGE_GRAPHICS_BEGIN

class Renderer : public Scene::Listener {
public:
	Renderer(void);
	virtual ~Renderer(void);

public:
	bool init(void);
	void objectAdded(ObjectPtr obj) override;
	void objectRemoved(ObjectPtr obj) override;
	void cameraAdded(CameraPtr cam) override;
	void cameraRemoved(CameraPtr cam) override;
	void update(float elapsed) override;
	void draw(void);

private:

};

SGE_GRAPHICS_END

#endif // SGE_GRAPHICS_RENDERER_HPP
