//
//
#ifndef CX_NONCOPYABLE_HPP
#define CX_NONCOPYABLE_HPP

#include <cx/common.hpp>

CX_BEGIN

class Noncopyable {
public:
	Noncopyable(void) { }
	~Noncopyable(void) { }

private:
	Noncopyable(const Noncopyable &);
	Noncopyable &operator=(const Noncopyable &);
};

CX_END

#endif // CX_NONCOPYABLE_HPP

