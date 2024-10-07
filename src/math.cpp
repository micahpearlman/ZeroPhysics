/**
 * @file math.cpp
 * @author Micah Pearlman (micahpearlman@gmail.com)
 * @brief
 * @version 0.1
 * @date 2024-09-17
 *
 * @copyright Copyright (c) 2024
 *
 */
#include <zero_physics/math.hpp>
#include <iostream>

namespace zo {
glm::vec2 closestPointOnLineSegment(const glm::vec2         &p,
                                    const line_segment_2d_t &ls) {
    // project p onto the line segment
    glm::vec2 ls_diff = ls.end - ls.start;
    float     t = glm::dot(p - ls.start, ls_diff) / glm::dot(ls_diff, ls_diff);

    // clamp t to the line segment
    t = glm::clamp(t, 0.0f, 1.0f);

    // calculate the closest point from t
    glm::vec2 closest_point = ls.start + (ls_diff * t);

    return closest_point;
}

bool circleToLineSegment(const circle_2d_t &c, const line_segment_2d_t &ls,
                         contact_2d_t &contact) {
    // calculate the closest point on the line segment to the circle
    glm::vec2 closest_point = closestPointOnLineSegment(c.center, ls);

    // calculate the distance between the closest point and the circle center
    glm::vec2 diff = c.center - closest_point;
    float     squared_distance = glm::dot(diff, diff);

    // if the distance is less than equal to the radius of the circle then we
    // have a collision.  NOTE: using squared distance to avoid sqrt until we
    // need it
    if (squared_distance > c.radius * c.radius) {
        return false;
    }

    // calculate the normal
    contact.normal = glm::normalize(diff);

    // calculate the penetration
    contact.penetration = c.radius - glm::sqrt(squared_distance);

    // calculate the contact point
    contact.point = c.center - (contact.normal * c.radius);

    return true;
}

bool circleToThickLineSegment(const circle_2d_t             &c,
                              const thick_line_segment_2d_t &ls,
                              contact_2d_t                  &contact) {
    // calculate the closest point on the line segment to the circle
    glm::vec2 closest_point = closestPointOnLineSegment(c.center, ls.line);

    // now it is just a circle to circle collision
    circle_2d_t ls_circle{closest_point, ls.radius};
    return circleToCircle(c, ls_circle, contact);

}

bool circleToCircle(const circle_2d_t &c1, const circle_2d_t &c2,
                    contact_2d_t &contact) {
    // calculate squared distance between the two circle centers
    glm::vec2   diff = c2.center - c1.center;
    const float squared_distance = glm::dot(diff, diff);

    // if squared distance is less than or equal to the sum of the radii squared
    // then we have a collision
    const float radius_sum = c1.radius + c2.radius;
    const float radius_squared = radius_sum * radius_sum;
    if (squared_distance > radius_squared) {
        return false;
    }
    // calculate the normal
    contact.normal = glm::normalize(diff);

    // calculate the penetration
    contact.penetration = radius_sum - glm::sqrt(squared_distance);

    // calculate the contact point which is the collision normal times the
    // radius of c1
    contact.point = c1.center + (contact.normal * c1.radius);

    return true;
}
} // namespace zo
