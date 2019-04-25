//
//
#ifndef SGE_SCENE_HPP
#define SGE_SCENE_HPP

#include <sge/db.hpp>
#include <sge/scene/common.hpp>
#include <sge/scene/camera.hpp>

SGE_SCENE_BEGIN

enum {
	STATE_IDLE = 0,
	STATE_LOADING,
	STATE_READY,
	STATE_CANCELING
};

bool init(void);
void shutdown(void);
void update(float elapsed);
void draw(void);
int state(void);
void reset(void);
bool start_loading(db::node node);
bool cancel_loading(void);
int loading_percentage(void);

SGE_SCENE_END

#endif // SGE_SCENE_HPP

