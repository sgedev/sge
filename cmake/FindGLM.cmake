
find_path(GLM_INCLUDE_DIR NAMES glm/glm.hpp)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(glm DEFAULT_MSG GLM_INCLUDE_DIR)

mark_as_advanced(GLM_INCLUDE_DIR)

