//
//
#ifndef SGE_CAMERA_HPP
#define SGE_CAMERA_HPP

#include <cx/signals.hpp>

#include <SGE/Common.hpp>
#include <SGE/Object.hpp>

SGE_BEGIN

struct Camera: public Object {
	cx::signal<void (float)> setFov;
	cx::signal<void (int, int, int, int)> setViewport;
	cx::signal<void (const glm::mat4 &)> setProjection;
};

SGE_END

#endif // SGE_CAMERA_HPP

