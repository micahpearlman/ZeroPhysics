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

class MemoryPoolTest : public ::testing::Test {
  protected:
    MemoryPool<TestComponent> pool;

    void SetUp() override {
        // Initialize any necessary data here
    }

    void TearDown() override {
        // Clean up any necessary data here
    }
};

TEST_F(MemoryPoolTest, AllocateAndDeallocate) {
    auto *component = pool.allocate();
    ASSERT_NE(component, nullptr);
    component->value = 42;
    EXPECT_EQ(component->value, 42);

    pool.deallocate(component);
    auto *new_component = pool.allocate();
    ASSERT_NE(new_component, nullptr);
    component->value = 13;
    EXPECT_EQ(new_component->value,
              13); // Memory is reused, so value should be the same
}

TEST_F(MemoryPoolTest, AllocateUntilExhaustion) {
    std::vector<TestComponent *> allocated_components;
    for (size_t i = 0; i < 1024; ++i) {
        auto *component = pool.allocate();
        ASSERT_NE(component, nullptr);
        component->value = -i;
        allocated_components.push_back(component);
    }

    auto *component = pool.allocate();
    EXPECT_EQ(component, nullptr); // Pool should be exhausted

    for (auto *comp : allocated_components) {
        pool.deallocate(comp);
    }
    // print out the allocated components
    printf("done");
}

TEST_F(MemoryPoolTest, UseAsStdAllocator) {
    std::vector<TestComponent, MemoryPool<TestComponent>> vec(pool);
    vec.push_back(TestComponent{42});
    vec.push_back(TestComponent{84});

    ASSERT_EQ(vec.size(), 2);
    EXPECT_EQ(vec[0].value, 42);
    EXPECT_EQ(vec[1].value, 84);
}

} // namespace zo

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
