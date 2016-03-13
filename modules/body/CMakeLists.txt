SET(OPENMA_BODY_SRCS
  src/descriptor.cpp
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

# ADD_OPENMA_MODULE(body )

ADD_LIBRARY(body ${OPENMA_LIBS_BUILD_TYPE} ${OPENMA_BODY_SRCS})
TARGET_LINK_LIBRARIES(body base maths)
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

IF(BUILD_TESTING)
  ADD_SUBDIRECTORY(test)
ENDIF()