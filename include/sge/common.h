/*
 *
 */
#ifndef SGE_H
#define SGE_H

#include <assert.h>
#include <stdarg.h>
#include <stdbool.h>

#ifdef SGE_DEBUG
#	define SGE_ASSERT(expr) assert(expr)
#else
#	define SGE_ASSERT(expr)
#endif

#ifdef __cplusplus
#	define SGE_EXTEN extern "C"
#	define SGE_BEGIN_DECLS extern "C" {
#	define SGE_END_DECLS }
#else
#	define SGE_EXTEN
#	define SGE_BEGIN_DECLS
#	define SGE_END_DECLS
#endif

#if defined(_MSC_VER)
#	define SGE_LIKELY(x) (x)
#	define SGE_UNLIKELY(x) (x)
#	define SGE_INLINE inline
#	define SGE_PRINTF_LIKE(a, b)
#	define SGE_INT64 __int64
#	define SGE_UINT64 unsigned __int64
#elif defined(__GNUC__)
#	define SGE_LIKELY(x) __builtin_expect(!!(x), 1)
#	define SGE_UNLIKELY(x) __builtin_expect(!!(x), 0)
#	define SGE_INLINE inline __attribute__((always_inline))
#	define SGE_PRINTF_LIKE(f, a) __attribute__((format(printf, f, a)))
#	ifdef _LP64
#		define SGE_INT64 long
#		define SGE_UINT64 unsigned long
#	else
#		define SGE_INT64 long long
#		define SGE_UINT64 unsgined long long
#	endif
#else
#	error unsupported compiler.
#endif

#define SGE_CONCAT(a, b) SGE_CONCAT_IMPL(a, b)
#define SGE_CONCAT_IMPL(a, b) a##b

#define SGE_CONCAT3(a, b, c) SGE_CONCAT_IMPL(a, b, c)
#define SGE_CONCAT3_IMPL(a, b, c) a##b##c

#define SGE_STRIZE(x) SGE_STRIZE_IMPL(x)
#define SGE_STRIZE_IMPL(x) #x

#define SGE_PMOVB(p, offset) \
	(((unsigned char *)(p)) + (offset))

#define SGE_OFFSETOF(struct_type, member_name) \
	((intptr_t)(&(((struct_type *)0)->member_name)))

#define SGE_MEMBEROF(p, struct_type, member_name) \
	((struct_type *)SGE_PMOVB(p, -SGE_OFFSETOF(struct_type, member_name)))

SGE_BEGIN_DECLS

typedef signed char sge_int8_t;
typedef unsigned char sge_uint8_t;
typedef signed short sge_int16_t;
typedef unsigned short sge_uint16_t;
typedef signed int sge_int32_t;
typedef unsigned int sge_uint32_t;
typedef SGE_INT64 sge_int64_t;
typedef SGE_UINT64 sge_uint64_t;

SGE_END_DECLS

#endif /* SGE_H */
