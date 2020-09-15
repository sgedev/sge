//
//
#ifndef SGE_HPP
#define SGE_HPP

#include <sge/common.hpp>
#include <sge/renderer.hpp>
#include <sge/vm.hpp>

SGE_BEGIN

bool start(uv_loop_t *loop);
void stop(void);
void post_event(const SDL_Event &evt);

SGE_END

#endif // SGE_HPP
