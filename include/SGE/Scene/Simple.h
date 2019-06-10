//
//
#ifndef SGE_SCENE_SIMPLE_H
#define SGE_SCENE_SIMPLE_H

#include <SGE/Scene/Common.h>
#include <SGE/Scene/Base.h>

SGE_SCENE_BEGIN

class Simple: public Base {
	Q_OBJECT

public:
	Simple(QObject *parent = Q_NULLPTR);
	virtual ~Simple(void);

public:
	virtual void update(float elapsed);
	virtual void draw(Renderer::Base *target);
};

SGE_SCENE_END

#endif // SGE_SCENE_SIMPLE_H

