
target_sources(sge
	PUBLIC
		common.hpp
		entity.hpp
		scene.hpp
		view.hpp
		vm.h
		vm.hpp
	PRIVATE
		config.hpp.in
	)

configure_file(
	config.hpp.in
	config.hpp
	)

target_compile_definitions(sge
	PUBLIC
		LUA_USER_H=\"${CMAKE_CURRENT_SOURCE_DIR}/vm.h\"
	)

