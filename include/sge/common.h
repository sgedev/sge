/*
 *
 */
#ifndef SGE_COMMON_H
#define SGE_COMMON_H

#include <cx/common.h>
#include <HandmadeMath.h>
#include <GL/glew.h>
#include <GL/glex.h>
#include <nanovg.h>
#include <SDL.h>

#include <sge/config.h>

#if defined(_MSC_VER)
#	define SGE_DLLEXPORT __declspec(dllexport)
#	define SGE_DLLIMPORT __declspec(dllimport)
#	define SGE_INT64 __int64
#	define SGE_UINT64 unsigned __int64
#elif defined(__GNUC__)
#	define SGE_DLLEXPORT
#	define SGE_DLLIMPORT
#	ifdef _LP64
#		define SGE_INT64 long
#		define SGE_UINT64 unsigned long
#	else
#		define SGE_INT64 long long
#		define SGE_UINT64 unsigned long long
#	endif
#else
#	error unknown compiler.
#endif

#define SGE_BEGIN_DECLS CX_BEGIN_DECLS
#define SGE_END_DECLS CX_END_DECLS

#ifdef SGE
#	define SGE_API SGE_DLLEXPORT
#else
#	define SGE_API SGE_DLLIMPORT
#endif

#endif /* SGE_COMMON_H */

