//
//
#include <cstdarg>

#include <sge/common.hpp>

extern "C" void sge_log(SDL_LogPriority prio, const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	SDL_LogMessageV(SDL_LOG_CATEGORY_APPLICATION, prio, fmt, args);
	va_end(args);
}
