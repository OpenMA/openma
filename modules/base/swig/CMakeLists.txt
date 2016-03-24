IF(BUILD_MATLAB_BINDINGS)
  SET(BASE_MATLAB_SRCS "base.i")
  SET(CMAKE_SWIG_OUTDIR "${OPENMA_EXECUTABLE_PATH}/swig/matlab/openma")
  SET_SOURCE_FILES_PROPERTIES("base.i" PROPERTIES CPLUSPLUS ON)
  SET(SWIG_MODULE_baseMEX_EXTRA_DEPS
    "reference_counting.i"
    "base_matlab.i"
    "base/any.i"
    "base/event.i"
    "base/node.i"
    "base/object.i"
    "base/subject.i"
    "base/timesequence.i"
    "base/trial.i")
  SET(SWIG_MODULE_baseMEX_EXTRA_FLAGS
    # "-debug-tmsearch"
    "-module" "ma"
    "-pkgname" "ma"
    "-mexname" "baseMEX"
    "-I${SWIG_DIR}/Lib/matlab"
    "-I${SWIG_DIR}/Lib")
  SWIG_ADD_MODULE(baseMEX matlab "${BASE_MATLAB_SRCS}")
  TARGET_INCLUDE_DIRECTORIES(baseMEX PRIVATE "${MATLAB_INCLUDE_DIR}")
  SWIG_LINK_LIBRARIES(baseMEX base ${MATLAB_LIBRARIES})
  SET_TARGET_PROPERTIES(${SWIG_MODULE_baseMEX_REAL_NAME} PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${OPENMA_EXECUTABLE_PATH}/swig/matlab/openma"
    PREFIX ""
    SUFFIX ".${MATLAB_MEXFILE_EXT}")
ENDIF()
