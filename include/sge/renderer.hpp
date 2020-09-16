//
//
#ifndef SGE_RENDERER_HPP
#define SGE_RENDERER_HPP

#include <sge/renderer/common.hpp>
#include <sge/renderer/fb.hpp>
#include <sge/renderer/light.hpp>
#include <sge/renderer/mesh.hpp>
#include <sge/renderer/vg.hpp>

SGE_RENDERER_BEGIN

extern fb default_target;
extern light ambient_light;

bool init(void);
void shutdown(void);
void begin(void);
void end(void);
void add(light *p);
void add(mesh *p);

SGE_RENDERER_END

#endif // SGE_RENDERER_HPP
