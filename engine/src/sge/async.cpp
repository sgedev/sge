//
//
#include <sge/async.hpp>

SGE_BEGIN

async::async(looper &lp):
    looper::handle(lp),
    m_activated(false)
{
    SGE_ASSERT(m_fn);
}

async::~async(void)
{
}

bool async::start(function fn)
{
    m_fn = fn;

    do_start();

    return true;
}

void async::stop(void)
{
    do_stop();
}

looper::duration async::update(void)
{
    if (m_activated) {
        if (m_fn)
            m_fn();
        m_activated = false;
    }

    return looper::duration::max();
}

SGE_END
