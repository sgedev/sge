//
//
#ifndef SGL_OBJECT_H
#define SGL_OBJECT_H

#include <string>

#include "SGL_common.h"
#include "SGL_renderer_object.h"
#include "SGL_physics_object.h"

SGL_BEGIN

class Object {
public:
	Object(const SGL_ObjectType *type);
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

SGL_END

#endif // SGL_OBJECT_H
