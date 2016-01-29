IF(BUILD_TESTING)
  ADD_SUBDIRECTORY(test)
ENDIF()

# Eigen
INSTALL(DIRECTORY ${OPENMA_SOURCE_DIR}/3rdparty/eigen3/Eigen
        INCLUDES DESTINATION include
        COMPONENT dev
        PATTERN ".DS_Store" EXCLUDE
        PATTERN "CMakeLists.txt" EXCLUDE
        PATTERN ".cpp" EXCLUDE)
# Eigen_unsupported (extra modules)
INSTALL(DIRECTORY ${OPENMA_SOURCE_DIR}/3rdparty/eigen3/Eigen_unsupported
        INCLUDES DESTINATION include
        COMPONENT dev
        PATTERN ".DS_Store" EXCLUDE
        PATTERN "CMakeLists.txt" EXCLUDE
        PATTERN ".cpp" EXCLUDE)
# Eigen_openma (plugins and extras)
INSTALL(DIRECTORY ${OPENMA_SOURCE_DIR}/3rdparty/eigen3/Eigen_openma
        INCLUDES DESTINATION include
        COMPONENT dev
        PATTERN ".DS_Store" EXCLUDE)