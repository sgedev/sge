/*
 *
 */
#ifndef SGE_VM_H
#define SGE_VM_H

#include <sge/common.h>

typedef struct {
	int (*get_fps)(void);
	int (*get_fps_max)(void);
	void (*set_fps_max)(int v);
	void (*toggle_show_fps)(void);
} sge_vm_traps_t;

int sge_vm_init(void);
void sge_vm_shutdown(void);
void sge_vm_update(float elapsed, const sge_vm_traps_t *traps);

#endif /* SGE_VM_H */

