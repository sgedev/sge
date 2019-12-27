//
//
#ifndef SGE_SCENE_HPP
#define SGE_SCENE_HPP

#include <sge/common.hpp>
#include <sge/renderer.hpp>

SGE_BEGIN

class Scene {
public:
	Scene(void);
	virtual ~Scene(void);

public:
	bool init(void);
	void update(float elapsed);
	void draw(Renderer::View *view);
};

SGE_END

#endif // SGE_SCENE_HPP

