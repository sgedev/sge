
find_path(EIGEN3_INCLUDE_DIR NAMES signature_of_eigen3_matrix_library
    PATHS
    ${CMAKE_INSTALL_PREFIX}/include
    ${KDE4_INCLUDE_DIR}
    PATH_SUFFIXES eigen3 eigen
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(eigen3 DEFAULT_MSG EIGEN3_INCLUDE_DIR)

mark_as_advanced(EIGEN3_INCLUDE_DIR)

