/**
 * @file test_math.cpp
 * @brief Unit tests for math functions in the zo namespace
 */

#include <gtest/gtest.h>
#include <zero_physics/math.hpp>
#include <glm/glm.hpp>
#include <optional>

using namespace zo;

TEST(MathTest, ClosestPointOnLineSegment) {
    line_segment_2d_t ls{glm::vec2(0.0f, 0.0f), glm::vec2(10.0f, 0.0f)};
    glm::vec2 p(5.0f, 5.0f);

    glm::vec2 closest_point = closestPointOnLineSegment(p, ls);
    EXPECT_EQ(closest_point, glm::vec2(5.0f, 0.0f));

    p = glm::vec2(-5.0f, 5.0f);
    closest_point = closestPointOnLineSegment(p, ls);
    EXPECT_EQ(closest_point, glm::vec2(0.0f, 0.0f));

    p = glm::vec2(15.0f, 5.0f);
    closest_point = closestPointOnLineSegment(p, ls);
    EXPECT_EQ(closest_point, glm::vec2(10.0f, 0.0f));
}

TEST(MathTest, CircleToLineSegmentCollision) {
    circle_2d_t circle{glm::vec2(5.0f, 5.0f), 6.0f};
    line_segment_2d_t ls{glm::vec2(0.0f, 0.0f), glm::vec2(10.0f, 0.0f)};

    auto contact = circleToLineSegment(circle, ls);
    ASSERT_TRUE(contact.has_value());
    EXPECT_EQ(contact->normal, glm::vec2(0.0f, 1.0f));
    EXPECT_EQ(contact->point, glm::vec2(5.0f, -1.0f));
    EXPECT_NEAR(contact->penetration, 1.0f, 1e-5);

    circle = circle_2d_t{glm::vec2(15.0f, 5.0f), 5.0f};
    contact = circleToLineSegment(circle, ls);
    EXPECT_FALSE(contact.has_value());

    circle = circle_2d_t{glm::vec2(5.0f, -5.0f), 5.0f};
    contact = circleToLineSegment(circle, ls);
    ASSERT_TRUE(contact.has_value());
    EXPECT_EQ(contact->normal, glm::vec2(0.0f, -1.0f));
    EXPECT_EQ(contact->point, glm::vec2(5.0f, 0.0f));
    EXPECT_NEAR(contact->penetration, 0.0f, 1e-5);
}
 