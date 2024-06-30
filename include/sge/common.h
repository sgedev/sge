/*
 *
 */
#ifndef SGE_COMMON_H
#define SGE_COMMON_H

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include <lua.h>
#include <lauxlib.h>
#include <physfs.h>
#include <uv.h>
#include <SDL.h>

#include <sge/config.h>

#ifdef SGE_DEBUG
#	include <assert.h>
#	define SGE_ASSERT(expr) assert(expr)
#else
#	define SGE_ASSERT(expr)
#endif

#ifdef __cplusplus
#	define SGE_C_BEGIN extern "C" {
#	define SGE_C_END }
#else
#	define SGE_C_BEGIN
#	define SGE_C_END
#endif

#if defined(_MSC_VER)
#	define SGE_FORCE_INLINE __forceinline
#elif defined(__GNUC__)
#	define SGE_FORCE_INLINE __attribute __((always_inline))
#else
#	error unknown compiler.
#endif

#define SGE_PMOVB(ptr, offset) \
	(((uint8_t*)ptr) + (offset))

#define SGE_OFFSETOF(struct_type, member_name) \
	offsetof(struct_type, member_name)

#define SGE_MEMBEROF(ptr, struct_type, member_name) \
	((struct_type*)SGE_PMOVB(ptr, -SGE_OFFSETOF(struct_type, member_name)))

#define SGE_CONCAT(a, b) SGE_CONCAT_IMPL(a, b)
#define SGE_CONCAT_IMPL(a, b) a##b

#define SGE_CONCAT3(a, b,c ) SGE_CONCAT3_IMPL(a, b, c)
#define SGE_CONCAT3_IMPL(a, b, c) a##b##c

#define SGE_STRIZE(x) SGE_STRIZE_IMPL(x)
#define SGE_STRIZE_IMPL(x) #x

#define SGE_VERSION_STR \
	SGE_STRIZE(SGE_VERSION_MAJOR) "." \
	SGE_STRIZE(SGE_VERSION_MINOR) "." \
	SGE_STRIZE(SGE_VERSION_PATCH)

#endif /* SGE_COMMON_H */
