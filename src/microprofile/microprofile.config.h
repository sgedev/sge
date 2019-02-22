
#include <GL/glew.h>
#include <SGL.h>

#ifndef MICROPROFILE_GPU_TIMERS_GL
#define MICROPROFILE_GPU_TIMERS_GL 1
#endif

#if defined(SGL_PROFILE) && defined(_WIN32)
#pragma comment(lib, "ws2_32.lib")
#endif
