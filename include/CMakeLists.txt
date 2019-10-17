
add_subdirectory(cx cx)
add_subdirectory(GL GL)
add_subdirectory(glm glm)
add_subdirectory(KHR KHR)
add_subdirectory(sge sge)
add_subdirectory(ttvfs ttvfs)

target_sources(sge
	PRIVATE
		glm.hpp
		lauxlib.h
		lua.h
		lua.hpp
		luaconf.h
		lualib.h
		miniz.h
		miniz_common.h
		miniz_tdef.h
		miniz_zip.h
		sge.hpp
		ttvfs.h
		ttvfs_stdio.h
		ttvfs_zip.h
	)

