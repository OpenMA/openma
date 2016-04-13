# - Try to find a version of Matlab and headers/library required by the 
#   used compiler. It determines the right MEX-File extension and add 
#   a macro to help the build of MEX-functions.
#
# This module detects a Matlab's version between Matlab 7.0 (R14)
# and Matlab 9.0 (r2016a).
#
# This module defines: 
#  MATLAB_ROOT:               Matlab installation path
#  MATLAB_EXECUTABLE:         Path to the Matlab executable
#  MATLAB_INCLUDE_DIR:        Include path for mex.h, engine.h
#  MATLAB_MEX_LIBRARY:        Path to libmex.lib
#  MATLAB_MX_LIBRARY:         Path to libmx.lib
#  MATLAB_ENG_LIBRARY:        Path to libeng.lib
#  MATLAB_LIBRARIES:          Required libraries: libmex, libmx, libeng
#  MATLAB_MEXFILE_EXT:        MEX extension required for the current platform
#
# Copyright (C) 2016, Moveck Solution Inc., all rights reserved.
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.

IF(MATLAB_ROOT AND MATLAB_EXECUTABLE AND MATLAB_INCLUDE_DIR AND MATLAB_LIBRARIES)
   # in cache already
   SET(Matlab_FIND_QUIETLY TRUE)
ENDIF()

