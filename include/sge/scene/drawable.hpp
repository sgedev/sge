//
//
#ifndef SGE_SCENE_DRAWABLE_HPP
#define SGE_SCENE_DRAWABLE_HPP

#include <sge/scene/common.hpp>
#include <sge/scene/object.hpp>

SGE_SCENE_BEGIN

class drawable: public object {
public:
	drawable(void);
	virtual ~drawable(void);
};

SGE_SCENE_END

#endif // SGE_SCENE_DRAWABLE_HPP

