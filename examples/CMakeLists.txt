# Build example applications

set(MKVG_DO_EXAMPLES OFF)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../thirdparty/MonkVG ${CMAKE_CURRENT_BINARY_DIR}/MonkVG)

# suppress annoying C++ 20 deprecation warnings
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_compile_options(-Wno-deprecated-enum-enum-conversion)
endif()

find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)

if (APPLE)
    # M1s have an issue with GLFW includes.  See: https://stackoverflow.com/questions/67373307/macos-m1-fatal-error-glfw-glfw3-h-file-not-found
    set(PLATFORM_LIBS ${PLATFORM_LIBS} "-framework Cocoa" "-framework OpenGL" "-framework IOKit")
endif(APPLE)

## Hello World
add_executable(hello_world hello_world.cpp)
target_link_libraries(hello_world 
    zero_physics
    monkvg
    ${GLU_LIBRARIES} # Required by MonkVG
    glfw   
    OpenGL::GL
    ${PLATFORM_LIBS}
    ${CMAKE_DL_LIBS}
    pthread
    )
target_include_directories(hello_world 
    PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/../thirdparty/MonkVG/include
    ${GLFW_INCLUDE_DIRS}
    zero_physics
    )

## Stress
add_executable(stress stress.cpp)
target_link_libraries(stress 
    zero_physics
    monkvg
    ${GLU_LIBRARIES} # Required by MonkVG
    glfw   
    OpenGL::GL
    ${PLATFORM_LIBS}
    ${CMAKE_DL_LIBS}
    pthread
    )
target_include_directories(stress 
    PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/../thirdparty/MonkVG/include
    ${GLFW_INCLUDE_DIRS}
    zero_physics
    )
