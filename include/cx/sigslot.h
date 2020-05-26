/*
 *
 */
#ifndef CX_SIGSLOT_H
#define CX_SIGSLOT_H

#include <cx/common.h>
#include <cx/list.h>
#include <cx/math.h>

#define cx_sig(type) \
	CX_CONCAT(cx_sig_, type)

#define cx_sig_func(type) \
	CX_CONCAT(cx_sig_func_, type)

#define cx_slot(type) \
	CX_CONCAT(cx_slot_, type)

#define cx_sigslot_define(type, ...) \
	typedef void (*cx_sig_func(type))(__VA_ARGS__); \
	typedef struct { \
		cx_list_t slot_list; \
	} cx_sig(type); \
	typedef struct { \
		cx_list_node_t node; \
		cx_sig_func(type) func; \
	} cx_slot(type)

#define cx_sig_init(type, sig) \
	do { \
		cx_sig(type) *p = &(sig); \
		cx_list_reset(&p->slot_list); \
	} while (0)

#define cx_slot_init(type, slot, fp) \
	do { \
		cx_slot(type) *p = &(slot); \
		cx_list_node_reset(&p->node); \
		p->func = fp; \
	} while (0)

#define cx_sig_connect(type, sig, slot) \
	do { \
		cx_sig(type) *psig = &(sig); \
		cx_slot(type) *pslot = &(slot); \
		cx_list_node_unlink(&pslot->node); \
		cx_list_add_tail(&psig->slot_list, &pslot->node); \
	} while (0)

#define cx_sig_disconnect(type, sig, slot) \
	do { \
		cx_sig(type) *psig = &(sig); \
		cx_slot(type) *pslot = &(slot); \
		cx_list_del_node(&psig->slot_list, &pslot->node); \
	} while (0)

#define cx_sig_emit(type, sig, ...) \
	do { \
		cx_list_node_t *node; \
		cx_sig(type) *psig = &(sig); \
		cx_slot(type) *slot; \
		CX_LIST_FOREACH(node, &psig->slot_list) { \
			slot = CX_MEMBEROF(node, cx_slot(type), node); \
			slot->func(__VA_ARGS__); \
		} \
	} while (0)

CX_BEGIN_C_DECLS

cx_sigslot_define(void);
cx_sigslot_define(bool, bool v);
cx_sigslot_define(int, int v);
cx_sigslot_define(float, float v);
cx_sigslot_define(str, const char *v);
cx_sigslot_define(vec3, const cx_vec3_t *v);
cx_sigslot_define(quat, const cx_quat_t *v);
cx_sigslot_define(mat4, const cx_mat4_t *v);

CX_END_C_DECLS

#endif /* CX_SIGSLOT_H */
