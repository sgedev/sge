/*
 *
 */
#ifndef SGE_COMMON_H
#define SGE_COMMON_H

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include <sge/config.h>

#ifdef SGE_DEBUG
#   include <assert.h>
#   define SGE_ASSERT(expr) assert(expr)
#else
#   define SGE_ASSERT(expr)
#endif

#ifdef __cplusplus
#   define SGE_C_BEGIN extern "C" {
#   define SGE_C_END }
#else
#   define SGE_C_BEGIN
#   define SGE_C_END
#endif

#if defined(_MSC_VER)
#   define SGE_INLINE __forceinline
#elif defined(__linux__)
#   define SGE_INLINE // TODO
#else
#   error Unsupported platform.
#endif

#define _CON(a, b) SGE_CON_(a, b)
#define SGE_CON_(a, b) a##b

#define SGE_STR(x) SGE_STR_(x)
#define SGE_STR_(x) #x

#define SGE_VERSION_STR \
    SGE_STR(SGE_VERSION_MAJOR) "." \
    SGE_STR(SGE_VERSION_MINOR) "." \
    SGE_STR(SGE_VERSION_PATCH)

#define SGE_PMOVB(p, offset) \
    (((uint8_t*)(p)) + (offset))

#define SGE_MEMBEROF(p, struct_type, member_name) \
    ((struct_type*)SGE_PMOVB(p, -offsetof(struct_type, member_name)))

#endif // SGE_COMMON_H
