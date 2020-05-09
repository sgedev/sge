//
//
#ifndef SGL_OBJECT_HPP
#define SGL_OBJECT_HPP

#include <SGL/Common.hpp>
#include <SGL/ObjectStore.hpp>

SGL_BEGIN

class Object {
public:
	typedef Uint32 ObjectID;
	typedef Uint32 PropertyID;

	class Store {
	public:
		Store(void);
		virtual ~Store(void);

	public:
		PropertyID newProperty(const char *name);
		void deleteProperty(PropertyID prop);
		const char propertyName(PropertyID prop);
		ObjectID newObject(const char *name);
		ObjectID dupObject(const char *name);
		void deleteObject(ObjectID obj);
		const const char *objectName(Object ID obj);
	};

public:
	Object(Store *store = NULL);
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

protected:
	Store *store(void);

private:
	Store *m_store;
	ObjectStore::Object m_id;
};

SGL_END

#endif // SGL_OBJECT_HPP

