//
//
#ifndef SGE_SCENE_HPP
#define SGE_SCENE_HPP

#include <memory>

#include <cx/signals.hpp>

#include <SGE/Common.hpp>
#include <SGE/Object.hpp>
#include <SGE/Camera.hpp>

SGE_BEGIN

typedef std::shared_ptr<Object> ObjectPtr;
typedef std::shared_ptr<Camera> CameraPtr;

struct Scene {
	cx::signal<void(ObjectPtr &)> addObject;
	cx::signal<void(ObjectPtr &)> removeObject;
	cx::signal<void(CameraPtr &)> addCamera;
	cx::signal<void(CameraPtr &)> removeCamera;
};

SGE_END

#endif // SGE_SCENE_HPP
