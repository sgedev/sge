//
//
#ifndef SGE_HPP
#define SGE_HPP

#include <string>

#include <sge/common.hpp>
#include <sge/renderer.hpp>
#include <sge/sound.hpp>
#include <sge/vm.hpp>

SGE_BEGIN

bool start(uv_loop_t *loop, const std::string &root, const std::string &init);
void stop(void);
void post_event(const SDL_Event &evt);

SGE_END

#endif // SGE_HPP
