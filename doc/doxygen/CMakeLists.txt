SET(DOXYGEN_SKIP_DOT TRUE)
FIND_PACKAGE(Doxygen REQUIRED)
FIND_PACKAGE(PythonInterp QUIET)

CONFIGURE_FILE(
  ${CMAKE_CURRENT_SOURCE_DIR}/doxyfile_C++.in
  ${CMAKE_CURRENT_BINARY_DIR}/doxyfile_C++)
  
ADD_CUSTOM_TARGET(doxygen_cpp
  COMMAND ${CMAKE_COMMAND} -E make_directory ${PROJECT_BINARY_DIR}/doc/_doxygen
  COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/doxyfile_C++ > doxy-coverage.detail.txt 2>&1 
  WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
  COMMENT "Generate C++ Doxygen XML output..."
  )

IF(PYTHONINTERP_FOUND)
  ADD_CUSTOM_COMMAND(TARGET doxygen_cpp POST_BUILD
    COMMAND ${PYTHON_EXECUTABLE} ${PROJECT_SOURCE_DIR}/3rdparty/doxygen/doxy-coverage.py --badge --noerror ${PROJECT_BINARY_DIR}/doc/_doxygen/xml > doxy-coverage.resume.txt
    WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
    )
ENDIF()