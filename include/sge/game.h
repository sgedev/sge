//
//
#ifndef SGE_GAME_H
#define SGE_GAME_H

#include <sge/common.h>

#define SGE_GAME_BEGIN SGE_BEGIN namespace Game {
#define SGE_GAME_END } SGE_END

SGE_GAME_BEGIN

namespace Helper {
	namespace FPS {
		bool init(void);
		void shutdown(void);
		void update(void);
		void handle_event(const SDL_Event *event);
	}
}

bool PreInit(void);
bool Init(void);
void Shutdown(void);
void PostShutdown(void);
void Update(void);
void Draw(void);
void HandleEvent(const SDL_Event *event);

SGE_GAME_END

#endif // SGE_GAME_H
