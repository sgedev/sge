//
//
#ifndef CX_NONCOPYABLE_HPP
#define CX_NONCOPYABLE_HPP

#include <cx/common.hpp>

CX_BEGIN

class noncopyable {
private:
	noncopyable(const noncopyable &) = delete;
	noncopyable &operator=(const noncopyable &) = delete;
};

CX_END

#endif // CX_NONCOPYABLE_HPP
