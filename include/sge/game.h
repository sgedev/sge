//
//
#ifndef SGE_GAME_H
#define SGE_GAME_H

#include <sge/common.h>

#define SGE_GAME_BEGIN SGE_BEGIN namespace game {
#define SGE_GAME_END } SGE_END

SGE_GAME_BEGIN

namespace helper {
	namespace fps {
		bool init(void);
		void shutdown(void);
		void update(void);
		void handle_event(const SDL_Event *event);
	}
}

bool pre_init(void);
bool init(void);
void shutdown(void);
void post_shutdown(void);
void update(void);
void draw(void);
void handle_event(const SDL_Event *event);

SGE_GAME_END

#endif // SGE_GAME_H
