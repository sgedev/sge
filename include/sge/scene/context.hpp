//
//
#ifndef SGE_SCENE_CONTEXT_HPP
#define SGE_SCENE_CONTEXT_HPP

#include <sge/renderer.hpp>
#include <sge/scene/common.hpp>

SGE_SCENE_BEGIN

class context {
public:
	context(void);
	virtual ~context(void);

public:
	bool init(renderer::context *p);
	void shutdown(void);
	void update(float elapsed);
	void draw(void);

private:
	renderer::context *m_renderer;
};

SGE_SCENE_END

#endif // SGE_SCENE_CONTEXT_HPP

