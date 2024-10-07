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

// Zero Physics
#include <zero_physics/physics_system_2d.hpp>
#include <zero_physics/collider_2d.hpp>

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
#include <sstream>
#include <string>
#include <iomanip>
#include <memory>

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

    VGPaint fill_paint_2 = vgCreatePaint();
    vgSetPaint(fill_paint_2, VG_FILL_PATH);
    VGfloat fill_color_2[4] = {0.0f, 0.0f, 0.0f, 1.0f};
    vgSetParameterfv(fill_paint_2, VG_PAINT_COLOR, 4, &fill_color_2[0]);

    VGPaint stroke_paint = vgCreatePaint();
    vgSetPaint(stroke_paint, VG_STROKE_PATH);
    VGfloat stroke_color[4] = {0.0f, 0.0f, 1.0f, 1.0f};
    vgSetParameterfv(stroke_paint, VG_PAINT_COLOR, 4, &stroke_color[0]);

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    // set viewport
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    /// initialize the physics system
    std::shared_ptr<zo::PhysicsSystem2d> physics_system =
        zo::PhysicsSystem2d::create(1024, 1, zo::BroadPhaseType::NAIVE);

    constexpr float MASS_A = 1;
    constexpr float MASS_B = 1;
    constexpr float RADIUS_A = MASS_A * 10;
    constexpr float RADIUS_B = MASS_B * 10;
    constexpr float WALL_THICKNESS = 5.0f;
