//
//
#include <stdio.h>

#include <sge/gl.h>
#include <sge/gui.h>
#include <sge/renderer.h>
#include <sge/physics.h>
#include <sge/sound.h>
#include <sge/game.h>

SGE_BEGIN

static bool Run;
static Uint32 CurrentTime;
static Uint32 MinFrameElapsed;
static float FrameElapsed;
static unsigned long FPS;
static bool IsWindowVisibled;

static void HandleWindowEvent(const SDL_WindowEvent *event)
{
	SGE_ASSERT(event != NULL);

	if (event->windowID != GL::GetWindowID())
		return;

	switch (event->event) {
	case SDL_WINDOWEVENT_EXPOSED:
	case SDL_WINDOWEVENT_SHOWN:
		IsWindowVisibled = true;
		break;
	case SDL_WINDOWEVENT_HIDDEN:
	case SDL_WINDOWEVENT_MINIMIZED:
		IsWindowVisibled = false;
		break;
	}
}

static void HandleEvent(const SDL_Event *event)
{
	SGE_ASSERT(event != NULL);

	switch (event->type) {
	case SDL_WINDOWEVENT:
		HandleWindowEvent(&event->window);
		break;
	case SDL_QUIT:
		Run = false;
		break;
	}

	GUI::HandleEvent(event);
	Renderer::HandleEvent(event);
	Physics::HandleEvent(event);
	Sound::HandleEvent(event);
	Game::HandleEvent(event);
}

static void UpdateWindow(void)
{
	if (!IsWindowVisibled)
		return;

	GL::DrawBegin();

	Game::Draw();
	GUI::Draw();
	Renderer::Draw();

	GL::DrawEnd();
}

static void MainLoop(void)
{
	FPS = 0;
	Run = true;
	CurrentTime = SDL_GetTicks();
	FrameElapsed = 0.0f;

	SDL_Event event;
	Uint32 elapsed;
	Uint32 last = CurrentTime;
	Uint32 fps_last = CurrentTime;
	unsigned long fps_count = 0;

	SGE_LOGD("Main loop started.\n");

	while (Run) {
		while (SDL_PollEvent(&event))
			HandleEvent(&event);

		CurrentTime = SDL_GetTicks();
		elapsed = CurrentTime - last;
		if (elapsed < MinFrameElapsed) {
			SDL_Delay(1);
			continue;
		}

		FrameElapsed = float(elapsed) / 1000.0f;

		Physics::Update();
		Renderer::Update();
		Sound::Update();
		Game::Update();

		UpdateWindow();

		last = SDL_GetTicks();
		fps_count++;

		if (fps_last < last && (last - fps_last) >= 250) {
			FPS = fps_count << 2;
			fps_last = last;
			fps_count = 0;
		}
	}

	SGE_LOGD("Main loop exited.\n");
}

static void ShowInfo(void)
{
	SGE_LOGI("SGE %d.%d.%d Copyright(C) 2019\n",
		SGE_VERSION_MAJOR, SGE_VERSION_MINOR, SGE_VERSION_PATCH);

	// TODO print licenses.
	SGE_LOGI("<<LICENSES>>\n");

#ifdef SGE_DEBUG
	SGE_LOGI("Debug enabled.\n");
#endif

	SGE_LOGI("Game: \"%s\"\n", SGE_GAME_NAME);
}

static int Main(int argc, char *argv[])
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
		return -1;

#ifdef SGE_DEBUG
	SDL_LogSetAllPriority(SDL_LOG_PRIORITY_DEBUG);
#endif

	ShowInfo();

	SetMaxFPS(60);

	Game::PreInit();

	GL::Init();
	Renderer::Init();
	GUI::Init();
	Physics::Init();
	Sound::Init();
	Game::Init();

	MainLoop();

	Game::Shutdown();
	Sound::Shutdown();
	Physics::Shutdown();
	GUI::Shutdown();
	Renderer::Shutdown();
	GL::Shutdown();

	Game::PostShutdown();

	SDL_Quit();

	return 0;
}

Uint32 GetNow(void)
{
	return CurrentTime;
}

float GetElapsed(void)
{
	return FrameElapsed;
}

unsigned long GetFPS(void)
{
	return FPS;
}

unsigned long GetMaxFPS(void)
{
	return 1000 / MinFrameElapsed;
}

void SetMaxFPS(unsigned long v)
{
	SGE_ASSERT(v > 0);
	SGE_LOGD("Set max fps to %d\n", v);
	MinFrameElapsed = 1005 / v;
}

SGE_END

int main(int argc, char *argv[])
{
	return SGE::Main(argc, argv);
}
