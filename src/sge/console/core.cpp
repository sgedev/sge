//
//
#include <string>

#include <imgui.h>
#include <imgui_dialogs.h>

#include <sge/console.hpp>

SGE_CONSOLE_BEGIN

bool init(void)
{
	return true;
}

void shutdown(void)
{
}

bool handle_event(const SDL_Event &event)
{
	if (event.type == SDL_QUIT) {
		uv_stop(uv_default_loop());
		return true;
	}

	return false;
}

void update(float elapsed)
{
}

void draw(void)
{

}

SGE_CONSOLE_END

