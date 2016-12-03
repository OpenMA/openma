# Path for the files used in the unit/regression tests
IF(NOT OPENMA_TESTING_DATA_PATH)
  SET(OPENMA_TESTING_DATA_PATH "${OPENMA_TESTING_DATA_PATH}" CACHE PATH "Path for the data used by the unit tests." FORCE)
  MESSAGE(FATAL_ERROR "You must set the path for the data used by the unit tests")
ENDIF(NOT OPENMA_TESTING_DATA_PATH)

FILE(TO_CMAKE_PATH "${OPENMA_TESTING_DATA_PATH}" OPENMA_TESTING_DATA_PATH)
# Build the directories used to write files in some unit/regression tests
EXECUTE_PROCESS(COMMAND ${CMAKE_COMMAND} -E make_directory "${OPENMA_BINARY_DIR}/test/data/output/c3d")
# Configure the file paths
CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/test_file_path.h.in ${CMAKE_CURRENT_BINARY_DIR}/test_file_path.h)

ADD_CXXTEST_TESTDRIVER(openma_io ioTest.cpp io)
ADD_CXXTEST_TESTDRIVER(openma_io_binarystream binarystreamTest.cpp io)
ADD_CXXTEST_TESTDRIVER(openma_io_buffer bufferTest.cpp io)
ADD_CXXTEST_TESTDRIVER(openma_io_file fileTest.cpp io)
ADD_CXXTEST_TESTDRIVER(openma_io_handlerplugin handlerpluginTest.cpp io)
ADD_CXXTEST_TESTDRIVER(openma_io_handlerplugin_reader_c3d trial/c3dreaderTest.cpp io)
ADD_CXXTEST_TESTDRIVER(openma_io_handlerplugin_writer_c3d trial/c3dwriterTest.cpp io)