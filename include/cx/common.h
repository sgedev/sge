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
#elif defined(__GNUC__)
#	define CX_DLLEXPORT
#	define CX_DLLIMPORT
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

#define CX_LIKELY(expr) expr
#define CX_UNLIKELY(expr) expr

#define CX_INLINE inline

#define CX_CON(a, b) CX_CON_(a, b)
#define CX_CON_(a, b) a##b

#define CX_STR(x) CX_STR_(x)
#define CX_STR_(x) #x

#endif /* CX_H */

