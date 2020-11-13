//
//
#ifndef SGE_SPIN_LOCK_HPP
#define SGE_SPIN_LOCK_HPP

#include <atomic>

#include <sge/common.hpp>

SGE_BEGIN

class spin_lock final: public noncopyable {
public:
    spin_lock(void);

public:
    bool try_lock(void);
    void lock(void);
    void unlock(void);

private:
    std::atomic_flag m_flag;
};

SGE_INLINE spin_lock::spin_lock(void)
{
    m_flag.clear();
}

SGE_INLINE bool spin_lock::try_lock(void)
{
    return !m_flag.test_and_set();
}

SGE_INLINE void spin_lock::lock(void)
{
    while (m_flag.test_and_set());
}

SGE_INLINE void spin_lock::unlock(void)
{
    m_flag.clear();
}

SGE_END

#endif // SGE_SPIN_LOCK_HPP
