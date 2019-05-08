//
//
#ifndef SGE_SCENE_HPP
#define SGE_SCENE_HPP

#include <sge/db.hpp>
#include <sge/scene/common.hpp>
#include <sge/scene/camera.hpp>

SGE_SCENE_BEGIN

namespace loading {
	bool start(db::node node);
	bool is_started(void);
	void stop(void);
	int percentage(void);
}

bool init(void);
void shutdown(void);
void update(float elapsed);
void draw(void);
void reset(void);

SGE_SCENE_END

#endif // SGE_SCENE_HPP

