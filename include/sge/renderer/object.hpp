//
//
#ifndef SGE_RENDERER_OBJECT_HPP
#define SGE_RENDERER_OBJECT_HPP

#include <sge/renderer/common.hpp>

SGE_RENDERER_BEGIN

class Object {
public:
	Object(const SGE_ObjectType *type);
	virtual ~Object(void);
};

SGE_RENDERER_END

#endif // SGE_RENDERER_OBJECT_HPP
