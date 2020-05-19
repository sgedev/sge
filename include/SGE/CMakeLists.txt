
sge_subdirectories(
	Graphics
	Physics
	)

sge_sources(
	Camera.hpp
	Common.hpp
	Config.hpp.in
	Editor.hpp
	Game.hpp
	Graphics.hpp
	List.hpp
	Object.hpp
	Physics.hpp
	Player.hpp
	Scene.hpp
	Server.hpp
	VM.hpp
	VMTask.h
	)

configure_file(
	Config.hpp.in
	Config.hpp
	)

sge_compile_definitions(LUA_USER_H=\"${CMAKE_CURRENT_SOURCE_DIR}/VMTask.h\")
