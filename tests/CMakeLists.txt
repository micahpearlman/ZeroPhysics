# ZeroPhysics Unit Tests

find_package(GTest REQUIRED)
set(GLM_ENABLE_CXX_20 ON)


add_executable(zo_unit_tests test_zoPhysicsMemory.cpp)

# Link the test executable with your project and the GTest library
target_link_libraries(zo_unit_tests PRIVATE GTest::GTest GTest::Main zero_physics)

# Register the test with CTest
add_test(NAME zo_unit_tests COMMAND zo_unit_tests)