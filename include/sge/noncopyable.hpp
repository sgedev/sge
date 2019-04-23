//
//
#ifndef SGE_NONCOPYABLE_HPP
#define SGE_NONCOPYABLE_HPP

#include <sge/common.hpp>

SGE_BEGIN

class noncopyable {
private:
	noncopyable(const noncopyable &);
	noncopyable &operator=(const noncopyable&);
};

SGE_END

#endif  // SGE_NONCOPYABLE_HPP

