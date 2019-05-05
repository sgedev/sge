//
//
#include <memory>

#define SDL_MAIN_HANDLED
#include <SDL.h>

#include <sge/game.hpp>

extern sge::game *sge_game_new(uv_loop_t *loop);

struct SDL_Initializer {
	int InitResult;

	SDL_Initializer(void)
	{
		InitResult = SDL_Init(SDL_INIT_EVERYTHING);
	}

	~SDL_Initializer(void)
	{
		if (InitResult == 0)
			SDL_Quit();
	}

	operator bool(void) const
	{
		return (InitResult == 0);
	}
};

int main(int argc, char *argv[])
{
	SDL_SetMainReady();

	SDL_Initializer SDL;
	if (!SDL)
		return EXIT_FAILURE;

#ifdef SGE_DEBUG
	SDL_LogSetAllPriority(SDL_LOG_PRIORITY_DEBUG);
#endif

	std::unique_ptr<sge::game> game(sge_game_new(uv_default_loop()));
	if (!game || !game->start())
		return EXIT_FAILURE;

	uv_run(uv_default_loop(), UV_RUN_DEFAULT);
	game->stop();

	return EXIT_SUCCESS;
}
