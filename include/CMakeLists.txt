
add_subdirectory(cx cx)
add_subdirectory(filesystem filesystem)
add_subdirectory(GL GL)
add_subdirectory(GLEX GLEX)
add_subdirectory(glm glm)
add_subdirectory(KHR KHR)
add_subdirectory(sge sge)

target_sources(sge
	PRIVATE
		glex.hpp
		glm.hpp
		lauxlib.h
		lua.h
		lua.hpp
		luaconf.h
		lualib.h
		miniz.h
		miniz_common.h
		miniz_export.h
		miniz_tdef.h
		miniz_tinfl.h
		miniz_zip.h
		nanovg.h
		nanovg_gl.h
		nanovg_gl_utils.h
		pugiconfig.hpp
		pugixml.hpp
		sge.hpp
	)

