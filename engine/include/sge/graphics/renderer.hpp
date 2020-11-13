//
//
#ifndef SGE_GRAPHICS_RENDERER_HPP
#define SGE_GRAPHICS_RENDERER_HPP

#include <GL/Program.hpp>

#include <sge/graphics/common.hpp>
#include <sge/graphics/surface.hpp>
#include <sge/graphics/light.hpp>
#include <sge/graphics/mesh.hpp>
#include <sge/graphics/material.hpp>
#include <sge/graphics/view.hpp>

SGE_GRAPHICS_BEGIN

class renderer {
public:
    renderer(void);
	virtual ~renderer(void);

public:
    bool init(surface *target);
	void shutdown(void);
    void render(const view &v);

private:
    surface *m_target;
    NVGcontext *m_vg;
    GL::Program m_program;
    light m_ambient_light;
};

SGE_GRAPHICS_END

#endif // SGE_GRAPHICS_RENDERER_HPP
