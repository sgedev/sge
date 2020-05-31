/*
 *
 */
#ifndef CX_H
#define CX_H

#include <assert.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef CX_DEBUG
#	define CX_ASSERT(expr) assert(expr)
#else
#	define CX_ASSERT(expr)
#endif

#ifdef __cplusplus
#	define CX_EXTEN extern "C"
#	define CX_BEGIN_DECLS extern "C" {
#	define CX_END_DECLS }
#else
#	define CX_EXTEN
#	define CX_BEGIN_DECLS
#	define CX_END_DECLS
#endif

#if defined(_MSC_VER)
#	define CX_DLLEXPORT __declspec(dllexport)
#	define CX_DLLIMPORT __declspec(dllimport)
#	define CX_LIKELY(x) (x)
#	define CX_UNLIKELY(x) (x)
#	define CX_INLINE inline
#	define CX_PRINTF_LIKE(a, b)
#	define CX_INT64 __int64
#	define CX_UINT64 unsigned __int64
#elif defined(__GNUC__)
#	define CX_DLLEXPORT
#	define CX_DLLIMPORT
#	define CX_LIKELY(x) __builtin_expect(!!(x), 1)
#	define CX_UNLIKELY(x) __builtin_expect(!!(x), 0)
#	define CX_INLINE inline __attribute__((always_inline))
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

CX_BEGIN_DECLS

typedef signed char cx_int8_t;
typedef unsigned char cx_uint8_t;
typedef signed short cx_int16_t;
typedef unsigned short cx_uint16_t;
typedef signed int cx_int32_t;
typedef unsigned int cx_uint32_t;
typedef CX_INT64 cx_int64_t;
typedef CX_UINT64 cx_uint64_t;

CX_END_DECLS

#endif /* CX_H */
