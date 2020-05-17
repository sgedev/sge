
sge_subdirectories(
	Graphics
	PHysics
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
	)

configure_file(
	Config.hpp.in
	Config.hpp
	)
