//
//
#ifndef SGE_SCENE_WORLD_HPP
#define SGE_SCENE_WORLD_HPP

#include <sge/scene/common.hpp>

SGE_SCENE_BEGIN

class world {
public:
	world(void);
	virtual ~world(void);

public:
	bool init(void);
	void update(float elapsed);
	void handle_event(const SDL_Event &evt);
};

SGE_SCENE_END

#endif // SGE_SCENE_WORLD_HPP
