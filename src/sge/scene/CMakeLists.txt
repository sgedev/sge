
# bullet physics

include(FindBullet)
if(BULLET_FOUND)
	sge_include_directories(${BULLET_INCLUDE_DIRS})
	sge_link_libraries(${BULLET_LIBRARIES})
else()
	message(FATAL_ERROR "BulletPhysics not foun.d")
endif()

sge_sources(
	core.cpp
	)
