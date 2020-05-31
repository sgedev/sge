//
//
#ifndef SGE_GRAPHICS_RENDERER_HPP
#define SGE_GRAPHICS_RENDERER_HPP

#include <SGE/Scene.hpp>
#include <SGE/Graphics/Common.hpp>
#include <SGE/Graphics/Memory.hpp>

SGE_GRAPHICS_BEGIN

class Renderer {
public:
	Renderer(void);
	virtual ~Renderer(void);

public:
	bool init(Scene &scene);
	void shutdown(void);
	void draw(void);

protected:
	void objectAdded(ObjectPtr obj);
	void objectRemoved(ObjectPtr obj);
	void cameraAdded(CameraPtr cam);
	void cameraRemoved(CameraPtr cam);

private:
	Scene *m_scene;
	Memory m_memory;
};

SGE_GRAPHICS_END

#endif // SGE_GRAPHICS_RENDERER_HPP
