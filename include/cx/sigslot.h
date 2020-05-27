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
	typedef void (*cx_sig_func(type))(__VA_ARGS__, void *data); \
	typedef struct { \
		cx_list_t slot_list; \
	} cx_sig(type); \
	typedef struct { \
		cx_list_node_t node; \
		cx_sig_func(type) func; \
		void *data; \
	} cx_slot(type)

#define cx_sig_init(type, sig) \
	do { \
		cx_sig(type) *p = &(sig); \
		cx_list_reset(&p->slot_list); \
	} while (0)

#define cx_slot_init(type, slot, fp, userdata) \
	do { \
		cx_slot(type) *p = &(slot); \
		cx_list_node_reset(&p->node); \
		p->func = fp; \
		p->data = userdata; \
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
			slot->func(__VA_ARGS__, slot->data); \
		} \
	} while (0)

#define cx_sig_emit_except(type, sig, except_slot, ...) \
	do { \
		cx_list_node_t *node; \
		cx_sig(type) *psig = &(sig); \
		cx_slot(type) *slot; \
		CX_LIST_FOREACH(node, &psig->slot_list) { \
			slot = CX_MEMBEROF(node, cx_slot(type), node); \
			if (slot != &(except_slot)) \
				slot->func(__VA_ARGS__, slot->data); \
		} \
	} while (0)

#endif /* CX_SIGSLOT_H */
