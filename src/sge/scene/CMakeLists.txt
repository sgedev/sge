
include(FindBullet)
if(BULLET_FOUND)
	sge_include_directories(${BULLET_INCLUDE_DIRS})
	sge_link_libraries(${BULLET_LIBRARIES})
else()
	message(FATAL_ERROR "bullet not found.")
endif()

sge_subdirectories(
	renderer
	)

sge_sources(
	camera.cpp
	core.cpp
	)

