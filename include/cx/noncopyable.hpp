//
//
#ifndef CX_NONCOPYABLE_HPP
#define CX_NONCOPYABLE_HPP

#include <cx/common.hpp>

CX_BEGIN

class noncopyable {
public:
	noncopyable(void) { }
	~noncopyable(void) { }

private:
	noncopyable(const Noncopyable &);
	noncopyable &operator=(const Noncopyable &);
};

CX_END

#endif // CX_NONCOPYABLE_HPP

