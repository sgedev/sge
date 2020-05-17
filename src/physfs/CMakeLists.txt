# PhysicsFS; a portable, flexible file i/o abstraction.
#
# Please see the file LICENSE.txt in the source's root directory.

# The CMake project file is meant to get this compiling on all sorts of
#  platforms quickly, and serve as the way Unix platforms and Linux distros
#  package up official builds, but you don't _need_ to use this; we have
#  built PhysicsFS to (hopefully) be able to drop into your project and
#  compile, using preprocessor checks for platform-specific bits instead of
#  testing in here.

set(PHYSFS_VERSION 3.0.2)

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
    sge_compile_definitions(-erroff=E_EMPTY_TRANSLATION_UNIT)
    sge_compile_definitions(-xldscope=hidden)
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

option(PHYSFS_ARCHIVE_ZIP "Enable ZIP support" TRUE)
if(NOT PHYSFS_ARCHIVE_ZIP)
    sge_compile_definitions(-DPHYSFS_SUPPORTS_ZIP=0)
endif()

option(PHYSFS_ARCHIVE_7Z "Enable 7zip support" TRUE)
if(NOT PHYSFS_ARCHIVE_7Z)
    sge_compile_definitions(-DPHYSFS_SUPPORTS_7Z=0)
endif()

option(PHYSFS_ARCHIVE_GRP "Enable Build Engine GRP support" TRUE)
if(NOT PHYSFS_ARCHIVE_GRP)
    sge_compile_definitions(-DPHYSFS_SUPPORTS_GRP=0)
endif()

option(PHYSFS_ARCHIVE_WAD "Enable Doom WAD support" TRUE)
if(NOT PHYSFS_ARCHIVE_WAD)
    sge_compile_definitions(-DPHYSFS_SUPPORTS_WAD=0)
endif()

option(PHYSFS_ARCHIVE_HOG "Enable Descent I/II HOG support" TRUE)
if(NOT PHYSFS_ARCHIVE_HOG)
    sge_compile_definitions(-DPHYSFS_SUPPORTS_HOG=0)
endif()

option(PHYSFS_ARCHIVE_MVL "Enable Descent I/II MVL support" TRUE)
if(NOT PHYSFS_ARCHIVE_MVL)
    sge_compile_definitions(-DPHYSFS_SUPPORTS_MVL=0)
endif()

option(PHYSFS_ARCHIVE_QPAK "Enable Quake I/II QPAK support" TRUE)
if(NOT PHYSFS_ARCHIVE_QPAK)
    sge_compile_definitions(-DPHYSFS_SUPPORTS_QPAK=0)
endif()

option(PHYSFS_ARCHIVE_SLB "Enable I-War / Independence War SLB support" TRUE)
if(NOT PHYSFS_ARCHIVE_SLB)
    sge_compile_definitions(-DPHYSFS_SUPPORTS_SLB=0)
endif()

option(PHYSFS_ARCHIVE_ISO9660 "Enable ISO9660 support" TRUE)
if(NOT PHYSFS_ARCHIVE_ISO9660)
    sge_compile_definitions(-DPHYSFS_SUPPORTS_ISO9660=0)
endif()

option(PHYSFS_ARCHIVE_VDF "Enable Gothic I/II VDF archive support" TRUE)
if(NOT PHYSFS_ARCHIVE_VDF)
    sge_compile_definitions(-DPHYSFS_SUPPORTS_VDF=0)
endif()

sge_sources(${PHYSFS_SRCS})
