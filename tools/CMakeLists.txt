
set(SGE_EDITOR OFF CACHE BOOL "Build sge editor.")
if(SGE_EDITOR)
    add_subdirectory(editor editor)
endif()

set(SGE_LAUNCHER ON CACHE BOOL "Build sge launcher.")
if(SGE_LAUNCHER)
    add_subdirectory(launcher launcher)
endif()

set(SGE_SERVER OFF CACHE BOOL "Build sge server.")
if(SGE_SERVER)
    add_subdirectory(server server)
endif()
