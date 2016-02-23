# Path for the files used in the unit/regression tests
IF(NOT OPENMA_TESTING_DATA_PATH)
  SET(OPENMA_TESTING_DATA_PATH "${OPENMA_TESTING_DATA_PATH}" CACHE PATH "Path for the data used by the unit tests." FORCE)
  MESSAGE(FATAL_ERROR "You must set the path for the data used by the unit tests")
ENDIF(NOT OPENMA_TESTING_DATA_PATH)

FILE(TO_CMAKE_PATH "${OPENMA_TESTING_DATA_PATH}" OPENMA_TESTING_DATA_PATH)
# Build the directories used to write files in some unit/regression tests
EXECUTE_PROCESS(COMMAND ${CMAKE_COMMAND} -E make_directory "${OPENMA_BINARY_DIR}/test/data/output/c3d")
# Configure the file paths
CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/tdd_file_path.h.in ${CMAKE_CURRENT_BINARY_DIR}/tdd_file_path.h)

CXXTEST_ADD_TDD(openma_io ioTest.cpp io)
CXXTEST_ADD_TDD(openma_io_binarystream binarystreamTest.cpp io)
CXXTEST_ADD_TDD(openma_io_buffer bufferTest.cpp io)
CXXTEST_ADD_TDD(openma_io_file fileTest.cpp io)
CXXTEST_ADD_TDD(openma_io_handlerplugin handlerpluginTest.cpp io)
CXXTEST_ADD_TDD(openma_io_handlerplugin_c3d trial/c3dhandlerTest.cpp io)