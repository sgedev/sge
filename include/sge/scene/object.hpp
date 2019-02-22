//
//
#ifndef SGE_OBJECT_H
#define SGE_OBJECT_H

#include <string>

#include "SGE_common.h"
#include "SGE_renderer_object.h"
#include "SGE_physics_object.h"

SGE_BEGIN

class Object {
public:
	Object(const SGE_ObjectType *type);
	virtual ~Object(void);

public:
	void SetName(const char *name);
	const char *GetName(void) const;

private:
	Renderer::Object m_RendererObject;
	Physics::Object m_PhysicsObject;
	std::string m_Name;
};

inline void Object::SetName(const char *name)
{
	m_Name = name;
}

inline const char *Object::GetName(void) const
{
	return m_Name.c_str();
}

SGE_END

#endif // SGE_OBJECT_H
