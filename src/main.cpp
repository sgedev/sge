//
//
#include <stdio.h>

#include "SGL_common.h"
#include "SGL_gl.h"
#include "SGL_gui.h"
#include "SGL_renderer.h"
#include "SGL_physics.h"
#include "SGL_sound.h"
#include "SGL_scene.h"
#include "SGL_game.h"
#include "SGL_object.h"

#ifdef SGL_PROFILE
#	include "microprofile/microprofile.h"
#endif

SGL_BEGIN

static bool IsWindowVisibled;

static void HandleWindowEvent(const SDL_WindowEvent *event)
{
	SGL_ASSERT(event != NULL);

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

static void PrintVersion(void)
{
	SGL_LOGI("SGL v%d.%d.%d initializing...\n",
		SGL_VERSION_MAJOR, SGL_VERSION_MINOR, SGL_VERSION_PATCH);
}

static bool Init(const SGL_Game *game)
{
	PrintVersion();

	Game::Init(game);
	GL::Init();
	Renderer::Init();
	GUI::Init();
	Physics::Init();
	Sound::Init();
	Scene::Init();

#ifdef SGL_PROFILE
	MicroProfileOnThreadCreate("Main");
	MicroProfileSetEnableAllGroups(true);
	MicroProfileSetForceMetaCounters(true);
	MicroProfileGpuInitGL();
#endif

	return true;
}

static void Shutdown(void)
{
	Scene::Shutdown();
	Sound::Shutdown();
	Physics::Shutdown();
	GUI::Shutdown();
	Renderer::Shutdown();
	GL::Shutdown();
	Game::Shutdown();

	SGL_LOGI("SGL shutdown.\n");
}

static void Frame(float elapsed)
{
#ifdef SGL_PROFILE
	MICROPROFILE_SCOPEI("Frame", "Frame", MP_WHEAT);
#endif

	Physics::Update(elapsed);
	Sound::Update(elapsed);
	Game::Update(elapsed);
	Scene::Update(elapsed);

	if (IsWindowVisibled) {
		GL::DrawBegin();

		Game::Draw(elapsed);
		Scene::Draw(elapsed);
		GUI::Draw(elapsed);
		Renderer::Draw(elapsed);

		GL::DrawEnd();
	}

#ifdef SGL_PROFILE
	MicroProfileFlip(0);
#endif
}

static void HandleEvent(const SDL_Event *event)
{
	SGL_ASSERT(event != NULL);

	switch (event->type) {
	case SDL_MOUSEMOTION:
		break;
	case SDL_MOUSEBUTTONDOWN:
		break;
	case SDL_MOUSEBUTTONUP:
		break;
	case SDL_MOUSEWHEEL:
		break;
	case SDL_KEYDOWN:
		break;
	case SDL_KEYUP:
		break;
	case SDL_WINDOWEVENT:
		HandleWindowEvent(&event->window);
		break;
	}

	GUI::HandleEvent(event);
	Renderer::HandleEvent(event);
	Physics::HandleEvent(event);
	Sound::HandleEvent(event);
	Scene::HandleEvent(event);
}

static void AddObject(Object *obj)
{
	
}

static void RemoveObject(Object *obj)
{
}

SGL_END

struct SGL_ObjectStruct {
	SGL_ObjectStruct(const SGL_ObjectType *type): obj(type) { }
	SGL::Object obj;
	const SGL_ObjectType *type;
};

SGL_API void SGL_MakeDefaultGame(SGL_Game *game)
{
	SGL::Game::MakeDefault(game);
}

SGL_API int SGL_Init(const SGL_Game *game)
{
	return SGL::Init(game) ? 0 : -1;
}

SGL_API void SGL_Quit(void)
{
	SGL::Shutdown();
}

SGL_API void SGL_Frame(float elapsed)
{
	SGL::Frame(elapsed);
}

SGL_API void SGL_HandleEvent(const SDL_Event *event)
{
	SGL::HandleEvent(event);
}

SGL_API SDL_Window *SGL_GetMainWindow(void)
{
	return SGL::GL::GetWindow();
}

SGL_API SDL_GLContext SGL_GetGLContext(void)
{
	return SGL::GL::GetContext();
}

SGL_API SGL_Object *SGL_CreateObject(const char *name, const SGL_ObjectType *type)
{
	SGL_Object *obj = new SGL_Object(type);
	if (obj) {
		obj->obj.SetName(name);
		obj->type = type;
	}

	return obj;
}

SGL_API void SGL_DestroyObject(SGL_Object *obj)
{
	if (obj != NULL)
		delete obj;
}

SGL_API const SGL_ObjectType *SGL_GetObjectType(SGL_Object *obj)
{
	if (obj == NULL)
		return NULL;

	return obj->type;
}

SGL_API void SGL_AddObject(SGL_Object *obj)
{
	if (obj != NULL)
		SGL::AddObject(&obj->obj);
}

SGL_API void SGL_RemoveObject(SGL_Object *obj)
{
	if (obj != NULL)
		SGL::RemoveObject(&obj->obj);
}
