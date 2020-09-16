//
//
#ifndef SGE_RENDERER_VG_HPP
#define SGE_RENDERER_VG_HPP

#include <sge/renderer/common.hpp>

#define SGE_RENDERER_VG_BEGIN SGE_RENDERER_BEGIN namespace vg {
#define SGE_RENDERER_VG_END } SGE_RENDERER_END

SGE_RENDERER_VG_BEGIN

bool init(void);
void shutdown(void);
void reset(void);
void render(void);

SGE_RENDERER_VG_END

#endif // SGE_RENDERER_VG_HPP
