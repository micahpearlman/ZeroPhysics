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

class GameObject {
  private:
    VGPath  _path;
    VGPaint _fill_paint;
    VGPaint _stroke_paint;

    std::unique_ptr<zo::PhysicsObject2d> _phy_obj;
    std::shared_ptr<zo::PhysicsSystem2d> _phy_sys;

  public:
    GameObject(std::shared_ptr<zo::PhysicsSystem2d> physics_system)
        : _phy_sys(physics_system) {
        // create a physics object
        _phy_obj = _phy_sys->createPhysicsObject();

        // create path
        _path = vgCreatePath(VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1, 0,
                             0, 0, VG_PATH_CAPABILITY_ALL);
        // create fill paint
        _fill_paint = vgCreatePaint();

        // create stroke paint
        _stroke_paint = vgCreatePaint();
    }

    zo::PhysicsObject2d &physicsObject() { return *_phy_obj; }

    VGPath &path() { return _path; }

    VGPaint &fillPaint() { return _fill_paint; }

    VGPaint &strokePaint() { return _stroke_paint; }

    void draw() {

        // set up path trasnform
        vgSeti(VG_MATRIX_MODE, VG_MATRIX_PATH_USER_TO_SURFACE);
        vgLoadIdentity();
        glm::vec2 pos = _phy_obj->position();
        vgTranslate(pos.x, pos.y);

        // set stroke width
        vgSetf(VG_STROKE_LINE_WIDTH, 5.0f);

        // fill and stroke paints
        vgSetPaint(_fill_paint, VG_FILL_PATH);
        vgSetPaint(_stroke_paint, VG_STROKE_PATH);

        // draw the path with fill and stroke
        vgDrawPath(_path, VG_FILL_PATH | VG_STROKE_PATH);
    }

    ~GameObject() {
        vgDestroyPath(_path);
        vgDestroyPaint(_fill_paint);
        vgDestroyPaint(_stroke_paint);
        _phy_obj.reset();
    }
};

class Ball : public GameObject {
  public:
    Ball(std::shared_ptr<zo::PhysicsSystem2d> physics_system, float radius)
        : GameObject(physics_system) {

        // create a circle collider
        zo::CollisionSystem2d &collision_system =
            physics_system->collisionSystem();
        std::unique_ptr<zo::CircleCollider2d> circle_collider =
            collision_system.createCollider<zo::CircleCollider2d>();
        circle_collider->setRadius(radius);
        physicsObject().setCollider(*circle_collider, 0);

        // create a circle path
        vguEllipse(path(), 0.0f, 0.0f, radius, radius);

        // generate random fill color
        VGfloat fill_color[4] = {(float)rand() / (float)RAND_MAX,
                                 (float)rand() / (float)RAND_MAX,
                                 (float)rand() / (float)RAND_MAX, 1.0f};
        vgSetParameterfv(fillPaint(), VG_PAINT_COLOR, 4, &fill_color[0]);

        // generate random stroke color
        VGfloat stroke_color[4] = {(float)rand() / (float)RAND_MAX,
                                   (float)rand() / (float)RAND_MAX,
                                   (float)rand() / (float)RAND_MAX, 1.0f};
        vgSetParameterfv(strokePaint(), VG_PAINT_COLOR, 4, &stroke_color[0]);
    }
};

class StaticBox : public GameObject {
  private:
    std::vector<std::unique_ptr<zo::LineCollider2d>> _line_colliders;

