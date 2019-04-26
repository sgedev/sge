//
//
#include <sge/gui.hpp>
#include <sge/console.hpp>

SGE_CONSOLE_BEGIN

static gui::window s_window;

bool init(void)
{
	s_window.create("Console", 50, 50, 480, 320, NULL, gui::window::FLAG_NO_COLLAPSE);
	s_window.show();

	return true;
}

void shutdown(void)
{
	s_window.destroy();
}

void enable(void)
{
	s_window.show();
}

void disable(void)
{
	s_window.hide();
}

SGE_CONSOLE_END

