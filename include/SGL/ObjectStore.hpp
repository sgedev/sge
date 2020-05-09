//
//
#ifndef SGL_OBJECTSTORE_HPP
#define SGL_OBJECTSTORE_HPP

#include <SGL/Common.hpp>

SGL_BEGIN

class ObjectStore {
public:
	typedef Uint32 ObjectID;
	typedef Uint32 PropertyID;

	enum {
		PropertyNone = 0,

		PropertyName,
		PropertyPosition,

		PropertyUser
	};

public:
	Property newProperty(const char *name);
	void deleteProperty(Property prop);
	const char *propertyName(Property prop) const;
	ObjectID newObject(const char *name);
	ObjectID dupObject(ObjectID obj);
	void deleteObject(Object obj);
	const char *objectName(Object obj);
	bool setObjectProperty(Object obj, Property prop, bool value);
	bool setObjectProperty(Object obj, Property prop, int value);
	bool setObjectProperty(Object obj, Property prop, bool value);
};

SGL_END

#endif // SGL_OBJECTSTORE_HPP

