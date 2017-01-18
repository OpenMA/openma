SET_SOURCE_FILES_PROPERTIES("processing.i" PROPERTIES CPLUSPLUS ON)
SET(SWIG_MODULE_processing_EXTRA_SRCS)
SET(SWIG_MODULE_processing_EXTRA_FLAGS
  # "-debug-tmsearch"
  "-I${PROJECT_SOURCE_DIR}/modules/base/swig"
  "-I${PROJECT_SOURCE_DIR}/modules/bindings/swig"
  "-I${SWIG_DIR}/Lib")

IF(BUILD_MATLAB_BINDINGS)
  SET(PROCESSING_MATLAB_SRCS
    "processing.i"
    "../../bindings/include/openma/matlab/mexFunction.def")
  SET(CMAKE_SWIG_OUTDIR "${OPENMA_EXECUTABLE_PATH}/swig/matlab/openma")
  SET(SWIG_MODULE_processingMEX_EXTRA_DEPS
    ${SWIG_MODULE_processing_EXTRA_SRCS}
    "processing_matlab.i")
  SET(SWIG_MODULE_processingMEX_EXTRA_FLAGS
    "-module" "ma.processing"
    "-mexname" "processingMEX"
    ${SWIG_MODULE_processing_EXTRA_FLAGS}
    "-I${SWIG_DIR}/Lib/matlab")
  SWIG_ADD_MODULE(processingMEX matlab "${PROCESSING_MATLAB_SRCS}")
  SWIG_LINK_LIBRARIES(${SWIG_MODULE_processingMEX_REAL_NAME} processing bindings ${MATLAB_LIBRARIES})
  TARGET_INCLUDE_DIRECTORIES(${SWIG_MODULE_processingMEX_REAL_NAME} PRIVATE "${MATLAB_INCLUDE_DIR}")
  SET_TARGET_PROPERTIES(${SWIG_MODULE_processingMEX_REAL_NAME} PROPERTIES
    COMPILE_DEFINITIONS "_SWIG_MATLAB"
    LIBRARY_OUTPUT_DIRECTORY "${OPENMA_EXECUTABLE_PATH}/swig/matlab/openma"
    PREFIX ""
    SUFFIX ".${MATLAB_MEXFILE_EXT}")
ENDIF()

IF(BUILD_PYTHON_BINDINGS)
  SET(PROCESSING_PYTHON_SRCS
    "processing.i")
  SET(CMAKE_SWIG_OUTDIR "${OPENMA_EXECUTABLE_PATH}/swig/python/openma/ma")
  SET(SWIG_MODULE_processing_EXTRA_DEPS
    ${SWIG_MODULE_processing_EXTRA_SRCS}
    "processing_python.i")
  SET(SWIG_MODULE_processing_EXTRA_FLAGS
    ${SWIG_MODULE_processing_EXTRA_FLAGS}
    "-I${SWIG_DIR}/Lib/python")
  SWIG_ADD_MODULE(processing python "${PROCESSING_PYTHON_SRCS}")
  SWIG_LINK_LIBRARIES(processing processing bindings ${PYTHON_LIBRARY})
  TARGET_INCLUDE_DIRECTORIES(${SWIG_MODULE_processing_REAL_NAME} PRIVATE "${PYTHON_INCLUDE_DIR}")
  SET_TARGET_PROPERTIES(${SWIG_MODULE_processing_REAL_NAME} PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${OPENMA_EXECUTABLE_PATH}/swig/python/openma/ma")
ENDIF()