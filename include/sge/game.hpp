//
//
#ifndef SGL_GAME_H
#define SGL_GAME_H

#include "SGL_common.h"

#define SGL_GAME_BEGIN SGL_BEGIN namespace Game {
#define SGL_GAME_END } SGL_END

SGL_GAME_BEGIN

namespace Window {
	const Eigen::Vector4i &GetRect(void);
	Uint32 GetFlags(void);
}

namespace Renderer {
	bool IsClearEnabled(void);
	const Eigen::Vector4f &GetClearColor(void);
}

void MakeDefault(SGL_Game *game);
bool Init(const SGL_Game *game);
void Shutdown(void);
void Update(float elapsed);
void Draw(float elapsed);
const char *GetName(void);

SGL_GAME_END

#endif // SGL_GAME_H

