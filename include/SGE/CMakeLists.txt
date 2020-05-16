
sge_subdirectories(
	Graphics
	PHysics
	)

sge_sources(
	Camera.hpp
	Common.hpp
	Config.hpp.in
	Editor.hpp
	Graphics.hpp
	Handle.hpp
	Object.hpp
	Physics.hpp
	Player.hpp
	Scene.hpp
	Server.hpp
	)

configure_file(
	Config.hpp.in
	Config.hpp
	)