IF(WIN32)
  # Default paths
  SET(MATLAB_PATHS 
    "[HKEY_LOCAL_MACHINE\\SOFTWARE\\MathWorks\\MATLAB\\9.0;MATLABROOT]"
    "[HKEY_LOCAL_MACHINE\\SOFTWARE\\MathWorks\\MATLAB\\8.5;MATLABROOT]"
    "[HKEY_LOCAL_MACHINE\\SOFTWARE\\MathWorks\\MATLAB\\8.4;MATLABROOT]"
    "[HKEY_LOCAL_MACHINE\\SOFTWARE\\MathWorks\\MATLAB\\8.3;MATLABROOT]"
    "[HKEY_LOCAL_MACHINE\\SOFTWARE\\MathWorks\\MATLAB\\8.2;MATLABROOT]"
    "[HKEY_LOCAL_MACHINE\\SOFTWARE\\MathWorks\\MATLAB\\8.1;MATLABROOT]"
    "[HKEY_LOCAL_MACHINE\\SOFTWARE\\MathWorks\\MATLAB\\8.0;MATLABROOT]"
    "[HKEY_LOCAL_MACHINE\\SOFTWARE\\MathWorks\\MATLAB\\7.15;MATLABROOT]"
    "[HKEY_LOCAL_MACHINE\\SOFTWARE\\MathWorks\\MATLAB\\7.14;MATLABROOT]"
    "[HKEY_LOCAL_MACHINE\\SOFTWARE\\MathWorks\\MATLAB\\7.13;MATLABROOT]"
    "[HKEY_LOCAL_MACHINE\\SOFTWARE\\MathWorks\\MATLAB\\7.12;MATLABROOT]"
    "[HKEY_LOCAL_MACHINE\\SOFTWARE\\MathWorks\\MATLAB\\7.11;MATLABROOT]"
    "[HKEY_LOCAL_MACHINE\\SOFTWARE\\MathWorks\\MATLAB\\7.10;MATLABROOT]"
    "[HKEY_LOCAL_MACHINE\\SOFTWARE\\MathWorks\\MATLAB\\7.9;MATLABROOT]"
    "[HKEY_LOCAL_MACHINE\\SOFTWARE\\MathWorks\\MATLAB\\7.8;MATLABROOT]"
    "[HKEY_LOCAL_MACHINE\\SOFTWARE\\MathWorks\\MATLAB\\7.7;MATLABROOT]"
    "[HKEY_LOCAL_MACHINE\\SOFTWARE\\MathWorks\\MATLAB\\7.6;MATLABROOT]"
    "[HKEY_LOCAL_MACHINE\\SOFTWARE\\MathWorks\\MATLAB\\7.5;MATLABROOT]"
    "[HKEY_LOCAL_MACHINE\\SOFTWARE\\MathWorks\\MATLAB\\7.4;MATLABROOT]"
    "[HKEY_LOCAL_MACHINE\\SOFTWARE\\MathWorks\\MATLAB\\7.3;MATLABROOT]"
    "[HKEY_LOCAL_MACHINE\\SOFTWARE\\MathWorks\\MATLAB\\7.2;MATLABROOT]"
    "[HKEY_LOCAL_MACHINE\\SOFTWARE\\MathWorks\\MATLAB\\7.1;MATLABROOT]"
  )
  FIND_PATH(MATLAB_ROOT "license.txt" ${MATLAB_PATHS} NO_DEFAULT_PATH)
  # Paths for Matlab 32-bit under Windows 64-bit
  IF(NOT MATLAB_ROOT)
    SET(MATLAB_PATHS 
      "[HKEY_LOCAL_MACHINE\\SOFTWARE\\Wow6432Node\\MathWorks\\MATLAB\\9.0;MATLABROOT]"
      "[HKEY_LOCAL_MACHINE\\SOFTWARE\\Wow6432Node\\MathWorks\\MATLAB\\8.5;MATLABROOT]"
      "[HKEY_LOCAL_MACHINE\\SOFTWARE\\Wow6432Node\\MathWorks\\MATLAB\\8.4;MATLABROOT]"
      "[HKEY_LOCAL_MACHINE\\SOFTWARE\\Wow6432Node\\MathWorks\\MATLAB\\8.3;MATLABROOT]"
      "[HKEY_LOCAL_MACHINE\\SOFTWARE\\Wow6432Node\\MathWorks\\MATLAB\\8.2;MATLABROOT]"
      "[HKEY_LOCAL_MACHINE\\SOFTWARE\\Wow6432Node\\MathWorks\\MATLAB\\8.1;MATLABROOT]"
      "[HKEY_LOCAL_MACHINE\\SOFTWARE\\Wow6432Node\\MathWorks\\MATLAB\\8.0;MATLABROOT]"
      "[HKEY_LOCAL_MACHINE\\SOFTWARE\\Wow6432Node\\MathWorks\\MATLAB\\7.15;MATLABROOT]"
      "[HKEY_LOCAL_MACHINE\\SOFTWARE\\Wow6432Node\\MathWorks\\MATLAB\\7.14;MATLABROOT]"
      "[HKEY_LOCAL_MACHINE\\SOFTWARE\\Wow6432Node\\MathWorks\\MATLAB\\7.13;MATLABROOT]"
      "[HKEY_LOCAL_MACHINE\\SOFTWARE\\Wow6432Node\\MathWorks\\MATLAB\\7.12;MATLABROOT]"
      "[HKEY_LOCAL_MACHINE\\SOFTWARE\\Wow6432Node\\MathWorks\\MATLAB\\7.11;MATLABROOT]"
      "[HKEY_LOCAL_MACHINE\\SOFTWARE\\Wow6432Node\\MathWorks\\MATLAB\\7.10;MATLABROOT]"
      "[HKEY_LOCAL_MACHINE\\SOFTWARE\\Wow6432Node\\MathWorks\\MATLAB\\7.9;MATLABROOT]"
      "[HKEY_LOCAL_MACHINE\\SOFTWARE\\Wow6432Node\\MathWorks\\MATLAB\\7.8;MATLABROOT]"
      "[HKEY_LOCAL_MACHINE\\SOFTWARE\\Wow6432Node\\MathWorks\\MATLAB\\7.7;MATLABROOT]"
      "[HKEY_LOCAL_MACHINE\\SOFTWARE\\Wow6432Node\\MathWorks\\MATLAB\\7.6;MATLABROOT]"
      "[HKEY_LOCAL_MACHINE\\SOFTWARE\\Wow6432Node\\MathWorks\\MATLAB\\7.5;MATLABROOT]"
      "[HKEY_LOCAL_MACHINE\\SOFTWARE\\Wow6432Node\\MathWorks\\MATLAB\\7.4;MATLABROOT]"
      "[HKEY_LOCAL_MACHINE\\SOFTWARE\\Wow6432Node\\MathWorks\\MATLAB\\7.3;MATLABROOT]"
      "[HKEY_LOCAL_MACHINE\\SOFTWARE\\Wow6432Node\\MathWorks\\MATLAB\\7.2;MATLABROOT]"
      "[HKEY_LOCAL_MACHINE\\SOFTWARE\\Wow6432Node\\MathWorks\\MATLAB\\7.1;MATLABROOT]"
    )
    FIND_PATH(MATLAB_ROOT "license.txt" ${MATLAB_PATHS} NO_DEFAULT_PATH)
  ENDIF()
  IF(NOT MATLAB_ROOT)
    SET(MATLAB_PATHS 
      "[HKEY_LOCAL_MACHINE\\SOFTWARE\\Wow6432Node\\MathWorks\\MATLAB\\7.0;MATLABROOT]"
      "[HKEY_LOCAL_MACHINE\\SOFTWARE\\MathWorks\\MATLAB\\7.0;MATLABROOT]"
      "[HKEY_LOCAL_MACHINE\\SOFTWARE\\MathWorks\\MATLAB\\6.5;MATLABROOT]"
    )
    FIND_PATH(MATLAB_ROOT "license.txt" ${MATLAB_PATHS} NO_DEFAULT_PATH)
    IF(MATLAB_ROOT)
      SET(MATLAB_OLD_WIN_MEXFILE_EXT 1 CACHE STRING "Old MEX extension for Windows")
    ENDIF()
  ENDIF()

  SET(MATLAB_LIBRARIES_PATHS
      "${MATLAB_ROOT}/extern/lib/win64/microsoft"
      "${MATLAB_ROOT}/extern/lib/win32/microsoft"
      "${MATLAB_ROOT}/extern/lib/win32/microsoft/msvc70"
      "${MATLAB_ROOT}/extern/lib/win32/microsoft/msvc71")
  SET(MATLAB_INCLUDE_PATHS "${MATLAB_ROOT}/extern/include")

  # MEX files extension
  IF(CMAKE_CXX_COMPILER MATCHES "^.*cl.exe$" OR CMAKE_CXX_COMPILER MATCHES "^.*cl$")
    IF(MATLAB_OLD_WIN_MEXFILE_EXT)
      SET(MATLAB_MEXFILE_EXT dll)
    ELSE()
      IF(CMAKE_CL_64)
        SET(MATLAB_MEXFILE_EXT mexw64)
      ELSE()
        SET(MATLAB_MEXFILE_EXT mexw32)
      ENDIF()
    ENDIF()
  ELSE()
    MESSAGE(FATAL_ERROR "Matlab Mex files are only supported by MS Visual Studio")
  ENDIF()
  #MESSAGE(STATUS "MATLAB_MEXFILE_EXT: ${MATLAB_MEXFILE_EXT}")

  # LIBMEX, LIBMX, LIBENG names
  SET(LIBMEX "libmex")
  SET(LIBMX "libmx")
  SET(LIBENG "libeng")
