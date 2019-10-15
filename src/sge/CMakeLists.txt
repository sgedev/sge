
target_sources(sge
	PRIVATE
		entity.cpp
		game.cpp
		input.cpp
		keycode.cpp
		scene.cpp
		view.cpp
	)

if(UNIX)
target_compile_options(sge PUBLIC "-pthread")
target_link_libraries(sge "pthread")
endif()

