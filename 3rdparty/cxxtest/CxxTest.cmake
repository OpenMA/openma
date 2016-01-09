# Simple CMake script which include paths of CxxTest and additional content.
# The variable for the source files is not provided as they are all included in 
# one file 'cxxtest/Root.cpp'. This file is included at the end of the main.cpp
# file.

# NOTE: The CxxTest library included was modified.
#       This version uses C/C++ Macros instead of Python/Perl test generator.

GET_FILENAME_COMPONENT(CXXTEST_ROOT ${CMAKE_CURRENT_LIST_FILE} PATH)

SET(CXXTEST_INCLUDES
  "${CXXTEST_ROOT}"
  "${CXXTEST_ROOT}/additional/generator/macro"
  "${CXXTEST_ROOT}/additional/tdd"
)
                      
SET(CXXTEST_TDD_MAIN_SRC "${CXXTEST_ROOT}/additional/tdd/main.cpp")

FUNCTION(CXXTEST_ADD_TDD)
  # Parse the arguments
  SET(options )
  SET(oneValueArgs NAME)
  SET(multiValueArgs SOURCES INCLUDES LIBRARIES)
  CMAKE_PARSE_ARGUMENTS(_TDD "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
  # Extra unparsed arguments?
  LIST(LENGTH _TDD_UNPARSED_ARGUMENTS _TDD_NUM_UNPARSED_ARGUMENTS)
  IF (${_TDD_NUM_UNPARSED_ARGUMENTS} GREATER 0)
    IF (NOT _TDD_NAME)
      LIST(GET _TDD_UNPARSED_ARGUMENTS 0 _TDD_NAME)
    ENDIF()
  ENDIF()
  IF (${_TDD_NUM_UNPARSED_ARGUMENTS} GREATER 1)
    IF (NOT _TDD_SOURCES)
      LIST(GET _TDD_UNPARSED_ARGUMENTS 1 _TDD_SOURCES)
    ENDIF()
  ENDIF()
  IF (${_TDD_NUM_UNPARSED_ARGUMENTS} GREATER 2)
    IF (NOT _TDD_LIBRARIES)
      LIST(GET _TDD_UNPARSED_ARGUMENTS 2 _TDD_LIBRARIES)
    ENDIF()
  ENDIF ()
  # Create the test and register it
  SET(_TDD_SRCS ${CXXTEST_TDD_MAIN_SRC} ${_TDD_SOURCES})
  ADD_EXECUTABLE(tdd_${_TDD_NAME} ${_TDD_SRCS})
  SET_TARGET_PROPERTIES(tdd_${_TDD_NAME} PROPERTIES TARGET_MESSAGES OFF)
  ADD_TEST(NAME ${_TDD_NAME} COMMAND tdd_${_TDD_NAME})
  TARGET_INCLUDE_DIRECTORIES(tdd_${_TDD_NAME} PRIVATE ${CXXTEST_INCLUDES})
  IF(_TDD_INCLUDES)
    TARGET_INCLUDE_DIRECTORIES(tdd_${_TDD_NAME} PRIVATE ${_TDD_INCLUDES})
  ENDIF()
  IF(_TDD_LIBRARIES)
    TARGET_LINK_LIBRARIES(tdd_${_TDD_NAME} ${_TDD_LIBRARIES})
  ENDIF()
ENDFUNCTION()