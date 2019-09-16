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
	noncopyable(const noncopyable &);
	noncopyable &operator=(const noncopyable &);
};

CX_END

#endif // CX_NONCOPYABLE_HPP

