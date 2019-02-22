//
//
#include <string>

#include "SGL_game.h"

SGL_GAME_BEGIN

namespace Window {
	Eigen::Vector4i Rect(SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480);
	Uint32 Flags = 0;

	const Eigen::Vector4i &GetRect(void) {
		return Rect;
	}

	Uint32 GetFlags(void) {
		return Flags;
	}

	void Load(const SGL_GameWindow *p) {
		Flags = p->Flags;

		Rect[0] = p->X;
		Rect[1] = p->Y;
		Rect[2] = p->Width;
		Rect[3] = p->Height;

		if (Rect[2] < 1)
			Rect[2] = 0;

		if (Rect[3] < 1)
			Rect[3] = 0;
	}
}

namespace Renderer {
	bool ClearEnabled = true;
	Eigen::Vector4f ClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	bool IsClearEnabled(void) {
		return ClearEnabled;
	}

	const Eigen::Vector4f &GetClearColor(void) {
		return ClearColor;
	}

	void Load(const SGL_GameRenderer *p) {
		ClearEnabled = !!p->Clear;
		ClearColor[0] = p->ClearColor[0];
		ClearColor[1] = p->ClearColor[1];
		ClearColor[2] = p->ClearColor[2];
		ClearColor[3] = p->ClearColor[3];
	}
}

void MakeDefault(SGL_Game *game) {
	SGL_ASSERT(game != NULL);

	memset(game, 0, sizeof(SGL_Game));

	game->Name = "SGL";
	game->Window.X = SDL_WINDOWPOS_CENTERED;
	game->Window.Y = SDL_WINDOWPOS_CENTERED;
	game->Window.Width = 800;
	game->Window.Height = 600;
	game->Window.Flags = 0;
	game->Renderer.Clear = SDL_FALSE;
	game->Funcs.Update = NULL;
	game->Funcs.Draw = NULL;
}

static std::string Name = "SGL";
static SGL_GameFuncs Funcs = { NULL };

bool Init(const SGL_Game *game) {
	if (game == NULL)
		return true;

	if (game->Name != NULL)
		Name = game->Name;

	Funcs = game->Funcs;

	Window::Load(&game->Window);	
	Renderer::Load(&game->Renderer);

	return true;
}

void Shutdown(void) {
}

void Update(float elapsed) {
	if (Funcs.Update != NULL)
		Funcs.Update(elapsed);
}

void Draw(float elapsed) {
	if (Funcs.Draw != NULL)
		Funcs.Draw(elapsed);
}

const char *GetName(void) {
	return Name.c_str();
}

SGL_GAME_END

