
sge_subdirectories(
	gui
	)

sge_sources(
	common.h
	config.h.in
	gui.h
	lua.h
	object.h
	physics.h
	renderer.h
	)

configure_file(
	config.h.in
	config.h
	)
