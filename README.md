# ZeroPhysics

A Verlet Physics Library.

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