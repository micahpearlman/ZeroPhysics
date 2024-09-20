#include <gtest/gtest.h>

#include "test_memory.hpp"
#include "test_collision_system_2d.hpp"
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
