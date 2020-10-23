//
//
#ifndef SGE_COMMON_HPP
#define SGE_COMMON_HPP

#include <cstdint>
#include <list>
#include <vector>
#include <string>
#include <filesystem>

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
#include <rttr/registration>

#define SGE_BEGIN namespace sge {
#define SGE_END }

SGE_BEGIN

typedef std::vector<uint8_t> byte_array_t;
typedef std::list<std::string> string_list_t;

class noncopyable {
public:
    inline noncopyable(void) { }
    noncopyable(const noncopyable &) = delete;
    noncopyable &operator=(const noncopyable &) = delete;
};

static inline void uv_sync_close(uv_handle_t *p)
{
    int done = 0;
    p->data = &done;
    uv_close(p, [](uv_handle_t *p) { *reinterpret_cast<int *>(p->data) = 1; });
    while (!done) { uv_run(p->loop, UV_RUN_ONCE); }
}

static inline void uv_sync_close(uv_timer_t *p)
{
    uv_sync_close(reinterpret_cast<uv_handle_t *>(p));
}

static inline void uv_sync_close(uv_async_t *p)
{
    uv_sync_close(reinterpret_cast<uv_handle_t *>(p));
}

static inline void uv_sync_close(uv_prepare_t *p)
{
    uv_sync_close(reinterpret_cast<uv_handle_t *>(p));
}

static inline bool is_writable_path(const std::string &path)
{
    static const std::filesystem::perms write_perms =
        std::filesystem::perms::owner_write |
        std::filesystem::perms::group_write |
        std::filesystem::perms::others_write;

    std::filesystem::perms perms = std::filesystem::status(path).permissions();

    return ((perms & write_perms) != std::filesystem::perms::none);
}

SGE_END

#endif // SGE_COMMON_HPP
