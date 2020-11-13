//
//
#include <algorithm>

#include <sge/looper.hpp>

SGE_BEGIN

// looper::handle

looper::handle::handle(looper &lp):
    m_looper(lp),
    m_started(false)
{
}

looper::handle::~handle(void)
{
}

// looper

looper::looper_map looper::c_looper_map;
std::mutex looper::c_looper_map_mutex;

looper::looper(void):
    m_thread_id(std::this_thread::get_id()),
    m_now(clock::now()),
    m_wakeup(false)
{
}

looper::~looper(void)
{
    std::lock_guard locker(c_looper_map_mutex);
    SGE_ASSERT(c_looper_map.find(m_thread_id)->second == this);
    c_looper_map.erase(m_thread_id);
}

void looper::add_handle(handle *ph)
{
    SGE_ASSERT(std::this_thread::get_id() == m_thread_id);

#ifdef SGE_DEBUG
    auto it(std::find(m_handles.begin(), m_handles.end(), ph));
    SGE_ASSERT(it == m_handles.end());
#endif

    m_handles.push_back(ph);
}

void looper::remove_handle(handle *ph)
{
    SGE_ASSERT(std::this_thread::get_id() == m_thread_id);

    auto it(std::find(m_handles.begin(), m_handles.end(), ph));
    SGE_ASSERT(it != m_handles.end());
    (*it) = nullptr;
}

void looper::run(int mode)
{
    SGE_ASSERT(std::this_thread::get_id() == m_thread_id);

    std::lock_guard locker(m_mutex);

    do {
        m_now = clock::now();
        auto timeout = update_handles();
        if (m_wakeup) {
            timeout = duration(0);
            m_wakeup = false;
        }
        if (timeout < duration::max())
            m_cond.wait_for(m_mutex, timeout);
        else
            m_cond.wait(m_mutex);
    } while (mode == RUN_DEFAULT);
}

looper *looper::thread_looper(std::thread::id tid)
{
    std::lock_guard locker(c_looper_map_mutex);

    auto it = c_looper_map.find(tid);
    if (it != c_looper_map.end())
        return it->second;

    if (tid != std::this_thread::get_id())
        return nullptr;

    auto p = new looper();
    if (p == nullptr)
        return nullptr;

    c_looper_map[tid] = p;

    return p;
}

looper::duration looper::update_handles(void)
{
    SGE_ASSERT(std::this_thread::get_id() == m_thread_id);

    duration timeout = duration::max();

    for (size_t i = 0; i < m_handles.size(); ++i) {
        auto ph = m_handles.at(i);
        if (ph != nullptr) {
            auto ret = ph->update();
            if (ret < timeout)
                timeout = ret;
        }
    }

    auto it(m_handles.begin());
    while (it != m_handles.end()) {
        if ((*it) != nullptr)
            ++it;
        else
            it = m_handles.erase(it);
    }

    return timeout;
}

SGE_END
