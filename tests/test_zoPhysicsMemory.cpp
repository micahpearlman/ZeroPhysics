/**
 * @file test_zoPhysicsMemory.hpp
 * @brief Unit tests for zo::ComponentStore
 */

#include <gtest/gtest.h>
#include "zoPhysicsMemory.hpp"

namespace zo {

struct TestComponent {
    int value;
};

class ComponentStoreTest : public ::testing::Test {
  protected:
    ComponentStore<TestComponent> store;

    void SetUp() override {
        // Initialize any necessary data here
    }

    void TearDown() override {
        // Clean up any necessary data here
    }
};

TEST_F(ComponentStoreTest, AddComponent) {
    TestComponent component{42};
    auto          handle = store.add(component);

    auto retrieved = store.get(handle);
    ASSERT_TRUE(retrieved.has_value());
    EXPECT_EQ(retrieved->get().value, 42);
}

TEST_F(ComponentStoreTest, RemoveComponent) {
    TestComponent component{42};
    auto          handle = store.add(component);

    store.remove(handle);
    auto retrieved = store.get(handle);
    EXPECT_FALSE(retrieved.has_value());
}

TEST_F(ComponentStoreTest, AddMultipleComponents) {
    TestComponent component1{42};
    TestComponent component2{84};

    auto handle1 = store.add(component1);
    auto handle2 = store.add(component2);

    auto retrieved1 = store.get(handle1);
    auto retrieved2 = store.get(handle2);

    ASSERT_TRUE(retrieved1.has_value());
    ASSERT_TRUE(retrieved2.has_value());
    EXPECT_EQ(retrieved1->get().value, 42);
    EXPECT_EQ(retrieved2->get().value, 84);
}

TEST_F(ComponentStoreTest, RemoveNonExistentComponent) {
    TestComponent component{42};
    auto          handle = store.add(component);

    store.remove(handle + 1); // Attempt to remove a non-existent component
    auto retrieved = store.get(handle);
    EXPECT_TRUE(retrieved.has_value());
    EXPECT_EQ(retrieved->get().value, 42);
}

TEST_F(ComponentStoreTest, AddAndRemoveMultipleComponents) {
    TestComponent component1{42};
    TestComponent component2{84};
    TestComponent component3{126};

    auto handle1 = store.add(component1);
    auto handle2 = store.add(component2);
    auto handle3 = store.add(component3);

    store.remove(handle2);

    auto retrieved1 = store.get(handle1);
    auto retrieved2 = store.get(handle2);
    auto retrieved3 = store.get(handle3);

    ASSERT_TRUE(retrieved1.has_value());
    EXPECT_FALSE(retrieved2.has_value());
    ASSERT_TRUE(retrieved3.has_value());
    EXPECT_EQ(retrieved1->get().value, 42);
    EXPECT_EQ(retrieved3->get().value, 126);
}

} // namespace zo

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
