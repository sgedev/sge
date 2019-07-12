//
//
#ifndef SGE_SCENE_BASE_H
#define SGE_SCENE_BASE_H

#include <QObject>

#include <SGE/Renderer.h>
#include <SGE/Scene/Common.h>

typedef SGE::Renderer::Base tbase;

SGE_SCENE_BEGIN

class Base: public QObject {
	Q_OBJECT

public:
	Base(QObject *parent = Q_NULLPTR);
	virtual ~Base(void);

public:
	virtual void update(float elapsed);
	virtual void draw(Renderer::Base *target);
};

SGE_SCENE_END

#endif // SGE_SCENE_BASE_H

