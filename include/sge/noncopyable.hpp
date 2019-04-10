//
//
#ifndef SGE_NONCOPYABLE_HPP
#define SGE_NONCOPYABLE_HPP

#include <sge/common.hpp>

SGE_BEGIN

class noncopyable {
public:
	noncopyable(void) = default;
	virtual ~noncopyable(void) = default;

private:
	noncopyable(const noncopyable &) = default;
	noncopyable &operator=(const noncopyable &) = default;
};

SGE_END

#endif  // SGE_NONCOPYABLE_HPP

