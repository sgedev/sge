//
//
#ifndef SGE_OBJECT_HPP
#define SGE_OBJECT_HPP

#include <cx/noncopyable.hpp>

#include <sge/common.hpp>

SGE_BEGIN

class Object: cx::noncopyable {
public:
	Object(void);
	virtual ~Object(void);
};

SGE_END

#endif // SGE_OBJECT_HPP
