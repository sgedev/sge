/*
 *
 */
#ifndef CX_COMMON_H
#define CX_COMMON_H

#include <stdint.h>
#include <stdlib.h>

#ifdef __cplusplus
#	define CX_BEGIN_DECLS extern "C" {
#	define CX_END_DECLS }
#else
#	define CX_BEGIN_DECLS
#	define CX_END_DECLS
#endif

#define CX_INLINE inline

#ifndef CX_ASSERT
#	include <assert.h>
#	define CX_ASSERT(expr) assert(expr)
#endif

#define CX_PMOVB(ptr, offset) \
	(((unsigned char *)(ptr)) + (offset))

#define CX_OFFSETOF(struct_type, member_name) \
	((intptr_t)(&(((struct_type *)0)->member_name)))

#define CX_MEMBEROF(ptr, struct_type, member_name) \
	((struct_type *)CX_PMOVB(ptr, -CX_OFFSETOF(struct_type, member_name)))

#endif /* CX_COMMON_H */
