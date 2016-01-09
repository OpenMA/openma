# Build API documentation option
OPTION(BUILD_DOCUMENTATION "Build project documentation" OFF)
IF(BUILD_DOCUMENTATION)
  ADD_SUBDIRECTORY(doxygen)
ENDIF()