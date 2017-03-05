ADD_CXX_CXXTEST_DRIVER(openma_math_array arrayTest.cpp math)
ADD_CXX_CXXTEST_DRIVER(openma_math_pose poseTest.cpp math)
ADD_CXX_CXXTEST_DRIVER(openma_math_map mapTest.cpp math)
ADD_CXX_CXXTEST_DRIVER(openma_math_blockop blockopTest.cpp math)
ADD_CXX_CXXTEST_DRIVER(openma_math_mix mixTest.cpp math)

# To have access to the symbol M_PI
SET_TARGET_PROPERTIES(test_openma_math_pose PROPERTIES COMPILE_DEFINITIONS "_USE_MATH_DEFINES")