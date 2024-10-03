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

    contact_2d_t contact;
    bool result = circleToLineSegment(circle, ls, contact);
    ASSERT_TRUE(result);
    EXPECT_EQ(contact.normal, glm::vec2(0.0f, 1.0f));
    EXPECT_EQ(contact.point, glm::vec2(5.0f, -1.0f));
    EXPECT_NEAR(contact.penetration, 1.0f, 1e-5);

    circle = circle_2d_t{glm::vec2(15.0f, 5.0f), 5.0f};
    result = circleToLineSegment(circle, ls, contact);
    EXPECT_FALSE(result);

    circle = circle_2d_t{glm::vec2(5.0f, -5.0f), 5.0f};
    result = circleToLineSegment(circle, ls, contact);
    ASSERT_TRUE(result);
    EXPECT_EQ(contact.normal, glm::vec2(0.0f, -1.0f));
    EXPECT_EQ(contact.point, glm::vec2(5.0f, 0.0f));
    EXPECT_NEAR(contact.penetration, 0.0f, 1e-5);
}

TEST(MathTest, CircleToCircle) {
    circle_2d_t c1{glm::vec2(0.0f, 0.0f), 5.0f};
    circle_2d_t c2{glm::vec2(7.0f, 0.0f), 5.0f};

    contact_2d_t contact;
    bool result = circleToCircle(c1, c2, contact);
    ASSERT_TRUE(result);

    EXPECT_EQ(contact.normal, glm::vec2(-1.0f, 0.0f));
    EXPECT_EQ(contact.point, glm::vec2(-5.0f, 0.0f));
    EXPECT_NEAR(contact.penetration, 3.0f, 1e-5);

    c2 = circle_2d_t{glm::vec2(11.0f, 0.0f), 5.0f};
    result = circleToCircle(c1, c2, contact);
    EXPECT_FALSE(result);

}

TEST(MathTest, CircleToThickLineSegment) {
    circle_2d_t c{glm::vec2(0.0f, 0.0f), 5.0f};
    thick_line_segment_2d_t ls{line_segment_2d_t{glm::vec2(7.0f, 0.0f), glm::vec2(20.0f, 0.0f)}, 5.0f};

    contact_2d_t contact;
    bool result = circleToThickLineSegment(c, ls, contact);
    ASSERT_TRUE(result);
    EXPECT_EQ(contact.normal, glm::vec2(-1.0f, 0.0f));
    EXPECT_EQ(contact.point, glm::vec2(-5.0f, 0.0f));
    EXPECT_NEAR(contact.penetration, 3.0f, 1e-5);

    c = circle_2d_t{glm::vec2(-5.0f, 0.0f), 5.0f};
    result = circleToThickLineSegment(c, ls, contact);
    ASSERT_FALSE(result);


}