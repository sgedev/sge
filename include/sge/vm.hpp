//
//
#ifndef SGE_VM_HPP
#define SGE_VM_HPP

#include <sge/vm/common.hpp>
#include <sge/vm/gui.hpp>
#include <sge/vm/scene.hpp>
#include <sge/vm/output.hpp>

SGE_VM_BEGIN

typedef void (*output_func_t)(output *p);

bool start(uv_loop_t *loop, output_func_t func);
void stop(void);
void post_event(const SDL_Event &evt);

SGE_VM_END

#endif // SGE_VM_HPP
