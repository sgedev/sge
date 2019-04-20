//
//
#ifndef SGE_GL_DRAWABLE_HPP
#define SGE_GL_DRAWABLE_HPP

#include <sge/gl/common.hpp>
#include <sge/gl/object.hpp>

SGE_GL_BEGIN

class drawable: public object {
public:
	drawable(void);
	virtual ~drawable(void);
};

SGE_GL_END

#endif // SGE_GL_DRAWABLE_HPP
