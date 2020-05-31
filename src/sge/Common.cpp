//
//
#include <SGE/Common.hpp>

SGE_BEGIN

void logWrite(Log type, const char *fmt, va_list args)
{
#ifndef SGE_DEBUG
	if (type == LogDebug)
		return;
#endif

	if (type == LogError || type == LogWarning)
		vfprintf(stderr, fmt, args);
	else
		vfprintf(stdout, fmt, args);
}

SGE_END
