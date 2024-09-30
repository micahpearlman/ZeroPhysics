cmake_minimum_required(VERSION 3.5.0)
project(zero_physics VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)

## configuration
# Set CMP0077 policy to NEW
cmake_policy(SET CMP0077 NEW)
option(ZOPHY_BUILD_EXAMPLES "Build examples" ON)

# prefer vendor opengl
set(OpenGL_GL_PREFERENCE "GLVND")


## Thirdparty
set(GLM_ENABLE_CXX_20 ON)
add_subdirectory(thirdparty/glm)
set(GLM_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/glm)

## ZeroPhysics
set(ZOPHY_SOURCE_FILES
    src/physics_system_2d.cpp
    src/collider_2d.cpp
    src/collision_system_2d.cpp
    src/math.cpp
    src/physics_object_2d.cpp
)
set(ZOPHY_INCLUDE_DIRS
    ./include
)

add_library(zero_physics ${ZOPHY_SOURCE_FILES})
message(STATUS "GLM_INCLUDE_DIRS: ${GLM_INCLUDE_DIRS}")
target_link_libraries(zero_physics glm)
target_include_directories(zero_physics 
    PUBLIC 
        ${ZOPHY_INCLUDE_DIRS} 
        ${GLM_INCLUDE_DIRS}
    PRIVATE 
        ./src
)
        

if(ZOPHY_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()


include(CTest)
enable_testing()
add_subdirectory(tests)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
