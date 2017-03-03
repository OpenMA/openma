CMAKE_MINIMUM_REQUIRED(VERSION 3.1.0 FATAL_ERROR)

# OpenMA: Open Movement Analysis Library
PROJECT(OPENMA)

# OPENMA version number.
SET(OPENMA_VERSION_MAJOR 1)
SET(OPENMA_VERSION_MINOR 0)
SET(OPENMA_VERSION_PATCH 0)
SET(OPENMA_VERSION "${OPENMA_VERSION_MAJOR}.${OPENMA_VERSION_MINOR}.${OPENMA_VERSION_PATCH}")
SET(OPENMA_VERSION_STRING ${OPENMA_VERSION})
# The major.minor is enough to distinguish available features of the toolkit. Moreover, 
# this variable is used to create lib/share/include folders where the patch number does 
# not need to be included.
SET(OPENMA_LIBRARY_VERSION_STRING "${OPENMA_VERSION_MAJOR}.${OPENMA_VERSION_MINOR}")

# ------------------------------------------------------------------------
# CMAKE GENERAL CONFIGURATION
# ------------------------------------------------------------------------

SET(CMAKE_INCLUDE_CURRENT_DIR ON)
SET(CMAKE_INCLUDE_CURRENT_DIR_IN_INTERFACE ON)
SET(CMAKE_CXX_STANDARD 11)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)

