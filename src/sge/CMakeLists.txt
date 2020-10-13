
target_sources(sge
	PRIVATE
		buffer.cpp
		file.cpp
		fs.cpp
		io.cpp
		list.c
		root_fs.cpp
		zip_fs.cpp
		zip_reader.cpp
	)

add_subdirectory(boot boot)
add_subdirectory(graphics graphics)
add_subdirectory(gui gui)
add_subdirectory(scene scene)
add_subdirectory(sound sound)
add_subdirectory(vm vm)
