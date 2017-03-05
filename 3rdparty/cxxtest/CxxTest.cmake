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
  "${CXXTEST_ROOT}/additional/driver"
)

SET(CXXTEST_TEST_MAIN_SRC "${CXXTEST_ROOT}/additional/driver/main.cpp")

FUNCTION(ADD_CXX_CXXTEST_DRIVER)
  # Parse the arguments
  SET(options )
  SET(oneValueArgs NAME)
  SET(multiValueArgs SOURCES INCLUDES LIBRARIES)
  CMAKE_PARSE_ARGUMENTS(_TEST "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
  # Extra unparsed arguments?
  LIST(LENGTH _TEST_UNPARSED_ARGUMENTS _TEST_NUM_UNPARSED_ARGUMENTS)
  IF (${_TEST_NUM_UNPARSED_ARGUMENTS} GREATER 0)
    IF (NOT _TEST_NAME)
      LIST(GET _TEST_UNPARSED_ARGUMENTS 0 _TEST_NAME)
    ENDIF()
  ENDIF()
  IF (${_TEST_NUM_UNPARSED_ARGUMENTS} GREATER 1)
    IF (NOT _TEST_SOURCES)
      LIST(GET _TEST_UNPARSED_ARGUMENTS 1 _TEST_SOURCES)
    ENDIF()
  ENDIF()
  IF (${_TEST_NUM_UNPARSED_ARGUMENTS} GREATER 2)
    IF (NOT _TEST_LIBRARIES)
      LIST(GET _TEST_UNPARSED_ARGUMENTS 2 _TEST_LIBRARIES)
    ENDIF()
  ENDIF ()
  # Create the test and register it
  SET(_TEST_SRCS ${CXXTEST_TEST_MAIN_SRC} ${_TEST_SOURCES})
  ADD_EXECUTABLE(test_${_TEST_NAME} ${_TEST_SRCS})
  SET_TARGET_PROPERTIES(test_${_TEST_NAME} PROPERTIES TARGET_MESSAGES OFF)
  ADD_TEST(NAME ${_TEST_NAME} COMMAND test_${_TEST_NAME})
  TARGET_INCLUDE_DIRECTORIES(test_${_TEST_NAME} PRIVATE ${CXXTEST_INCLUDES})
  IF(_TEST_INCLUDES)
    TARGET_INCLUDE_DIRECTORIES(test_${_TEST_NAME} PRIVATE ${_TEST_INCLUDES})
  ENDIF()
  IF(_TEST_LIBRARIES)
    TARGET_LINK_LIBRARIES(test_${_TEST_NAME} ${_TEST_LIBRARIES})
  ENDIF()
ENDFUNCTION()