IF((CMAKE_SYSTEM_NAME STREQUAL "Darwin") AND (CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang"))
  SET(CMAKE_CXX_FLAGS "-stdlib=libc++")
  # CMake 3.1 complains it does not know how to set the compile flag to enable C++11 with (Apple) Clang
  SET(CMAKE_CXX11_STANDARD_COMPILE_OPTION "-std=c++11")
  SET(CMAKE_CXX11_EXTENSION_COMPILE_OPTION "-std=gnu++11")
ELSEIF((CMAKE_SYSTEM_NAME STREQUAL "Windows") AND MSVC)
  # CMake 3.1 complains it does not know how to set the compile flag to enable C++11 with MSVC
  SET(CMAKE_CXX11_STANDARD_COMPILE_OPTION "")
  SET(CMAKE_CXX11_EXTENSION_COMPILE_OPTION "")
  # CXX Flag details
  #  - EHsc: enable exception handling for C++ object
  #  - D_SCL_SECURE_NO_WARNINGS: disable function call with unsafe parameters warning
  SET(CMAKE_CXX_FLAGS "/EHsc -D_SCL_SECURE_NO_WARNINGS")
ELSEIF((CMAKE_SYSTEM_NAME STREQUAL "Windows") AND MINGW)
  # CXX Flag details:
  #  - -ffloat-store: get the same floating point results with different optimization levels
  SET(CMAKE_CXX_FLAGS "-ffloat-store")
ENDIF()

# ------------------------------------------------------------------------
# COMPILATION INSTRUCTIONS
# ------------------------------------------------------------------------

# Using the configuration "Always full RPATH"
# from http://www.cmake.org/Wiki/CMake_RPATH_handling
#             --------------------------
# use, i.e. don't skip the full RPATH for the build tree
SET(CMAKE_SKIP_BUILD_RPATH  FALSE)
# when building, don't use the install RPATH already
# (but later on when installing)
SET(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE) 
# the RPATH to be used when installing
SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib/openma-${OPENMA_LIBRARY_VERSION_STRING}")
# add the automatically determined parts of the RPATH
# which point to directories outside the build tree to the install RPATH
SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

# Extra CMake scripts
SET(OPENMA_CMAKE_MODULE_PATH "${OPENMA_SOURCE_DIR}/cmake")
SET(CMAKE_MODULE_PATH "${OPENMA_CMAKE_MODULE_PATH}")

# Output directories.
SET(LIBRARY_OUTPUT_PATH ${OPENMA_BINARY_DIR}/bin CACHE INTERNAL "Single output directory for building all libraries.")
SET(OPENMA_LIBRARY_PATH "${LIBRARY_OUTPUT_PATH}")
SET(EXECUTABLE_OUTPUT_PATH ${OPENMA_BINARY_DIR}/bin CACHE INTERNAL "Single output directory for building all executables.")
SET(OPENMA_EXECUTABLE_PATH "${EXECUTABLE_OUTPUT_PATH}")

# Load some CMake macros.
INCLUDE(CheckIncludeFile)
INCLUDE(GenerateExportHeader)
INCLUDE(CMakePackageConfigHelpers)
INCLUDE(TestBigEndian)

# By default OPENMA is compiled in Release mode
IF(NOT CMAKE_BUILD_TYPE)
  MESSAGE(STATUS "Setting build type to 'Release' as none was specified.")
  SET(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui
  SET_PROPERTY(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
ENDIF()

# 64-bit OS detection
IF((CMAKE_SIZEOF_VOID_P EQUAL 8) OR CMAKE_CL_64)
  SET(HAVE_64_BIT_COMPILER 1)
ENDIF()

# Detect the OS endianness
TEST_BIG_ENDIAN(HAVE_BIGENDIAN)

# Try to find mmap
CHECK_INCLUDE_FILE(sys/mman.h HAVE_SYS_MMAP)

# Extra compiler flags for the debug mode
INCLUDE(${OPENMA_CMAKE_MODULE_PATH}/ExtraCXXFlagsDebug.cmake)

# Custom target for Coverity integrated with Travis-ci
INCLUDE(${OPENMA_CMAKE_MODULE_PATH}/OpenMACoverityCustomTarget.cmake)
# Custom target for Coveralls integrated with Travis-ci
INCLUDE(${OPENMA_CMAKE_MODULE_PATH}/OpenMACoverallsCustomTarget.cmake)

# Build configuration option.
OPTION(BUILD_SHARED_LIBS "Build OPENMA with shared libraries." OFF)
SET(OPENMA_BUILD_SHARED_LIBS ${BUILD_SHARED_LIBS})
IF(WIN32)
   IF(BUILD_SHARED_LIBS)
     SET(OPENMA_LIBS_BUILD_TYPE "SHARED")
   ELSE()
     SET(OPENMA_LIBS_BUILD_TYPE "STATIC")
   ENDIF()
ENDIF()

# Build language bindings
OPTION(BUILD_MATLAB_BINDINGS "Build bindings to use OpenMA in Matlab." OFF)
OPTION(BUILD_PYTHON_BINDINGS "Build bindings to use OpenMA in Python." OFF)
IF(BUILD_MATLAB_BINDINGS OR BUILD_PYTHON_BINDINGS)
  SET(BUILD_LANGUAGE_BINDINGS 1)
  SET(USE_REFCOUNT_MECHANISM 1)
  INCLUDE(${OPENMA_CMAKE_MODULE_PATH}/OpenMALanguageBindings.cmake)
ELSE()
  SET(BUILD_LANGUAGE_BINDINGS 0)
ENDIF()

# Build unit/regression tests - TDD (Test-driven development) code
OPTION(BUILD_UNIT_TESTS "Build OpenMA unit and regression tests." OFF)
IF(BUILD_UNIT_TESTS)
  # The CxxTest library included with OpenMA is modified.
  INCLUDE(${OPENMA_SOURCE_DIR}/3rdparty/cxxtest/CxxTest.cmake)
  ENABLE_TESTING()
ENDIF()

# Configure files with settings for use by the build.
CONFIGURE_FILE(${OPENMA_CMAKE_MODULE_PATH}/templates/config.h.in
               ${OPENMA_BINARY_DIR}/include/openma/config.h @ONLY IMMEDIATE)

# This setting of SOVERSION assumes that any API change
# will increment either the minor or major version number of OPENMA.
IF(WIN32)
  SET(OPENMA_LIBRARY_PREFIX "openma_")
ELSE()
  SET(OPENMA_LIBRARY_PREFIX "libopenma_")
ENDIF()
SET(OPENMA_LIBRARY_PROPERTIES 
  VERSION   "${OPENMA_VERSION_MAJOR}.${OPENMA_VERSION_MINOR}.${OPENMA_VERSION_PATCH}"
  SOVERSION "${OPENMA_VERSION_MAJOR}.${OPENMA_VERSION_MINOR}"
  PREFIX    "${OPENMA_LIBRARY_PREFIX}"
  DEBUG_POSTFIX _debug)

# ------------------------------------------------------------------------
# PROJECT'S SUBDIRECTORIES
# ------------------------------------------------------------------------

# Extra libraries required (e.g. eigen3, CxxTest, Open3DMotion)
# This subdirectory is added first to compile required external shared libraries
ADD_SUBDIRECTORY(3rdparty)

# Code
ADD_SUBDIRECTORY(modules)

# Documentation
ADD_SUBDIRECTORY(doc)

# ------------------------------------------------------------------------
# INSTALLATION INSTRUCTIONS
# ------------------------------------------------------------------------

# Export the package for use from the build-tree
# (this registers the build-tree with a global CMake-registry)
# EXPORT(PACKAGE OPENMA)

# Create the OPENMAConfig.cmake and OPENMAConfigVersion.cmake files

# Build tree
# ==========

SET(INCLUDE_INSTALL_DIR "${OPENMA_SOURCE_DIR}")

CONFIGURE_PACKAGE_CONFIG_FILE("cmake/templates/OpenMAConfig.cmake.in" "${OPENMA_BINARY_DIR}/OpenMAConfig.cmake"
  INSTALL_DESTINATION ${OPENMA_BINARY_DIR}
  PATH_VARS INCLUDE_INSTALL_DIR)

WRITE_BASIC_PACKAGE_VERSION_FILE(
  "${OPENMA_BINARY_DIR}/OpenMAConfigVersion.cmake"
  VERSION ${OPENMA_VERSION}
  COMPATIBILITY SameMajorVersion
)

EXPORT(EXPORT OpenMATargets
  FILE "${OPENMA_BINARY_DIR}/OpenMATargets.cmake"
  NAMESPACE openma::
)

# Install tree
# ============

SET(INCLUDE_INSTALL_DIR "/include")

CONFIGURE_PACKAGE_CONFIG_FILE("cmake/templates/OpenMAConfig.cmake.in" "${OPENMA_BINARY_DIR}/cmake/OpenMAConfig.cmake"
  INSTALL_DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake"
  PATH_VARS INCLUDE_INSTALL_DIR)

INSTALL(DIRECTORY ${OPENMA_BINARY_DIR}/include
        DESTINATION .)
INSTALL(FILES ${OPENMA_BINARY_DIR}/cmake/OpenMAConfig.cmake
              ${OPENMA_BINARY_DIR}/OpenMAConfigVersion.cmake
        DESTINATION lib/cmake )
INSTALL(EXPORT OpenMATargets NAMESPACE openma:: DESTINATION lib/cmake)
