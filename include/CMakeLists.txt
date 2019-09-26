
add_subdirectory(cx cx)
add_subdirectory(GL GL)
add_subdirectory(GLEX GLEX)
add_subdirectory(glm glm)
add_subdirectory(KHR KHR)
add_subdirectory(sge sge)

target_sources(sge
	PUBLIC
		glex.hpp
		glm.hpp
		sge.hpp
	PRIVATE
		lauxlib.h
		lua.h
		lua.hpp
		luaconf.h
		lualib.h
	)

