//
//
#ifndef SGE_RENDERER_HPP
#define SGE_RENDERER_HPP

#include <QObject>

#include <SGE/Common.hpp>
#include <SGE/Object.hpp>

SGE_BEGIN

class Q_DECL_EXPORT Renderer: public QObject {
	Q_OBJECT

public:
	Renderer(QObject *parent = Q_NULLPTR);
	virtual ~Renderer(void);

public:
	virtual void addObject(ObjectPtr obj);
	virtual void removeObject(ObjectPtr obj);

private:
};

SGE_END

#endif // SGE_RENDERER_HPP
