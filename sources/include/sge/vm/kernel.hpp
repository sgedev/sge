//
//
#ifndef SGE_VM_KERNEL_HPP
#define SGE_VM_KERNEL_HPP

#include <sge/vm/common.hpp>

SGE_VM_BEGIN

struct traps_type {
	void (*foo)(void);
};

extern traps_type traps;

bool start(uv_loop_t *loop);
void stop(void);
bool is_inside(void);

SGE_VM_END

#endif // SGE_VM_KERNEL_HPP
