/*
 *
 */
#ifndef CX_H
#define CX_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef CX_DEBUG
#	include <assert.h>
#	define CX_ASSERT(expr) assert(expr)
#else
#	define CX_ASSERT(expr)
#endif

#ifdef __cplusplus
#	define CX_BEGIN_C_DECLS extern "C" {
#	define CX_END_C_DECLS }
#else
#	define CX_BEGIN_C_DECLS
#	define CX_END_C_DECLS
#endif

#if defined(_MSC_VER)
#	define CX_DECL_EXPORT __declspec(dllexport)
#	define CX_DECL_IMPORT __declspec(dllimport)
#	define CX_LIKELY(x) (x)
#	define CX_UNLIKELY(x) (x)
#	define CX_FORCE_INLINE __forceinline
#	define CX_PRINTF_LIKE(a, b)
#	define CX_INT64 __int64
#	define CX_UINT64 unsigned __int64
#elif defined(__GNUC__)
#	define CX_DECL_EXPORT
#	define CX_DECL_IMPORT
#	define CX_LIKELY(x) __builtin_expect(!!(x), 1)
#	define CX_UNLIKELY(x) __builtin_expect(!!(x), 0)
#	define CX_FORCE_INLINE inline __attribute__((always_inline))
#	define CX_PRINTF_LIKE(f, a) __attribute__((format(printf, f, a)))
#	ifdef _LP64
#		define CX_INT64 long
#		define CX_UINT64 unsigned long
#	else
#		define CX_INT64 long long
#		define CX_UINT64 unsgined long long
#	endif
#else
#	error unsupported compiler.
#endif

#if defined(_WIN32)
#	define CX_OS_WINDOWS
#elif defined(__linux__)
#	define CX_OS_LINUX
#else
#	error unknown os.
#endif

#ifndef CX_API
#	ifdef CX_SHARED
#		ifdef CX
#			define CX_API CX_DLLEXPORT
#		else
#			define CX_API CX_DLLIMPORT
#		endif
#	else
#		define CX_API
#	endif
#endif

#define CX_CONCAT(a, b) CX_CONCAT_IMPL(a, b)
#define CX_CONCAT_IMPL(a, b) a##b

#define CX_CONCAT3(a, b, c) CX_CONCAT_IMPL(a, b, c)
#define CX_CONCAT3_IMPL(a, b, c) a##b##c

#define CX_STRIZE(x) CX_STRIZE_IMPL(x)
#define CX_STRIZE_IMPL(x) #x

#define CX_PMOVB(p, offset) \
	(((unsigned char *)(p)) + (offset))

#define CX_OFFSETOF(struct_type, member_name) \
	((intptr_t)(&(((struct_type *)0)->member_name)))

#define CX_MEMBEROF(p, struct_type, member_name) \
	((struct_type *)CX_PMOVB(p, -CX_OFFSETOF(struct_type, member_name)))

#endif /* CX_H */
