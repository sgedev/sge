//
//
#ifndef SGE_OBJECT_HPP
#define SGE_OBJECT_HPP

#include <SGE/Common.hpp>

SGE_BEGIN

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

SGE_END

#endif // SGE_OBJECT_HPP

