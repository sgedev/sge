//
//
#include <sge/timer.hpp>

SGE_BEGIN

timer::timer(looper &lp):
    looper::handle(lp),
    m_expire(looper::time_point::max()),
    m_repeat(looper::duration::min())
{
}

timer::~timer(void)
{
}

bool timer::start(std::function<void (void)> fn, looper::duration timeout, looper::duration repeat)
{
    m_fn = fn;
    m_expire = owner().now() + timeout;
    m_repeat = repeat;

    do_start();

    return true;
}

void timer::stop(void)
{
    m_expire = looper::time_point::max();
    m_repeat = looper::duration::min();

    do_stop();
}

looper::duration timer::update(void)
{
    auto now = owner().now();
    if (now >= m_expire) {
        if (m_fn)
            m_fn();
        if (m_repeat != looper::duration::min())
            m_expire = now + m_repeat;
        else
            stop();
        return looper::duration::max();
    }

    return m_expire - now;
}

SGE_END
