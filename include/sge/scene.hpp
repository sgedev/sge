//
//
#ifndef SGE_SCENE_HPP
#define SGE_SCENE_HPP

#include <sge/common.hpp>
#include <sge/view.hpp>

SGE_BEGIN

class Scene {
public:
	Scene(void);
	virtual ~Scene(void);

public:
	bool init(void);
	void shutdown(void);
	void update(float elapsed);
	void draw(View *v);
};

SGE_END

#endif // SGE_SCENE_HPP

