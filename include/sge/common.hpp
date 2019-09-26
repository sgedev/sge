//
//
#ifndef SGE_COMMON_HPP
#define SGE_COMMON_HPP

#include <cstdint>

#include <glm.hpp>
#include <cx/common.hpp>

#include <sge/config.hpp>

#ifdef SGE_DEBUG
#	define SGE_ASSERT(expr) CX_ASSERT(expr)
#else
#	define SGE_ASSERT(expr)
#endif

#define SGE_BEGIN namespace sge {
#define SGE_END }

#endif // SGE_COMMON_HPP

