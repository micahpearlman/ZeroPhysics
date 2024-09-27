/**
 * @file test_collision_system_2d.hpp
 * @brief Unit tests for zo::CollisionSystem2d
 */

#include <gtest/gtest.h>
#include <zero_physics/collision_system_2d.hpp>
#include <zero_physics/collider_2d.hpp>
#include <zero_physics/types.hpp>

using namespace zo;

class CollisionSystem2dTest : public ::testing::Test {
protected:
    std::shared_ptr<CollisionSystem2d> collisionSystem;

    void SetUp() override {
        collisionSystem = CollisionSystem2d::create(100);
    }

    void TearDown() override {
        // Clean up any necessary data here
    }
};

TEST_F(CollisionSystem2dTest, CreateCollisionSystem) {
    auto system = CollisionSystem2d::create(100);
    ASSERT_NE(system, nullptr);
}

TEST_F(CollisionSystem2dTest, CreateCollider) {
    auto collider = collisionSystem->createCollider<CircleCollider2d>();
    ASSERT_NE(collider, nullptr);
    EXPECT_EQ(collider->type(), ColliderType::CIRCLE);
}

TEST_F(CollisionSystem2dTest, CreateColliderToExhaustion) {
    for (size_t i = 0; i < 100; ++i) {
        auto collider = collisionSystem->createCollider<CircleCollider2d>();
        ASSERT_NE(collider, nullptr);
        EXPECT_EQ(collider->type(), ColliderType::CIRCLE);
    }

    auto collider = collisionSystem->createCollider<CircleCollider2d>();
    EXPECT_EQ(collider, nullptr);

} // namespace zo

