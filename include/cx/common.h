/*
 *
 */
#ifndef CX_H
#define CX_H

#include <assert.h>
#include <stdarg.h>

#include <cx/config.h>

#ifdef CX_DEBUG
#	define CX_ASSERT(expr) assert(expr)
#else
#	define CX_ASSERT(expr)
#endif

#ifdef __cplusplus
#	define CX_EXTEN_C extern "C"
#	define CX_BEGIN_DECLS extern "C" {
#	define CX_END_DECLS }
#else
#	define CX_EXTEN_C
#	define CX_BEGIN_DECLS
#	define CX_END_DECLS
#endif

#if defined(_MSC_VER)
#	define CX_DLLEXPORT __declspec(dllexport)
#	define CX_DLLIMPORT __declspec(dllimport)
#	define CX_LIKELY(x) (x)
#	define CX_UNLIKELY(x) (x)
#	define CX_FORCE_INLINE inline
#	define CX_PRINTF_LIKE(a, b)
#elif defined(__GNUC__)
#	define CX_DLLEXPORT
#	define CX_DLLIMPORT
#	define CX_LIKELY(x) __builtin_expect(!!(x), 1)
#	define CX_UNLIKELY(x) __builtin_expect(!!(x), 0)
#	define CX_FORCE_INLINE inline __attribute__((always_inline))
#	define CX_PRINTF_LIKE(f, a) __attribute__((format(printf, f, a)))
#else
#	error unsupported compiler.
#endif

#ifdef CX_SHARED
#	ifdef CX
#		define CX_API CX_DLLEXPORT
#	else
#		define CX_API CX_DLLIMPORT
#	endif
#else
#	define CX_API
#endif

#define CX_CONCAT(a, b) CX_CONCAT_IMPL(a, b)
#define CX_CONCAT_IMPL(a, b) a##b

#define CX_STRIZE(x) CX_STRIZE_IMPL(x)
#define CX_STRIZE_IMPL(x) #x

#define CX_PMOVB(p, offset) \
	(((unsigned char *)(p)) + (offset))

#define CX_OFFSETOF(struct_type, member_name) \
	((intptr_t)(&(((struct_type *)0)->member_name)))

#define CX_MEMBEROF(p, struct_type, member_name) \
	((struct_type *)CX_PMOVB(p, -CX_OFFSETOF(struct_type, member_name)))

#endif /* CX_H */

