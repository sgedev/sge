
include(FindBullet)
if(BULLET_FOUND)
	include_directories(${BULLET_INCLUDE_DIRS})
	link_libraries(${BULLET_LIBRARIES})
else()
	message(FATAL_ERROR "BulletPhysics not found.")
endif()

target_sources(sge
	PRIVATE
		core.cpp
		object.cpp
	)

