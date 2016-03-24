IF(BUILD_MATLAB_BINDINGS)
  SET(BASE_MATLAB_SRCS "base.i")
  SET(CMAKE_SWIG_OUTDIR "${OPENMA_EXECUTABLE_PATH}/swig/matlab/openma")
  SET_SOURCE_FILES_PROPERTIES("base.i" PROPERTIES CPLUSPLUS ON)
  SET(SWIG_MODULE_openma_base_mex_EXTRA_DEPS
    "base_matlab.i"
    "base/any.i"
    "base/event.i"
    "base/node.i"
    "base/object.i"
    "base/subject.i"
    "base/timesequence.i"
    "base/trial.i")
  SET(SWIG_MODULE_openma_base_mex_EXTRA_FLAGS
    # "-debug-tmsearch"
    "-module" "ma"
    "-pkgname" "ma"
    "-mexname" "openma_base_mex"
    "-I${SWIG_DIR}/Lib/matlab"
    "-I${SWIG_DIR}/Lib")
  SWIG_ADD_MODULE(openma_base_mex matlab "${BASE_MATLAB_SRCS}")
  TARGET_INCLUDE_DIRECTORIES(openma_base_mex PRIVATE "${MATLAB_INCLUDE_DIR}")
  SWIG_LINK_LIBRARIES(openma_base_mex base ${MATLAB_LIBRARIES})
  SET_TARGET_PROPERTIES(${SWIG_MODULE_openma_base_mex_REAL_NAME} PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${OPENMA_EXECUTABLE_PATH}/swig/matlab/openma"
    PREFIX ""
    SUFFIX ".${MATLAB_MEXFILE_EXT}")
ENDIF()
