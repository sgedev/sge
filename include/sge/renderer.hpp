//
//
#ifndef SGE_RENDERER_HPP
#define SGE_RENDERER_HPP

#include <sge/renderer/common.hpp>
#include <sge/renderer/vg.hpp>
#include <sge/renderer/mesh.hpp>

SGE_RENDERER_BEGIN

bool init(void);
void shutdown(void);
void begin(void);
void end(void);

SGE_RENDERER_END

#endif // SGE_RENDERER_HPP