ELSE()
  # MEX files extension
  IF(APPLE)
    FILE(GLOB MATLAB_PATHS "/Applications/MATLAB_*")
    FIND_PATH(MATLAB_ROOT NAMES "license.txt" "license_agreement.txt" PATHS ${MATLAB_PATHS} NO_DEFAULT_PATH)
    MESSAGE(STATUS "MATLAB_ROOT: ${MATLAB_ROOT}")
    IF(CMAKE_SIZEOF_VOID_P EQUAL 4)
      IF(CMAKE_OSX_ARCHITECTURES MATCHES ppc)
        SET(MATLAB_LIBRARIES_PATHS "${MATLAB_ROOT}/bin/mac")
          SET(MATLAB_MEXFILE_EXT mexmac)
        ELSE()
        SET(MATLAB_LIBRARIES_PATHS "${MATLAB_ROOT}/bin/maci")
          SET(MATLAB_MEXFILE_EXT mexmaci)
        ENDIF()
    ELSE()
      # To force the compilation in 32-bit
      IF(CMAKE_OSX_ARCHITECTURES MATCHES i386)
        MESSAGE("The compilation of the Matlab MEX functions is forced to 32-bit mode as the architecture is set to i386.")
        SET(FORCE_COMPILE_32BIT_MODE 1)
      ELSE()
        IF(EXISTS "${MATLAB_ROOT}/bin/maci64/")
          SET(MATLAB_LIBRARIES_PATHS "${MATLAB_ROOT}/bin/maci64")
          SET(MATLAB_MEXFILE_EXT mexmaci64)
          SET(FORCE_COMPILE_32BIT_MODE 0)
        ELSEIF(EXISTS "${MATLAB_ROOT}/bin/maci/")
          MESSAGE("You use a Matlab 32-bit version on a 64-bit OS. The code will be compiled in 32-bit to be useable with your Matlab version.")
          SET(FORCE_COMPILE_32BIT_MODE 1)
        ENDIF()
      ENDIF()
      IF(FORCE_COMPILE_32BIT_MODE)
        SET(MATLAB_LIBRARIES_PATHS "${MATLAB_ROOT}/bin/maci")
        SET(MATLAB_MEXFILE_EXT mexmaci)
        SET(CMAKE_CXX_FLAGS "-O3 -DNDEBUG -m32" CACHE STRING "Flags used by the compiler during all build types." FORCE)
        SET(CMAKE_C_FLAGS "-O3 -DNDEBUG -m32" CACHE STRING "Flags used by the compiler during all build types." FORCE)
      ENDIF()
    ENDIF()
  
    SET(LIBMEX "libmex.dylib")
    SET(LIBMX "libmx.dylib")
    SET(LIBENG "libeng.dylib")
  ELSE()
    FILE(GLOB MATLAB_LOCAL_PATHS "/usr/local/[Mm][Aa][Tt][Ll][Aa][Bb]*")
    FILE(GLOB MATLAB_LOCAL_PATHS_BIS "/usr/local/[Mm][Aa][Tt][Ll][Aa][Bb]/R*")
    FILE(GLOB MATLAB_OPT_PATHS "/opt/[Mm][Aa][Tt][Ll][Aa][Bb]*")
    FILE(GLOB MATLAB_USR_PATHS "/usr/[Mm][Aa][Tt][Ll][Aa][Bb]*")
    SET(MATLAB_PATHS ${MATLAB_LOCAL_PATHS} ${MATLAB_LOCAL_PATHS_BIS} ${MATLAB_OPT_PATHS} ${MATLAB_USR_PATHS})
  
    SET(LIBMEX "libmex.so")
    SET(LIBMX "libmx.so")
    SET(LIBENG "libeng.so")
  
    FIND_PATH(MATLAB_ROOT "license.txt" ${MATLAB_PATHS})
    IF(NOT MATLAB_ROOT)
      MESSAGE("Linux users have to set the Matlab installation path into the MATLAB_ROOT variable.")
    ENDIF()
  
    IF(CMAKE_SIZEOF_VOID_P EQUAL 4)
      # Regular x86
      SET(MATLAB_LIBRARIES_PATHS "${MATLAB_ROOT}/bin/glnx86")
      SET(MATLAB_MEXFILE_EXT mexglx)
    ELSE()
      # AMD64:
      SET(MATLAB_LIBRARIES_PATHS "${MATLAB_ROOT}/bin/glnxa64")
      SET(MATLAB_MEXFILE_EXT mexa64)
    ENDIF()
  ENDIF()
  
  # Common for UNIX
  SET(MATLAB_INCLUDE_PATHS "${MATLAB_ROOT}/extern/include")
