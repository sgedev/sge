
sge_subdirectories(
	Graphics
	Physics
	)

sge_sources(
	Camera.hpp
	Client.hpp
	Common.hpp
	Config.hpp.in
	Game.hpp
	Graphics.hpp
	Lua.h
	Lua.hpp
	Object.hpp
	Physics.hpp
	Scene.hpp
	Server.hpp
	)

configure_file(
	Config.hpp.in
	Config.hpp
	)

sge_compile_definitions(LUA_USER_H=\"${CMAKE_CURRENT_SOURCE_DIR}/Lua.h\")

