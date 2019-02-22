//
//
#include <stdio.h>

#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SGL.h>

static bool run;

static void HandleEvent(const SDL_Event *event)
{
	switch (event->type) {
	case SDL_QUIT:
		run = false;
		break;
	}

	SGL_HandleEvent(event);
}

static void MakeGame(SGL_Game *game)
{
	SGL_MakeDefaultGame(game);

	game->Name = "SGL Test";

	game->Window.X = SDL_WINDOWPOS_CENTERED;
	game->Window.Y = SDL_WINDOWPOS_CENTERED;
	game->Window.Width = 640;
	game->Window.Height = 480;
	game->Window.Flags = SDL_WINDOW_RESIZABLE;

	game->Renderer.Clear = SDL_TRUE;
	game->Renderer.ClearColor[0] = 0.3f;
	game->Renderer.ClearColor[1] = 0.3f;
	game->Renderer.ClearColor[2] = 0.3f;
	game->Renderer.ClearColor[3] = 1.0f;
}

int main(int argc, char *argv[])
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
		return -1;

	SDL_LogSetAllPriority(SDL_LOG_PRIORITY_DEBUG);

	SGL_Game game;
	MakeGame(&game);
	SGL_Init(&game);

	run = true;

	Uint32 pass;
	Uint32 base = SDL_GetTicks();
	SDL_Event event;

	while (run) {
		while (SDL_PollEvent(&event))
			HandleEvent(&event);
		pass = SDL_GetTicks() - base;
		if (pass > 16) {
			SGL_Frame(float(pass) / 1000.0f);
			base = SDL_GetTicks();
		} else
			SDL_Delay(1);
	}

	SGL_Quit();
	SDL_Quit();

	return 0;
}

