
sge_subdirectories(
	Graphics
	Physics
	VM
	)

sge_sources(
	Camera.hpp
	Client.hpp
	Common.hpp
	Config.hpp.in
	Game.hpp
	Graphics.hpp
	Object.hpp
	Physics.hpp
	Scene.hpp
	Server.hpp
	VM.hpp
	)

configure_file(
	Config.hpp.in
	Config.hpp
	)
