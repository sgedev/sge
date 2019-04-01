/*
 *
 */
#ifndef SGE_COMMON_H
#define SGE_COMMON_H

#include <assert.h>
#include <stdlib.h>

#include <sge.h>

#ifdef SGE_DEBUG
#	define SGE_ASSERT(expr) assert(expr)
#else
#	define SGE_ASSERT(expr)
#endif

#define SGE_PMOVB(p, offset) \
	(((sge_uint8_t *)p) + (offset))

#define SGE_OFFSETOF(struct_type, member_name) \
	((int)((long *)(&(((struct_type *)0)->member_name))))

#define SGE_MEMBEROF(p, struct_type, member_name) \
	SGE_PMOVB(p, -SGE_OFFSETOF(struct_type, member_name))

#endif /* SGE_COMMON_H */

