//
//
#ifndef SGE_COMMON_HPP
#define SGE_COMMON_HPP

#include <cstddef>

#include <uv.h>
#include <SDL.h>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <SGE/Config.hpp>

#define SGE_INLINE inline

#ifdef SGE_DEBUG
#   include <SDL_assert.h>
#	ifdef SDL_ASSERT_LEVEL
#		undef SDL_ASSERT_LEVEL
#	endif
#   define SDL_ASSERT_LEVEL 2
#   define SGE_ASSERT(expr) SDL_assert(expr)
#else
#   define SGE_ASSERT(expr)
#endif

#ifdef SGE_LOG
#	define SGE_LOG_CATEGORY SDL_LOG_CATEGORY_CUSTOM
#	define SGE_LOGE(fmt, ...) SDL_LogError(SGE_LOG_CATEGORY, fmt, ##__VA_ARGS__)
#	define SGE_LOGW(fmt, ...) SDL_LogWarn(SGE_LOG_CATEGORY, fmt, ##__VA_ARGS__)
#	define SGE_LOGI(fmt, ...) SDL_LogInfo(SGE_LOG_CATEGORY, fmt, ##__VA_ARGS__)
#	define SGE_LOGD(fmt, ...) SDL_LogDebug(SGE_LOG_CATEGORY, fmt, ##__VA_ARGS__)
#else
#	define SGE_LOGE(fmt, ...) do { } while (0)
#	define SGE_LOGW(fmt, ...) do { } while (0)
#	define SGE_LOGI(fmt, ...) do { } while (0)
#	define SGE_LOGD(fmt, ...) do { } while (0)
#endif

#define SGE_BEGIN namespace SGE {
#define SGE_END }

SGE_BEGIN

class Noncopyable {
public:
	Noncopyable(void) { }

private:
	Noncopyable(const Noncopyable &);
	Noncopyable &operator=(const Noncopyable &);
};

SGE_END

#endif // SGE_COMMON_HPP
