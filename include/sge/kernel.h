/*
 *
 */
#ifndef SGE_KERNEL_H
#define SGE_KERNEL_H

#include <sge/common.h>

typedef struct {
	void (*foo)();
} sge_kernel_traps_t;

typedef void (*sge_kernel_start_cb)(void *);

extern sge_kernel_traps_t sge_kernel_traps;

int sge_kernel_init(uv_loop_t* loop, const char* rootfs, const char* initrc);
void sge_kernel_shutdown();
int sge_kernel_start(sge_kernel_start_cb cb, void *data);
void sge_kernel_stop();
void sge_kernel_feed_event(const SDL_Event* evt);
uv_loop_t* sge_kernel_loop();

#endif /* SGE_KERNEL_H */
