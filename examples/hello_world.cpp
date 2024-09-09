/**
 * @file monkvg_hello_world.cpp
 * @author Micah Pearlman (micahpearlman@gmail.com)
 * @brief ZeroPhysics test application using MonkVG
 * @version 0.1
 * @date 2024-09-06
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <zoPhysicsSystem2d.hpp>
// MonkVG OpenVG interface
#include <MonkVG/openvg.h>
#include <MonkVG/vgext.h>

// OpenGL window creation libraries
#if defined(__APPLE__)
#define GL_SILENCE_DEPRECATION
#define GLFW_INCLUDE_GLCOREARB
#include <GLFW/glfw3.h>
#else
#define GLFW_INCLUDE_ES32
#include <GLFW/glfw3.h>
#endif

// System
#include <iostream>

#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768

int main(int argc, char **argv) {
    std::cout << "Hello, MonkVG!\n";
    // Initialise GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return -1;
    }

    // create OpeGL window
#if defined(__APPLE__)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#else
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,
                   GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE,
                   GLFW_OPENGL_ANY_PROFILE); // We don't want the old OpenGL
#endif

    // Open a window and create its OpenGL context
    GLFWwindow *window = glfwCreateWindow(
        WINDOW_WIDTH, WINDOW_HEIGHT, "Zero Physics Hello World", NULL, NULL);
    if (window == NULL) {
        fprintf(stderr, "Failed to open GLFW window.\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    vgCreateContextMNK(WINDOW_WIDTH, WINDOW_HEIGHT,
                       VG_RENDERING_BACKEND_TYPE_OPENGL33);

    // create fill and stroke paints
    VGPaint fill_paint = vgCreatePaint();
    vgSetPaint(fill_paint, VG_FILL_PATH);
    VGfloat fill_color[4] = {0.0f, 1.0f, 0.0f, 1.0f};
    vgSetParameterfv(fill_paint, VG_PAINT_COLOR, 4, &fill_color[0]);

    VGPaint stroke_paint = vgCreatePaint();
    vgSetPaint(stroke_paint, VG_STROKE_PATH);
    VGfloat stroke_color[4] = {1.0f, 0.0f, 0.0f, 1.0f};
    vgSetParameterfv(stroke_paint, VG_PAINT_COLOR, 4, &stroke_color[0]);

    // create a simple box path
    VGPath path;
    path = vgCreatePath(VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1, 0, 0, 0,
                        VG_PATH_CAPABILITY_ALL);
    vguRect(path, 0.0f, 0.0f, 100.0f, 150.0f);

    // load and create an opencv image
    int img_width, img_height, img_channels;

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    // set viewport
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    /// initialize the physics system
    std::shared_ptr<zo::PhysicsSystem2d> physics_system =
        zo::PhysicsSystem2d::create();

    zo::PhysicsSystem2d::force_handle_t up =
        physics_system->addGlobalForce(glm::vec2(0.0f, 1));
    zo::PhysicsSystem2d::force_handle_t down =
        physics_system->addGlobalForce(glm::vec2(0.0f, 2));
    physics_system->removeGlobalForce(up);
    zo::PhysicsSystem2d::force_handle_t right =
        physics_system->addGlobalForce(glm::vec2(3, 0.0f));
    zo::PhysicsSystem2d::force_handle_t left =
        physics_system->addGlobalForce(glm::vec2(4, 0.0f));

    glm::vec2 force = physics_system->getGlobalForce(right).value_or(glm::vec2(0, 0));
    std::cout << force.x << " " << force.y << std::endl << std::endl;

    for (auto f : physics_system->globalForces()) {
        std::cout << f.x << " " << f.y << std::endl;
    }

    do {

        // Clear the screen.
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        /// do an ortho camera
        // NOTE:  this is not standard OpenVG
        vgPushOrthoCamera(0.0f, (float)width, (float)height, 0.0f, -1.0f, 1.0f);

        physics_system->update(1.0f / 60.0f);

        /// draw the basic path
        // set up path trasnform
        vgSeti(VG_MATRIX_MODE, VG_MATRIX_PATH_USER_TO_SURFACE);
        vgLoadIdentity();
        vgTranslate(width / 2, height / 2);

        // stroke wideth
        vgSetf(VG_STROKE_LINE_WIDTH, 5.0f);

        // fill and stroke paints
        vgSetPaint(fill_paint, VG_FILL_PATH);
        vgSetPaint(stroke_paint, VG_STROKE_PATH);

        // draw the path with fill and stroke
        vgDrawPath(path, VG_FILL_PATH | VG_STROKE_PATH);

        // pop the ortho camera
        vgPopOrthoCamera();

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

    } // Check if the ESC key was pressed or the window was closed
    while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0);

    // destroy MonkVG
    vgDestroyPath(path);
    vgDestroyPaint(fill_paint);
    vgDestroyContextMNK();

    glfwDestroyWindow(window);

    return 0;
}