ENDIF()

FIND_PROGRAM(MATLAB_EXECUTABLE
    "matlab"
    PATHS ${MATLAB_ROOT}
    PATH_SUFFIXES "bin" NO_DEFAULT_PATH
    )
FIND_LIBRARY(MATLAB_MEX_LIBRARY
    ${LIBMEX}
    ${MATLAB_LIBRARIES_PATHS} NO_DEFAULT_PATH
    )
FIND_LIBRARY(MATLAB_MX_LIBRARY
    ${LIBMX}
    ${MATLAB_LIBRARIES_PATHS} NO_DEFAULT_PATH
    )
FIND_LIBRARY(MATLAB_ENG_LIBRARY
    ${LIBENG}
    ${MATLAB_LIBRARIES_PATHS} NO_DEFAULT_PATH
    )
FIND_PATH(MATLAB_INCLUDE_DIR
    "mex.h"
    ${MATLAB_INCLUDE_PATHS} NO_DEFAULT_PATH
    )

# This is common to UNIX and Win32:
SET(MATLAB_LIBRARIES
  ${MATLAB_MEX_LIBRARY}
  ${MATLAB_MX_LIBRARY}
  ${MATLAB_ENG_LIBRARY}  
)

# Macros for building MEX-files
MACRO(MATLAB_EXTRACT_SOURCES_LIBRARIES sources thirdlibraries)
  SET(${sources})
  SET(${thirdlibraries})
  FOREACH(_arg ${ARGN})
    GET_FILENAME_COMPONENT(_ext ${_arg} EXT)
    IF("${_ext}" STREQUAL "")
      LIST(APPEND ${thirdlibraries} "${_arg}")
    ELSE()
      LIST(APPEND ${sources} "${_arg}")
    ENDIF()
  ENDFOREACH(_arg)
ENDMACRO(MATLAB_EXTRACT_SOURCES_LIBRARIES)

# MATLAB_MEX_CREATE(functionname inputfiles thridlibraries)
MACRO(MATLAB_MEX_CREATE functionname)
  MATLAB_EXTRACT_SOURCES_LIBRARIES(sources thirdlibraries ${ARGN})
  ADD_LIBRARY(${functionname} SHARED ${sources} mexFunction.def)
  TARGET_LINK_LIBRARIES(${functionname} ${MATLAB_LIBRARIES} ${thirdlibraries})
  SET_TARGET_PROPERTIES(${functionname} PROPERTIES
    PREFIX ""
    SUFFIX  ".${MATLAB_MEXFILE_EXT}"
  )
ENDMACRO(MATLAB_MEX_CREATE)

IF(MATLAB_ROOT)
  INCLUDE(FindPackageHandleStandardArgs)
  FIND_PACKAGE_HANDLE_STANDARD_ARGS(Matlab DEFAULT_MSG MATLAB_ROOT MATLAB_EXECUTABLE MATLAB_INCLUDE_DIR MATLAB_MEX_LIBRARY MATLAB_MX_LIBRARY MATLAB_ENG_LIBRARY)
ENDIF()

MARK_AS_ADVANCED(
  MATLAB_EXECUTABLE
  MATLAB_MEX_LIBRARY
  MATLAB_MX_LIBRARY
  MATLAB_ENG_LIBRARY
  MATLAB_LIBRARIES
  MATLAB_INCLUDE_DIR
  MATLAB_MEXFILE_EXT
  MATLAB_OLD_WIN_MEXFILE_EXT
)