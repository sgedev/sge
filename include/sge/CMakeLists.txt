
target_sources(sge
	PUBLIC
		common.hpp
		entity.hpp
		event.hpp
		filesystem.hpp
		game.h
		game.hpp
		keycode.hpp
		scene.hpp
		view.hpp
	PRIVATE
		config.hpp.in
	)

configure_file(
	config.hpp.in
	config.hpp
	)

target_compile_definitions(sge
	PUBLIC
		LUA_USER_H=\"${CMAKE_CURRENT_SOURCE_DIR}/game.h\"
	)

