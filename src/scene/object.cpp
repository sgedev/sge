//
//
#include "SGL_object.h"

SGL_BEGIN

Object::Object(const SGL_ObjectType *type)
	: m_RendererObject(type)
	, m_PhysicsObject(type)
{
}

Object::~Object(void)
{
}

SGL_END
