SET(PACKAGE_DIRECTORY "${EXECUTABLE_OUTPUT_PATH}/swig/python/openma")
SET(TESTS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
SET(PTD_DIRS ${PACKAGE_DIRECTORY} ${TESTS_DIRECTORY})

ADD_PYTHON_TESTDRIVER(python_body_landmarkstranslator landmarkstranslatorTest INCLUDES ${PTD_DIRS})
ADD_PYTHON_TESTDRIVER(python_body_plugingait_calibration plugingaitTest_calibration INCLUDES ${PTD_DIRS} "${CMAKE_BINARY_DIR}/modules/io/test/python")