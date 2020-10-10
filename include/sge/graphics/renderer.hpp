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
	renderer(void);
	virtual ~renderer(void);

public:
	canvas *target(void);
	void set_target(canvas *p);

private:
	canvas *m_target;
};

SGE_INLINE canvas *renderer::target(void)
{
	return m_target;
}

SGE_GRAPHICS_END

#endif // SGE_GRAPHICS_RENDERER_HPP
