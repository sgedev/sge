//
//
#ifndef SGE_ASYNC_HPP
#define SGE_ASYNC_HPP

#include <atomic>
#include <thread>
#include <functional>

#include <sge/common.hpp>
#include <sge/looper.hpp>

SGE_BEGIN

class async: public looper::handle {
public:
    using function = std::function<void (void)>;

public:
    async(looper &lp);
    ~async(void) override;

public:
    bool start(function fn);
    void stop(void);
    looper::duration update(void) override;
    void notify(void);

private:
    function m_fn;
    std::atomic_bool m_activated;
};

SGE_INLINE void async::notify(void)
{
    SGE_ASSERT(owner().thread_id() != std::this_thread::get_id());
    m_activated = true;
    owner().wakeup();
}

SGE_END

#endif // SGE_ASYNC_HPP
