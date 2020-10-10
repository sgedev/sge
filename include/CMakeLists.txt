
target_sources(sge
	PRIVATE
		argh.h
		fontstash.h
		nanovg.h
		nanovg_gl.h
		nanovg_gl_utils.h
		lauxlib.h
		lua.h
		lua.hpp
		luaconf.h
		lualib.h
		physfs.h
		stb_image.h
		stb_truetype.h
		uv.h
	)

add_subdirectory(GL GL)
add_subdirectory(glm glm)
add_subdirectory(rttr rttr)
add_subdirectory(sge sge)
add_subdirectory(uv uv)
