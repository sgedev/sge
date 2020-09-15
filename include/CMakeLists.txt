
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
		sge.hpp
		stb_image.h
		stb_truetype.h
	)

add_subdirectory(cx cx)
add_subdirectory(GL GL)
add_subdirectory(glm glm)
add_subdirectory(rttr rttr)
add_subdirectory(sge sge)
