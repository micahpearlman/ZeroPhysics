# ZeroPhysics

ZeroPhysics is a high performance 2D physics engine implemented in C++.  It is designed to be simple to use and easy to integrate into other projects. It is uses a Verlet integration method to solve the equations of motion.

## Dependencies

Unit tests require gtest to be installed.  On Ubuntu, this can be done with:

```bash
sudo apt-get install libgtest-dev
```

On McOS, this can be done with:

```bash
brew install gtest
```

Examples require glfw3 & OpenGl,

## Build

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release && cmake --build build
```

## Examples

### Hello World

<img src="./hello-world.gif" width=480>


Hello world is a simple example that creates a single "ball" and container box.  The ball is dropped into the container and bounces around.

To run the example:
```bash
./build/examples/hello_world
```

### Stress Test

<img src="./stress.gif" width=480>

Stress test creates a large number of balls and a shooter ball.  The shooter ball is fired into the container and bounces around.  Press the space bar to fire the shooter ball.