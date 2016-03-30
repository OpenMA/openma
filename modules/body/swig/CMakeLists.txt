IF(BUILD_MATLAB_BINDINGS)
  SET(BODY_MATLAB_SRCS
    "body.i"
    "../../bindings/include/openma/matlab/mexFunction.def")
  SET(CMAKE_SWIG_OUTDIR "${OPENMA_EXECUTABLE_PATH}/swig/matlab/openma")
  SET_SOURCE_FILES_PROPERTIES("body.i" PROPERTIES CPLUSPLUS ON)
  SET(SWIG_MODULE_bodyMEX_EXTRA_DEPS
    "body/plugingait.i"
    "body/skeletonhelper.i")
  SET(SWIG_MODULE_bodyMEX_EXTRA_FLAGS
    # "-debug-tmsearch"
    "-module" "ma.body"
    "-mexname" "bodyMEX"
    "-I${PROJECT_SOURCE_DIR}/modules/base/swig"
    "-I${PROJECT_SOURCE_DIR}/modules/bindings/swig"
    "-I${SWIG_DIR}/Lib/matlab"
    "-I${SWIG_DIR}/Lib")
  SWIG_ADD_MODULE(bodyMEX matlab "${BODY_MATLAB_SRCS}")
  TARGET_INCLUDE_DIRECTORIES(bodyMEX PRIVATE "${MATLAB_INCLUDE_DIR}")
  SWIG_LINK_LIBRARIES(bodyMEX body bindings ${MATLAB_LIBRARIES})
  SET_TARGET_PROPERTIES(${SWIG_MODULE_bodyMEX_REAL_NAME} PROPERTIES
    COMPILE_DEFINITIONS "_SWIG_MATLAB"
    LIBRARY_OUTPUT_DIRECTORY "${OPENMA_EXECUTABLE_PATH}/swig/matlab/openma"
    PREFIX ""
    SUFFIX ".${MATLAB_MEXFILE_EXT}")
ENDIF()