  public:
    StaticBox(std::shared_ptr<zo::PhysicsSystem2d> physics_system, float width,
              float height)
        : GameObject(physics_system) {

        // physicsObject().setPosition({width/2, height/2});
        physicsObject().setMass(0); // static object

        glm::vec2                offset = {width / 2, height / 2};
        std::array<glm::vec2, 4> vertices = {
            {glm::vec2(-width / 2, -height / 2) + offset,
             glm::vec2(width / 2, -height / 2) + offset,
             glm::vec2(width / 2, height / 2) + offset,
             glm::vec2(-width / 2, height / 2) + offset}};

        // create a series of line segment colliders from vertices
        zo::CollisionSystem2d &collision_system =
            physics_system->collisionSystem();
        for (int i = 0; i < 4; i++) {
            std::unique_ptr<zo::LineCollider2d> line_collider =
                collision_system.createCollider<zo::LineCollider2d>();
            zo::line_segment_2d_t line_segment = {vertices[i],
                                                  vertices[(i + 1) % 4]};
            line_collider->setLine(line_segment);
            _line_colliders.push_back(std::move(line_collider));
        }
        // create a box path from the vertices
        vguPolygon(path(), (VGfloat *)&vertices, vertices.size(), VG_TRUE);

        // generate random fill color
        VGfloat fill_color[4] = {(float)rand() / (float)RAND_MAX,
                                 (float)rand() / (float)RAND_MAX,
                                 (float)rand() / (float)RAND_MAX, 0.01f};
        vgSetParameterfv(fillPaint(), VG_PAINT_COLOR, 4, &fill_color[0]);

        // generate random stroke color
        VGfloat stroke_color[4] = {(float)rand() / (float)RAND_MAX,
                                   (float)rand() / (float)RAND_MAX,
                                   (float)rand() / (float)RAND_MAX, 1.0f};
        vgSetParameterfv(strokePaint(), VG_PAINT_COLOR, 4, &stroke_color[0]);
    }
};

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
    glfwSwapInterval( 0 );
    vgCreateContextMNK(WINDOW_WIDTH, WINDOW_HEIGHT,
                       VG_RENDERING_BACKEND_TYPE_OPENGL33);

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    // set viewport
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    /// random seed
    srand(time(NULL));

    /// initialize the physics system
    constexpr int MAX_PHYSICS_OBJECTS = 1000 * 10;
    ;
    std::shared_ptr<zo::PhysicsSystem2d> physics_system =
        zo::PhysicsSystem2d::create(MAX_PHYSICS_OBJECTS, 1);

    std::vector<std::unique_ptr<GameObject>> game_objects;

    // create a static box
    std::unique_ptr<StaticBox> box =
        std::make_unique<StaticBox>(physics_system, width, 500);

    game_objects.push_back(std::move(box));

    // create a row of balls with random velocities
    const float BALL_RADIUS = 5.0f;
    const float X_OFFSET = BALL_RADIUS + 1.0f;
    const float Y_POS = 30;
    glm::vec2   pos = {X_OFFSET, Y_POS};
    const int   NUM_BALLS_ROW = width / ((BALL_RADIUS + 1.0f) * 2);
    const int   NUM_ROWS = 10;
    for (int r = 0; r < NUM_ROWS; r++) {
        pos += glm::vec2(0, BALL_RADIUS * 2);
        for (int i = 0; i < NUM_BALLS_ROW; i++) {
            std::unique_ptr<Ball> ball =
                std::make_unique<Ball>(physics_system, BALL_RADIUS);
            pos += glm::vec2(X_OFFSET * 2, 0);
            ball->physicsObject().setPosition(pos);
            ball->physicsObject().setVelocity(
                {(float)(rand() % 200) - 100, (float)(rand() % 200) - 100});
            game_objects.push_back(std::move(ball));
        }
    }

    // create a ball
    // std::unique_ptr<Ball> ball =
    // std::make_unique<Ball>(physics_system, 50.0f);
    // ball->physicsObject().setPosition({300, 100});
    // ball->physicsObject().setVelocity({50, 0});
    // game_objects.push_back(std::move(ball));

    // set gravity
    physics_system->addGlobalForce({0, 120.0f});

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

        // draw the game objects
        for (auto &go : game_objects) {
            go->draw();
        }

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
        // if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        //     // reset all objects

        //     ball_phy_obj->setPosition({300, 100});
        //     ball_phy_obj->setVelocity({500, 0});
        // }

    } // Check if the ESC key was pressed or the window was closed
    while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0);

    // destroy MonkVG
    vgDestroyContextMNK();

    glfwDestroyWindow(window);

    return 0;
}
