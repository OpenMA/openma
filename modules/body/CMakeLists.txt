SET(OPENMA_BODY_SRCS
  src/anchor.cpp
  src/chain.cpp
  src/dempstertable.cpp
  src/descriptor.cpp
  src/dumasmcconvilleyoungtable.cpp
  src/dynamicdescriptor.cpp
  src/eulerdescriptor.cpp
  src/externalwrenchassigner.cpp
  src/simplegaitforceplatetofeetassigner.cpp
  src/inertialparameters.cpp
  src/inertialparametersestimator.cpp
  src/internationalsocietybiomechanics.cpp
  src/inversedynamicsmatrix.cpp
  src/inversedynamicsprocessor.cpp
  src/joint.cpp
  src/landmarkstranslator.cpp
  src/model.cpp
  src/plugingait.cpp
  src/point.cpp
  src/referenceframe.cpp
  src/segment.cpp
  src/skeletonhelper.cpp
  src/utils.cpp
)

ADD_LIBRARY(body ${OPENMA_LIBS_BUILD_TYPE} ${OPENMA_BODY_SRCS})
TARGET_LINK_LIBRARIES(body base instrument math)
TARGET_INCLUDE_DIRECTORIES(body PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
  $<INSTALL_INTERFACE:include>
)
GENERATE_EXPORT_HEADER(body
                       BASE_NAME OPENMA_BODY
                       EXPORT_FILE_NAME include/openma/body_export.h)

SET_TARGET_PROPERTIES(body PROPERTIES ${OPENMA_LIBRARY_PROPERTIES})
IF (NOT ${OPENMA_BUILD_SHARED_LIBS})
  SET_TARGET_PROPERTIES(body PROPERTIES COMPILE_FLAGS -DOPENMA_BODY_STATIC_DEFINE)
ENDIF()
SET_TARGET_PROPERTIES(body PROPERTIES COMPILE_DEFINITIONS "_USE_MATH_DEFINES")

IF(BUILD_LANGUAGE_BINDINGS)
  ADD_SUBDIRECTORY(swig)
ENDIF()

IF(BUILD_UNIT_TESTS)
  ADD_SUBDIRECTORY(test)
ENDIF()

INSTALL(TARGETS body EXPORT OpenMATargets
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  RUNTIME DESTINATION lib  
  INCLUDES DESTINATION lib
)