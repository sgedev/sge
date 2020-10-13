//
//
#ifndef SGE_COMMON_HPP
#define SGE_COMMON_HPP

#include <cstdint>
#include <list>
#include <vector>
#include <string>

#define GLM_FORCE_INLINE
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <sge/common.h>

#ifdef max
#	undef max
#endif

#ifdef min
#	undef min
#endif

#include <rttr/type>

#define SGE_BEGIN namespace sge {
#define SGE_END }

SGE_BEGIN

typedef std::vector<uint8_t> byte_array_t;
typedef std::list<std::string> string_list_t;

SGE_END

#endif // SGE_COMMON_HPP
