//
//
#ifndef SGE_COMMON_H
#define SGE_COMMON_H

#include <cstdint>

#include <uv.h>

//#define GLM_FORCE_CXX11
#define GLM_FORCE_INLINE
//#define GLM_FORCE_NO_CTOR_INIT
//#define GLM_FORCE_EXPLICIT_CTOR
#include <glm/glm.hpp>
#include <glm/gtc/bitfield.hpp>
#include <glm/gtc/color_space.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/epsilon.hpp>
#include <glm/gtc/functions.hpp>
#include <glm/gtc/integer.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#define SDL_MAIN_HANDLED
#include <SDL.h>

#include <sge/config.hpp>

#ifdef SGE_DEBUG
#	undef SDL_ASSERT_LEVEL
#	define SDL_ASSERT_LEVEL 2
#	include <SDL_assert.h>
#	define SGE_ASSERT(expr) SDL_assert(expr)
#	define SGE_LOG_CATEGORY SDL_LOG_CATEGORY_APPLICATION
#	ifdef __GNUC__
#		define SGE_LOGI(fmt, ...) SDL_LogInfo(SGE_LOG_CATEGORY, fmt, ##__VA_ARGS__)
#		define SGE_LOGE(fmt, ...) SDL_LogError(SGE_LOG_CATEGORY, fmt, ##__VA_ARGS__)
#		define SGE_LOGW(fmt, ...) SDL_LogWarn(SGE_LOG_CATEGORY, fmt, ##__VA_ARGS__)
#		define SGE_LOGD(fmt, ...) SDL_LogDebug(SGE_LOG_CATEGORY, "%s(%d) " fmt, __func__, __LINE__, ##__VA_ARGS__)
#	else
#		define SGE_LOGI(fmt, ...) SDL_LogInfo(SGE_LOG_CATEGORY, fmt, __VA_ARGS__)
#		define SGE_LOGE(fmt, ...) SDL_LogError(SGE_LOG_CATEGORY, fmt, __VA_ARGS__)
#		define SGE_LOGW(fmt, ...) SDL_LogWarn(SGE_LOG_CATEGORY, fmt, __VA_ARGS__)
#		define SGE_LOGD(fmt, ...) SDL_LogDebug(SGE_LOG_CATEGORY, "%s(%d) " fmt, __func__, __LINE__, __VA_ARGS__)
#	endif
#else
#	define SGE_ASSERT(expr)
#	define SGE_LOGI(fmt, ...) SDL_LogInfo(SGE_LOG_CATEGORY, fmt, ##__VA_ARGS__)
#	define SGE_LOGE(fmt, ...) SDL_LogError(SGE_LOG_CATEGORY, fmt, ##__VA_ARGS__)
#	define SGE_LOGW(fmt, ...) SDL_LogWarn(SGE_LOG_CATEGORY, fmt, ##__VA_ARGS__)
#	define SGE_LOGD(fmt, ...)
#endif

#define SGE_CONCAT(a, b) SGE_CONCAT_IMPL(a, b)
#define SGE_CONCAT_IMPL(a, b) a##b

#define SGE_STRIZE(x) SGE_STRIZE_IMPL(x)
#define SGE_STRIZE_IMPL(x) #x

#define SGE_BEGIN namespace sge {
#define SGE_END }

SGE_BEGIN

typedef std::int8_t int8_t;
typedef std::uint8_t uint8_t;
typedef std::int16_t int16_t;
typedef std::uint16_t uint16_t;
typedef std::int32_t int32_t;
typedef std::uint32_t uint32_t;
typedef std::int64_t int64_t;
typedef std::uint64_t uint64_t;

uv_loop_t *main_loop(void);
unsigned int fps(void);
float elapsed(void);

SGE_END

#endif // SGE_COMMON_H
