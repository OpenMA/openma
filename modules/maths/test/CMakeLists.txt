ADD_CXXTEST_TESTDRIVER(openma_maths_array arrayTest.cpp maths)
ADD_CXXTEST_TESTDRIVER(openma_maths_pose poseTest.cpp maths)
ADD_CXXTEST_TESTDRIVER(openma_maths_map mapTest.cpp maths)
ADD_CXXTEST_TESTDRIVER(openma_maths_blockop blockopTest.cpp maths)
ADD_CXXTEST_TESTDRIVER(openma_maths_mix mixTest.cpp maths)

# To have access to the symbol M_PI
SET_TARGET_PROPERTIES(test_openma_maths_pose PROPERTIES COMPILE_DEFINITIONS "_USE_MATH_DEFINES")