//
//
#include <sge/common.hpp>

SGE_BEGIN

static SDL_TLSID loop_tls;
static SDL_SpinLock loop_lock;

uv_loop_t *current_loop(void)
{
    if (SGE_LIKELY(loop_tls))
        return reinterpret_cast<uv_loop_t *>(SDL_TLSGet(loop_tls));

    SDL_AtomicLock(&loop_lock);

    if (loop_tls) {
        SDL_AtomicUnlock(&loop_lock);
        return reinterpret_cast<uv_loop_t *>(SDL_TLSGet(loop_tls));
    }

    loop_tls = SDL_TLSCreate();
    if (loop_tls) {
        uv_loop_t *loop = uv_loop_new();
        SDL_TLSSet(loop_tls, loop, [](void *p) {
            uv_loop_close(reinterpret_cast<uv_loop_t *>(p));
        });
        SDL_AtomicUnlock(&loop_lock);
        return loop;
    }

    SDL_AtomicUnlock(&loop_lock);

    return nullptr;
}

SGE_END
