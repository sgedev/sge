
set(SGE_UV_SOURCES
    errno.h
    tree.h
    version.h
    )

if(WIN32)
    list(APPEND SGE_UV_SOURCES win.h)
elseif(UNIX AND NOT APPLE)
    list(APPEND SGE_UV_SOURCES linux.h)
else()
    message(FATAL_ERROR "Unknown platform.")
endif()

target_sources(sge PRIVATE ${SGE_UV_SOURCES})
