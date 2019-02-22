
find_path(ASSIMP_INCLUDE_DIR assimp/version.h
  HINTS
    ENV ASSIMP_DIR
  PATH_SUFFIXES include
  PATHS
  ~/Library/Frameworks
  /Library/Frameworks
  /sw # Fink
  /opt/local # DarwinPorts
  /opt/csw # Blastwave
  /opt
)

find_library(ASSIMP_LIBRARY
  NAMES assimp
  HINTS
    ENV ASSIMP_DIR
  PATH_SUFFIXES lib
  PATHS
  ~/Library/Frameworks
  /Library/Frameworks
  /sw
  /opt/local
  /opt/csw
  /opt
)

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(ASSIMP REQUIRED_VARS ASSIMP_LIBRARY ASSIMP_INCLUDE_DIR)

mark_as_advanced(ASSIMP_INCLUDE_DIR ASSIMP_LIBRARY)

