
sge_subdirectories(
	cx
	GL
	KHR
	sge
	)

sge_sources(
	fontstash.h
	lauxlib.h
	lua.h
	lua.hpp
	luaconf.h
	lualib.h
	luasge.h
	nanovg.h
	nanovg_gl.h
	nanovg_gl_utils.h
	physfs.h
	stb_image.h
	stb_truetype.h
	)

sge_compile_definitions(LUA_USER_H=\"${CMAKE_CURRENT_SOURCE_DIR}/luasge.h\")
