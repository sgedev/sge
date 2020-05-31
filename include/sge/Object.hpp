//
//
#ifndef SGE_OBJECT_HPP
#define SGE_OBJECT_HPP

#include <string>

#include <cx/signals.hpp>

#include <SGE/Common.hpp>

SGE_BEGIN

struct Object {
	cx::signal<void (bool)> setEnable;
	cx::signal<void (bool)> setVisible;
	cx::signal<void (const std::string &)> setName;
	cx::signal<void (const glm::vec3 &)> setPosition;
	cx::signal<void (const glm::vec3 &)> setScale;
	cx::signal<void (const glm::quat &)> setRotation;
	cx::signal<void (const std::string &, const std::string &)> setProperty;
};

SGE_END

#endif // SGE_OBJECT_HPP
