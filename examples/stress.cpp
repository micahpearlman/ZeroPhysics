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
#include <vector>
#include <array>
#include <random>

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

    virtual void draw() {

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

        // if (_phy_obj->mass() == 0) {
        //     std::cout << "Static object\n";
        // }
    }

    virtual ~GameObject() {
        // std::cout << "\tGameObject destroyed: " << _phy_obj->handle() << "\n";
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

    virtual ~Ball() {
        // std::cout << "Ball destroyed\n";
    }
};

class StaticBox : public GameObject {
  private:
    std::vector<std::unique_ptr<zo::LineCollider2d>> _line_colliders;

  public:
    StaticBox(std::shared_ptr<zo::PhysicsSystem2d> physics_system, float width,
              float height, glm::vec2 pos = {0, 0})
        : GameObject(physics_system) {

        physicsObject().setMass(0); // 0 is static object

        // glm::vec2                offset = {width / 2, height / 2};
        std::array<glm::vec2, 4> vertices = {
            {glm::vec2(-width / 2, -height / 2) + pos,
             glm::vec2(width / 2, -height / 2) + pos,
             glm::vec2(width / 2, height / 2) + pos,
             glm::vec2(-width / 2, height / 2) + pos}};

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


    virtual ~StaticBox() {
        std::cout << "StaticBox destroyed\n";
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
        zo::PhysicsSystem2d::create(MAX_PHYSICS_OBJECTS, 1, zo::BroadPhaseType::GRID);

    std::vector<std::unique_ptr<GameObject>> game_objects;

    // create a static box
    std::unique_ptr<StaticBox> box =
        std::make_unique<StaticBox>(physics_system, width-20, height-20, glm::vec2{width/2, height/2});
    game_objects.push_back(std::move(box));

    std::vector<float> game_obj_lifetime;
    game_obj_lifetime.push_back(0);

    // create a row of balls with random velocities
    const float BALL_RADIUS = 5.0f;
    const float OFFSET = BALL_RADIUS + 5.0f;
    const float Y_POS = 100;
    glm::vec2   pos = {OFFSET, Y_POS};
    const int   NUM_BALLS_ROW = (width - OFFSET) / (OFFSET * 2);
    const int   NUM_ROWS = 25;
    for (int r = 0; r < NUM_ROWS; r++) {
        pos += glm::vec2(0, OFFSET * 2);
        pos.x = OFFSET;
        for (int i = 0; i < NUM_BALLS_ROW; i++) {
            std::unique_ptr<Ball> ball =
                std::make_unique<Ball>(physics_system, BALL_RADIUS);
            pos += glm::vec2(OFFSET * 2, 0);
            ball->physicsObject().setPosition(pos);
            ball->physicsObject().setVelocity(
                {(float)(rand() % 200) - 100, (float)(rand() % 200) - 100});
            game_objects.push_back(std::move(ball));
            game_obj_lifetime.push_back(rand() % 30);
        }
    }

    // set gravity
    physics_system->addGlobalForce({0, 160.0f});

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
        for (int i = 0; i < game_objects.size(); i++) {
            game_objects[i]->draw();
            game_obj_lifetime[i] -= delta_time;
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

        // remove game objects that have expired
        // for (int i = game_obj_lifetime.size()-1; i >= 1; i--) {
        //     if (game_obj_lifetime[i] < 0) {
        //         game_objects.erase(game_objects.begin() + i);  
        //         game_obj_lifetime.erase(game_obj_lifetime.begin() + i);
        //     }
        // }


    } // Check if the ESC key was pressed or the window was closed
    while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0);

    // clear the game objects
    game_objects.clear();

    // destroy MonkVG
    vgDestroyContextMNK();

    glfwDestroyWindow(window);

    return 0;
}
