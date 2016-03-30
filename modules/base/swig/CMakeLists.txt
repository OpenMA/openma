IF(BUILD_MATLAB_BINDINGS)
  SET(BASE_MATLAB_SRCS
    "base.i"
    "../../bindings/include/openma/matlab/mexFunction.def")
  SET(CMAKE_SWIG_OUTDIR "${OPENMA_EXECUTABLE_PATH}/swig/matlab/openma")
  SET_SOURCE_FILES_PROPERTIES("base.i" PROPERTIES CPLUSPLUS ON)
  SET(SWIG_MODULE_maMEX_EXTRA_DEPS
    "base_matlab.i"
    "base/any.i"
    "base/event.i"
    "base/node.i"
    "base/object.i"
    "base/subject.i"
    "base/timesequence.i"
    "base/trial.i")
  SET(SWIG_MODULE_maMEX_EXTRA_FLAGS
    # "-debug-tmsearch"
    "-I${PROJECT_SOURCE_DIR}/modules/bindings/swig"
    "-I${SWIG_DIR}/Lib/matlab"
    "-I${SWIG_DIR}/Lib")
  SWIG_ADD_MODULE(maMEX matlab "${BASE_MATLAB_SRCS}")
  TARGET_INCLUDE_DIRECTORIES(maMEX PRIVATE "${MATLAB_INCLUDE_DIR}")
  SWIG_LINK_LIBRARIES(maMEX base bindings ${MATLAB_LIBRARIES})
  SET_TARGET_PROPERTIES(${SWIG_MODULE_maMEX_REAL_NAME} PROPERTIES
    COMPILE_DEFINITIONS "_SWIG_MATLAB"
    LIBRARY_OUTPUT_DIRECTORY "${OPENMA_EXECUTABLE_PATH}/swig/matlab/openma"
    PREFIX ""
    SUFFIX ".${MATLAB_MEXFILE_EXT}")
ENDIF()
