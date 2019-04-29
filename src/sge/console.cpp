//
//
#include <sge/renderer.hpp>
#include <sge/console.hpp>

SGE_CONSOLE_BEGIN

bool init(void)
{
	return true;
}

void shutdown(void)
{

}

void update(void)
{
}

void draw(void)
{
	ImGui::Text("fps %d", fps());
}

void enable(void)
{

}

void disable(void)
{

}

SGE_CONSOLE_END

