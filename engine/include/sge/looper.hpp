//
//
#ifndef SGE_LOOPER_HPP
#define SGE_LOOPER_HPP

#include <vector>
#include <unordered_map>
#include <chrono>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>

#include <sge/common.hpp>

SGE_BEGIN

class looper {
public:
    using clock = std::chrono::high_resolution_clock;
    using time_point = clock::time_point;
    using duration = clock::duration;

    enum {
        RUN_DEFAULT = 0,
        RUN_ONCE
    };

    class handle {
    public:
        handle(looper &lp);
        virtual ~handle(void);

    public:
        looper &owner(void);
        virtual duration update(void) = 0;

    protected:
        void do_start(void);
        void do_stop(void);

    private:
        looper &m_looper;
        bool m_started;
    };

public:
    virtual ~looper(void);

public:
    void add_handle(handle *ph);
    void remove_handle(handle *ph);
    void run(int mode = RUN_DEFAULT);
    time_point now(void) const;
    void wakeup(void);
    std::thread::id thread_id(void) const;
    static looper *current(void);
    static looper *thread_looper(std::thread::id tid);

protected:
    looper(void);

private:
    looper::duration update_handles(void);

private:
    using looper_map = std::unordered_map<std::thread::id, looper *>;
    using handle_vector = std::vector<handle *>;

    std::thread::id m_thread_id;
    std::mutex m_mutex;
    std::condition_variable_any m_cond;
    handle_vector m_handles;
    time_point m_now;
    bool m_wakeup;
    static looper_map c_looper_map;
    static std::mutex c_looper_map_mutex;
};

SGE_INLINE looper &looper::handle::owner(void)
{
    return m_looper;
}

SGE_INLINE void looper::handle::do_start(void)
{
    SGE_ASSERT(!m_started);
    m_looper.add_handle(this);
    m_started = true;
}

SGE_INLINE void looper::handle::do_stop(void)
{
    SGE_ASSERT(m_started);
    m_looper.remove_handle(this);
    m_started = false;
}

SGE_INLINE looper::time_point looper::now(void) const
{
    return m_now;
}

SGE_INLINE void looper::wakeup(void)
{
    SGE_ASSERT(m_thread_id != std::this_thread::get_id());

    std::lock_guard locker(m_mutex);
    m_wakeup = true;
    m_cond.notify_all();
}

SGE_INLINE std::thread::id looper::thread_id(void) const
{
    return m_thread_id;
}

SGE_INLINE looper *looper::current(void)
{
    return thread_looper(std::this_thread::get_id());
}

SGE_END

#endif // SGE_LOOPER_HPP
