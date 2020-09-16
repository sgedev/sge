//
//
#ifndef SGE_VM_HPP
#define SGE_VM_HPP

#include <string>
#include <thread>

#include <sge/vm/common.hpp>
#include <sge/vm/gui.hpp>
#include <sge/vm/scene.hpp>
#include <sge/vm/output.hpp>

SGE_VM_BEGIN

extern std::thread thread;

typedef void (*backend_func_t)(output &r);

bool start(uv_loop_t *loop, const std::string &root, const std::string &init, backend_func_t func);
void stop(void);
void post_event(const SDL_Event &evt);

SGE_VM_END

#endif // SGE_VM_HPP
