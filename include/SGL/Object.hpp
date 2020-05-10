//
//
#ifndef SGL_OBJECT_HPP
#define SGL_OBJECT_HPP

#include <SGL/Common.hpp>

SGL_BEGIN

class Object {
public:
	Object(void);
	virtual ~Object(void);

public:
	virtual void update(float elapsed);
	const char *name(void) const;
	void rename(const char *name);
	bool enabled(void) const;
	void enable(void);
	void disable(void);
	bool visibled(void) const;
	void show(void);
	void hide(void);
	const glm::vec3 &pos(void) const;
	void setPos(const glm::vec3 &pos);
	const glm::vec3 &scale(void) const;
	void setScale(const glm::vec3 &scale);
};

SGL_END

#endif // SGL_OBJECT_HPP

