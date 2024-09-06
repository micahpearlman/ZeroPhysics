cmake_minimum_required(VERSION 3.5.0)
project(zero_physics VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)

option(ZOPHY_BUILD_EXAMPLES "Build examples" ON)

add_library(zero_physics zero_physics.cpp)

if(ZOPHY_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

# include(CTest)
# enable_testing()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
