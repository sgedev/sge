/*
 *
 */
#ifndef SGE_VM_H
#define SGE_VM_H

#include <sge/common.h>

typedef enum {
	SGE_VM_STATE_NONE = 0,
	SGE_VM_STATE_INITIALIZING,
	SGE_VM_STATE_LOADING,
	SGE_VM_STATE_RUNNING,
	SGE_VM_STATE_EXITING,
	SGE_VM_STATE_ERROR
} sge_vm_state_t;

typedef struct {
	const char *message;
	int percentage;
} sge_vm_loading_state_t;

typedef struct {
	/* base */
	void (*exit)(void);
	int (*get_fps)(void);
	int (*get_fps_max)(void);
	void (*set_fps_max)(int v);
	void (*toggle_show_fps)(void);
	bool (*set_font)(const char *filename, int size);
	struct {
		void (*set_title)(const char *name);
		void (*move)(int x, int y);
		void (*resize)(int width, int height);
		void (*get_rect)(int *x, int *y, int *width, int *height);
		void (*show)(void);
		void (*hide)(void);
	} window;
	struct {
		void (*reset)(void);
	} scene;
} sge_vm_traps_t;

int sge_vm_init(void);
void sge_vm_shutdown(void);
void sge_vm_update(float elapsed, const sge_vm_traps_t *traps);
sge_vm_state_t sge_vm_get_state(void);
const sge_vm_loading_state_t *sge_vm_get_loading_state(void);

#endif /* SGE_VM_H */

