//
//
#ifndef SGE_GRAPHICS_RENDERER_HPP
#define SGE_GRAPHICS_RENDERER_HPP

#include <sge/graphics/common.hpp>
#include <sge/graphics/canvas.hpp>
#include <sge/graphics/light.hpp>
#include <sge/graphics/mesh.hpp>

SGE_GRAPHICS_BEGIN

class renderer {
public:
	renderer(canvas *target);
	virtual ~renderer(void);

public:
	bool init(void);
	void shutdown(void);
	canvas *target(void);
	void begin(void);
	void end(void);

private:
	canvas *m_target;
};

SGE_INLINE canvas *renderer::target(void)
{
	return m_target;
}

SGE_GRAPHICS_END

#endif // SGE_GRAPHICS_RENDERER_HPP
