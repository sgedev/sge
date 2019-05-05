
include(FindBullet)
if(BULLET_FOUND)
	sge_include_directories(${BULLET_INCLUDE_DIRS})
	sge_link_libraries(${BULLET_LIBRARIES})
else()
	message(FATAL_ERROR "BulletPhysics not found.")
endif()

sge_subdirectories(
	io
	player
	)

sge_sources(
	base.cpp
	camera.cpp
	loader.cpp
	node.cpp
	physics.cpp
	player.cpp
	simple.cpp
	)

