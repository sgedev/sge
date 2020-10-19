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

static inline void uv_close(uv_handle_t *ph)
{
    int done = 0;
    ph->data = &done;
    uv_close(ph, [](uv_handle_t *ph) { *reinterpret_cast<int *>(ph->data) = 1; });
    while (!done) { printf("loop\n"); uv_run(ph->loop, UV_RUN_ONCE); }
}

static inline bool is_writable_path(const std::string &path)
{
    static const std::filesystem::perms write_perms =
        std::filesystem::perms::owner_write |
        std::filesystem::perms::group_write |
        std::filesystem::perms::others_write;

    std::filesystem::perms perms = std::filesystem::status(path).permissions();
    if ((perms & write_perms) != std::filesystem::perms::none)
        return true;

    return false;
}

SGE_END

#endif // SGE_COMMON_HPP