// static_assert(!"how do i figure out what the collision normal is?");
    // create a physics object
    std::unique_ptr<zo::PhysicsObject2d> ball_phy_obj_1 =
        physics_system->createPhysicsObject();
    ball_phy_obj_1->setPosition({300, 300});
    ball_phy_obj_1->setVelocity({150, 0});
    ball_phy_obj_1->setMass(MASS_A);

    std::unique_ptr<zo::PhysicsObject2d> ball_phy_obj_2 =
        physics_system->createPhysicsObject();
    ball_phy_obj_2->setPosition({600, 300});
    ball_phy_obj_2->setVelocity({-150, 0});
    ball_phy_obj_2->setMass(MASS_B);

    // create a circle collider
    zo::CollisionSystem2d &collision_system = physics_system->collisionSystem();

    std::unique_ptr<zo::CircleCollider2d> circle_collider_1 =
        collision_system.createCollider<zo::CircleCollider2d>();
    circle_collider_1->setRadius(RADIUS_A);
    circle_collider_1->setRestitution(1.0f);
    ball_phy_obj_1->setCollider(*circle_collider_1, 0);

    std::unique_ptr<zo::CircleCollider2d> circle_collider_2 =
        collision_system.createCollider<zo::CircleCollider2d>();
    circle_collider_2->setRadius(RADIUS_B);
    circle_collider_2->setRestitution(1.0f);
    ball_phy_obj_2->setCollider(*circle_collider_2, 0);

    // add some gravity
    physics_system->setGravity({0, 120.0f});

    // create a small vg circle to represent the physics object
    VGPath circle_1 = vgCreatePath(VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F,
                                   1, 0, 0, 0, VG_PATH_CAPABILITY_ALL);
    vguEllipse(circle_1, 0.0f, 0.0f, RADIUS_A*2, RADIUS_A*2);

    VGPath circle_2 = vgCreatePath(VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F,
                                   1, 0, 0, 0, VG_PATH_CAPABILITY_ALL);
    vguEllipse(circle_2, 0.0f, 0.0f, RADIUS_B*2, RADIUS_B*2);

    /// walls and floor segments
    std::array<glm::vec2, 4> segments = {
        {glm::vec2{200, 100}, glm::vec2{200, 500}, glm::vec2{800, 500},
         glm::vec2{800, 100}}};
    
    // left wall collider
    zo::thick_line_segment_2d_t left_wall_segment{segments[0], segments[1],
                                                  WALL_THICKNESS};
    std::unique_ptr<zo::LineCollider2d> left_wall_col =
        collision_system.createCollider<zo::LineCollider2d>();
    left_wall_col->setLine(left_wall_segment);

    // floor collider
    zo::thick_line_segment_2d_t floor_segment = {segments[1], segments[2],
                                                 WALL_THICKNESS};
    std::unique_ptr<zo::LineCollider2d> floor_col =
        collision_system.createCollider<zo::LineCollider2d>();
    floor_col->setLine(floor_segment);

    // right wall collider
    zo::thick_line_segment_2d_t right_wall_segment = {segments[2], segments[3],
                                                      WALL_THICKNESS};
    std::unique_ptr<zo::LineCollider2d> right_wall_col =
        collision_system.createCollider<zo::LineCollider2d>();
    right_wall_col->setLine(right_wall_segment);

    // create a floor path
    VGPath box_path = vgCreatePath(VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F,
                                   1, 0, 0, 0, VG_PATH_CAPABILITY_ALL);
    vguPolygon(box_path, (VGfloat *)&segments, segments.size(), VG_FALSE);

    // velocity vector display
    VGPath velocity_vector_1 =
        vgCreatePath(VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1, 0, 0, 0,
                     VG_PATH_CAPABILITY_ALL);
    VGPath velocity_vector_2 =
        vgCreatePath(VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1, 0, 0, 0,
                     VG_PATH_CAPABILITY_ALL);

    VGPaint velocity_paint = vgCreatePaint();
    VGfloat velocity_color[4] = {1.0f, 0.0f, 0.0f, 1.0f};
    vgSetParameterfv(velocity_paint, VG_PAINT_COLOR, 4, &velocity_color[0]);

    float  last_time = glfwGetTime();
    double previous_seconds = glfwGetTime();
    int    frame_count = 0;
    do {

        // Clear the screen.
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        /// do an ortho camera
        // NOTE:  this is not standard OpenVG
        vgPushOrthoCamera(0.0f, (float)width, (float)height, 0.0f, -1.0f, 1.0f);

        // update the physics system
        float delta_time = glfwGetTime() - last_time;
        delta_time = std::min(delta_time, 0.1f);
        physics_system->update(delta_time);
        last_time = glfwGetTime();

        /// draw the basic path
        vgLoadIdentity();

        // draw the floor
        vgSeti(VG_MATRIX_MODE, VG_MATRIX_PATH_USER_TO_SURFACE);
        vgLoadIdentity();
        vgSetf(VG_STROKE_LINE_WIDTH, 5.0f);
        vgSetPaint(stroke_paint, VG_STROKE_PATH);
        vgDrawPath(box_path, VG_STROKE_PATH);

        // draw first ball
        vgLoadIdentity();
        glm::vec2 pos = ball_phy_obj_1->position();
        vgTranslate(pos.x, pos.y);
        // fill and stroke paints
        vgSetPaint(fill_paint, VG_FILL_PATH);
        vgSetPaint(stroke_paint, VG_STROKE_PATH);

        // draw the ball path with fill and stroke
        vgDrawPath(circle_1, VG_FILL_PATH | VG_STROKE_PATH);

        // draw velocity vector for the balls
        glm::vec2 vel = ball_phy_obj_1->velocity() * 20.0f;
        vgSetf(VG_STROKE_LINE_WIDTH, 2.0f);
        vgSetPaint(velocity_paint, VG_STROKE_PATH);
        vgLoadIdentity();
        vgTranslate(pos.x, pos.y);
        vgClearPath(velocity_vector_1, VG_PATH_CAPABILITY_ALL);
        VGubyte commands[] = {VG_MOVE_TO_ABS, VG_LINE_TO_ABS};
        VGfloat coords[] = {0, 0, vel.x, vel.y};
        vgAppendPathData(velocity_vector_1, 2, commands, coords);
        vgDrawPath(velocity_vector_1, VG_STROKE_PATH);

        /// draw ball 2
        vgLoadIdentity();
        pos = ball_phy_obj_2->position();
        vgTranslate(pos.x, pos.y);
        // fill and stroke paints
        vgSetPaint(fill_paint_2, VG_FILL_PATH);
        vgSetPaint(stroke_paint, VG_STROKE_PATH);

        // draw the path with fill and stroke
        vgDrawPath(circle_2, VG_FILL_PATH | VG_STROKE_PATH);

        // draw velocity vector for the balls
        vel = ball_phy_obj_2->velocity() * 20.0f;
        vgSetf(VG_STROKE_LINE_WIDTH, 2.0f);
        vgSetPaint(velocity_paint, VG_STROKE_PATH);
        vgLoadIdentity();
        vgTranslate(pos.x, pos.y);
        vgClearPath(velocity_vector_2, VG_PATH_CAPABILITY_ALL);
        VGubyte commands_2[] = {VG_MOVE_TO_ABS, VG_LINE_TO_ABS};
        VGfloat coords_2[] = {0, 0, vel.x, vel.y};
        vgAppendPathData(velocity_vector_2, 2, commands_2, coords_2);
        vgDrawPath(velocity_vector_2, VG_STROKE_PATH);

        // pop the ortho camera
        vgPopOrthoCamera();

        // calculate the frame rate

        double current_seconds = glfwGetTime();
        double elapsed_seconds = current_seconds - previous_seconds;
        if (elapsed_seconds > 0.25) {
            previous_seconds = current_seconds;
            double fps = (double)frame_count / elapsed_seconds;
            double ms_per_frame = 1000.0 / fps;

            std::stringstream ss;
            ss << "Zero Physics Hello World - " << std::fixed
               << std::setprecision(2) << ms_per_frame << " ms/frame ("
               << std::setprecision(1) << fps << " FPS)";
            std::string title = ss.str();
            glfwSetWindowTitle(window, title.c_str());
            frame_count = 0;
            // std::cout << ms_per_frame << " ms/frame (" << fps << " FPS)\n";
        }
        frame_count++;

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

        // reset ball on space bar
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
            ball_phy_obj_1->setPosition({300, 100});
            ball_phy_obj_1->setVelocity({500, 0});
        }

    } // Check if the ESC key was pressed or the window was closed
    while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0);

    // destroy MonkVG
    vgDestroyPaint(fill_paint);
    vgDestroyContextMNK();

    glfwDestroyWindow(window);

    return 0;
}
