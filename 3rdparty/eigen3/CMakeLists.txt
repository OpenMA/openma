IF(BUILD_UNIT_TESTS)
  ADD_SUBDIRECTORY(test)
ENDIF()

# Eigen
INSTALL(DIRECTORY ${OPENMA_SOURCE_DIR}/3rdparty/eigen3/Eigen
        DESTINATION include/eigen3/Eigen
        COMPONENT dev
        PATTERN ".DS_Store" EXCLUDE
        PATTERN "CMakeLists.txt" EXCLUDE
        PATTERN ".cpp" EXCLUDE)
# Eigen_unsupported (extra modules)
INSTALL(DIRECTORY ${OPENMA_SOURCE_DIR}/3rdparty/eigen3/Eigen_unsupported
        DESTINATION include/eigen3/Eigen_unsupported
        COMPONENT dev
        PATTERN ".DS_Store" EXCLUDE
        PATTERN "CMakeLists.txt" EXCLUDE
        PATTERN ".cpp" EXCLUDE)
# Eigen_openma (plugins and extras)
INSTALL(DIRECTORY ${OPENMA_SOURCE_DIR}/3rdparty/eigen3/Eigen_openma
        DESTINATION include/eigen3/Eigen_openma
        COMPONENT dev
        PATTERN ".DS_Store" EXCLUDE)