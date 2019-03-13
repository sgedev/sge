//
//
#ifndef SGE_GAME_H
#define SGE_GAME_H

#include <sge/common.h>
#include <sge/fs.h>
#include <sge/gl.h>
#include <sge/gui.h>
#include <sge/renderer.h>
#include <sge/physics.h>
#include <sge/sound.h>

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

bool preinit(void);
bool postinit(void);
void preshutdown(void);
void postshutdown(void);
bool should_quit(void);
void update(void);
void draw(void);
void handle_event(const SDL_Event *event);

SGE_GAME_END

#endif // SGE_GAME_H
