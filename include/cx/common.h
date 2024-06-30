/*
 *
 */
#ifndef CX_COMMON_H
#define CX_COMMON_H

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include <cx/config.h>

#ifdef CX_DEBUG
#	include <assert.h>
#	define CX_ASSERT(expr) assert(expr)
#else
#	define CX_ASSERT(expr)
#endif

#ifdef __cplusplus
#	define CX_BEGIN_DECLS extern "C" {
#	define CX_END_DECLS }
#else
#	define CX_BEGIN_DECLS
#	define CX_END_DECLS
#endif

#if defined(_MSC_VER)
#	define CX_FORCE_INLINE __forceinline
#elif defined(__GNUC__)
#	define CX_FORCE_INLINE __attribute __((always_inline))
#else
#	error unknown compiler.
#endif

#define CX_PMOVB(ptr, offset) \
	(((uint8_t*)ptr) + (offset))

#define CX_OFFSETOF(struct_type, member_name) \
	offsetof(struct_type, member_name)

#define CX_MEMBEROF(ptr, struct_type, member_name) \
	((struct_type*)CX_PMOVB(ptr, -CX_OFFSETOF(struct_type, member_name)))

#define CX_CONCAT(a, b) CX_CONCAT_IMPL(a, b)
#define CX_CONCAT_IMPL(a, b) a##b

#define CX_CONCAT3(a, b,c ) CX_CONCAT3_IMPL(a, b, c)
#define CX_CONCAT3_IMPL(a, b, c) a##b##c

#define CX_STRIZE(x) SGE_STRIZE_IMPL(x)
#define CX_STRIZE_IMPL(x) #x

#define CX_VERSION_STR \
	CX_STRIZE(CX_VERSION_MAJOR) "." \
	CX_STRIZE(CX_VERSION_MINOR) "." \
	CX_STRIZE(CX_VERSION_PATCH)

#endif /* CX_COMMON_H */
