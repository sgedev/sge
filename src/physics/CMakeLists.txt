
include(FindBullet)
if(BULLET_FOUND)
	target_include_directories(sge PRIVATE ${BULLET_INCLUDE_DIRS})
	target_link_libraries(sge ${BULLET_LIBRARIES})
else()
	message(FATAL_ERROR "BulletPhysics not found.")
endif()

target_sources(sge
	PRIVATE
		core.cpp
		object.cpp
	)
