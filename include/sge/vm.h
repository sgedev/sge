/*
 *
 */
#ifndef SGE_VM_H
#define SGE_VM_H

#include <sge/common.h>
#include <sge/object.h>

SGE_BEGIN_DECLS

#define SGE_OBJECT_ENABLED 0x1
#define SGE_OBJECT_VISIBLED 0x2
#define SGE_OBJECT_MOVABLED 0x4

typedef struct sge_vm_object sge_vm_object_t;

struct sge_vm_object {
	sge_vm_object_t *(*dup)(sge_vm_object_t *object);
	sge_vm_object_t *(*get_parent)(sge_vm_object_t *object);
	sge_vm_object_t *(*get_next)(sge_vm_object_t *object);
	void (*set_parent)(sge_vm_object_t *object, sge_vm_object_t *parent);
	const char *(*get_name)(sge_vm_object_t *object);
	void (*set_name)(sge_vm_object_t *object, const char *name);
	int (*get_flags)(sge_vm_object_t *object);
	void (*enable)(sge_vm_object_t *object);
	void (*disable)(sge_vm_object_t *object);
	void (*show)(sge_vm_object_t *object);
	void (*hide)(sge_vm_object_t *object);
	const hmm_vec3 *(*get_pos)(sge_vm_object_t *object);
	void (*set_pos)(sge_vm_object_t *object, float x, float y, float z);
	const hmm_vec3 *(*get_scale)(sge_vm_object_t *object);
	void (*set_scale)(sge_vm_object_t *object, float x, float y, float z);
	const hmm_quaternion *(*get_rotation)(sge_vm_object_t *object);
	void (*set_rotation)(sge_vm_object_t *object, float angle, float x, float y, float z);
	bool (*has)(sge_vm_object_t *object, const char *pname);
	bool (*get_int)(sge_vm_object_t *object, const char *pname, int *val);
	bool (*get_float)(sge_vm_object_t *object, const char *pname, float *val);
	bool (*get_string)(sge_vm_object_t *object, const char *pname, const char **val);
};

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
	void (*exit)(void);
	int (*get_fps)(void);
	int (*get_fps_max)(void);
	void (*set_fps_max)(int v);
	void (*toggle_show_fps)(void);
	void (*set_window_title)(const char *name);
	void (*move_window)(int x, int y);
	void (*resize_window)(int width, int height);
	void (*get_window_rect)(int *x, int *y, int *width, int *height);
	void (*show_window)(void);
	void (*hide_window)(void);
	sge_vm_object_t *(*get_root_object)(void);
	sge_vm_object_t *(*create_object)(const char *name, sge_vm_object_t *parent);
	void (*destroy_object)(sge_vm_object_t *obj);
} sge_vm_traps_t;

int sge_vm_init(void);
void sge_vm_shutdown(void);
void sge_vm_update(float elapsed, const sge_vm_traps_t *traps);
sge_vm_state_t sge_vm_get_state(void);
const sge_vm_loading_state_t *sge_vm_get_loading_state(void);

SGE_END_DECLS

#endif /* SGE_VM_H */

