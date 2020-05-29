
sge_sources(
	Common.hpp
    Dir.hpp
	File.hpp
	FileSystem.hpp
	Kernel.hpp
	NativeFileSystem.hpp
	Root.hpp
	Task.h
	ZipFileSystem.hpp
	)

sge_compile_definitions(LUA_USER_H=\"${CMAKE_CURRENT_SOURCE_DIR}/Task.h\")
