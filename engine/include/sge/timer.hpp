//
//
#ifndef SGE_TIMER_HPP
#define SGE_TIMER_HPP

#include <functional>

#include <sge/common.hpp>
#include <sge/looper.hpp>

SGE_BEGIN

class timer: public looper::handle {
public:
    using function = std::function<void (void)>;

public:
    timer(looper &lp);
    ~timer(void) override;

public:
    bool start(function fn, looper::duration timeout, looper::duration repeat = looper::duration::min());
    void stop(void);
    looper::duration update(void) override;

private:
    function m_fn;
    looper::time_point m_expire;
    looper::duration m_repeat;
};

SGE_END

#endif // SGE_TIMER_HPP
