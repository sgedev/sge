
# I hate that they define "WIN32" ... we're about to move to Win64...I hope!
if(WIN32 AND NOT WINDOWS)
    set(WINDOWS TRUE)
endif()

if(APPLE)
    set(OTHER_LDFLAGS ${OTHER_LDFLAGS} "-framework IOKit -framework Foundation")
    set(PHYSFS_M_SRCS physfs_platform_apple.m)
endif()

if(CMAKE_COMPILER_IS_GNUCC)
    # Don't use -rpath.
    set(CMAKE_SKIP_RPATH ON CACHE BOOL "Skip RPATH" FORCE)
endif()

if(CMAKE_C_COMPILER_ID STREQUAL "SunPro")
    add_definitions(-erroff=E_EMPTY_TRANSLATION_UNIT)
    add_definitions(-xldscope=hidden)
endif()

if(HAIKU)
    # We add this explicitly, since we don't want CMake to think this
    #  is a C++ project unless we're on Haiku.
    set(PHYSFS_CPP_SRCS physfs_platform_haiku.cpp)
    find_library(BE_LIBRARY be)
    find_library(ROOT_LIBRARY root)
    set(OPTIONAL_LIBRARY_LIBS ${OPTIONAL_LIBRARY_LIBS} ${BE_LIBRARY} ${ROOT_LIBRARY})
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "WindowsPhone" OR CMAKE_SYSTEM_NAME STREQUAL "WindowsStore")
    set(WINRT TRUE)
endif()

if(WINRT)
    set(PHYSFS_CPP_SRCS physfs_platform_winrt.cpp)
endif()

if(UNIX AND NOT WINDOWS AND NOT APPLE)  # (MingW and such might be UNIX _and_ WINDOWS!)
    find_library(PTHREAD_LIBRARY pthread)
    if(PTHREAD_LIBRARY)
        set(OPTIONAL_LIBRARY_LIBS ${OPTIONAL_LIBRARY_LIBS} ${PTHREAD_LIBRARY})
    endif()
endif()

# Almost everything is "compiled" here, but things that don't apply to the
#  build are #ifdef'd out. This is to make it easy to embed PhysicsFS into
#  another project or bring up a new build system: just compile all the source
#  code and #define the things you want.
set(PHYSFS_SRCS
    physfs.c
    physfs_byteorder.c
    physfs_unicode.c
    physfs_platform_posix.c
    physfs_platform_unix.c
    physfs_platform_windows.c
    physfs_platform_os2.c
    physfs_platform_qnx.c
    physfs_archiver_dir.c
    physfs_archiver_unpacked.c
    physfs_archiver_grp.c
    physfs_archiver_hog.c
    physfs_archiver_7z.c
    physfs_archiver_mvl.c
    physfs_archiver_qpak.c
    physfs_archiver_wad.c
    physfs_archiver_zip.c
    physfs_archiver_slb.c
    physfs_archiver_iso9660.c
    physfs_archiver_vdf.c
    ${PHYSFS_CPP_SRCS}
    ${PHYSFS_M_SRCS}
)

# Archivers ...
# These are (mostly) on by default now, so these options are only useful for
#  disabling them.

#sge_compile_definitions(PHYSFS_SUPPORTS_ZIP=0)
sge_compile_definitions(PHYSFS_SUPPORTS_7Z=0)
sge_compile_definitions(PHYSFS_SUPPORTS_GRP=0)
sge_compile_definitions(PHYSFS_SUPPORTS_WAD=0)
sge_compile_definitions(PHYSFS_SUPPORTS_HOG=0)
sge_compile_definitions(PHYSFS_SUPPORTS_MVL=0)
sge_compile_definitions(PHYSFS_SUPPORTS_QPAK=0)
sge_compile_definitions(PHYSFS_SUPPORTS_SLB=0)
sge_compile_definitions(PHYSFS_SUPPORTS_ISO9660=0)
sge_compile_definitions(PHYSFS_SUPPORTS_VDF=0)

sge_sources(${PHYSFS_SRCS})

