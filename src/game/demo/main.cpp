//
//
#include "demo.hpp"

int main(int argc, char *argv[])
{
	SDL_Init(SDL_INIT_EVERYTHING);

	demo *pd = new demo();

	pd->start(uv_default_loop());

	uv_run(uv_default_loop(), UV_RUN_DEFAULT);

	pd->stop();

	delete pd;

	SDL_Quit();

	return 0;
}

