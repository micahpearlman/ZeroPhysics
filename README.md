# ZeroPhysics

ZeroPhysics is a high performance 2D physics engine implemented in C++.  It is designed to be simple to use and easy to integrate into other projects. It is uses a Verlet integration method to solve the equations of motion.

## Dependencies

Unit tests require gtest to be installed.  On Ubuntu, this can be done with:

```bash
sudo apt-get install libgtest-dev
```

On MacOS, this can be done with:

```bash
brew install gtest
```

Examples require glfw3, OpenGL.

```bash
sudo apt-get install libglfw3
sudo apt-get install libglfw3-dev
```

## Build

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release && cmake --build build
```

## Quick Start

Hello World example that creates a dynamic object and a floor plane.

```cpp
#include <zero_physics/physics_system_2d.hpp>
#include <iostream>
#include <memory>
#include <thread>
#include <chrono>

int main(int argc, char **argv) {
    std::cout << "Hello, ZeroPhysics!\n";

    // create a physics system
    std::shared_ptr<zo::PhysicsSystem2d> physics_system =
        zo::PhysicsSystem2d::create(
            1024,                     // max number of physics objects
            1,                        // number of iterations
            zo::BroadPhaseType::NAIVE // broad phase collision scheme
        );

    // set gravity
    physics_system->setGravity({0, 100.0f});

    // create a dynamic physics object
    std::unique_ptr<zo::PhysicsObject2d> ball_phy_obj =
        physics_system->createPhysicsObject();
    ball_phy_obj->setPosition({10, 0});

    // create a circle collider for the ball physics object
    std::unique_ptr<zo::CircleCollider2d> circle_collider =
        physics_system->collisionSystem()
            .createCollider<zo::CircleCollider2d>();
    circle_collider->setRadius(1.0f);

    // set the circle collider for the physics object
    ball_phy_obj->setCollider(*circle_collider, 0);

    // create a floor collider
    std::unique_ptr<zo::LineCollider2d> floor_col =
        physics_system->collisionSystem().createCollider<zo::LineCollider2d>();
    zo::thick_line_segment_2d_t floor_segment = {
        {{0.0f, 10.0f}, {20.0f, 10.0f}}, 1.0f};
    floor_col->setLine(floor_segment);

    // run update loop
    int frame_cnt = 250;
    while (frame_cnt--) {
        physics_system->update(0.01f); // update physics system

        if (frame_cnt % 4 == 0) {
            std::cout << "Ball position: " << ball_phy_obj->position().x << ", "
                      << ball_phy_obj->position().y << "\n";
        }

        // add delay
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    return 0;
}

```
## Examples

### Simple Example

<img src="./hello-world.gif" width=480>


Simple example with rendering that creates two balls and a container box.  

To run the example:
```bash
./build/examples/hello_world
```

### Stress Test

<img src="./stress.gif" width=480>

Stress test creates a large number of balls and a shooter ball.  The shooter ball is fired into the container and bounces around.  Press the space bar to fire the shooter ball.
