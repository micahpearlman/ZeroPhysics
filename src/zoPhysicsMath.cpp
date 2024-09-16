#include "zoPhysicsMath.hpp"

namespace zo {
glm::vec2 closest_point_on_line_segment(const glm::vec2         &p,
                                        const line_segment_2d_t &ls) {
    // project p onto the line segment
    glm::vec2 ls_diff = ls.end - ls.start;
    float     t = glm::dot(p - ls.start, ls_diff) / glm::dot(ls_diff, ls_diff);

    // clamp t to the line segment
    t = glm::clamp(t, 0.0f, 1.0f);

    // calculate the closest point from t
    glm::vec2 closest_point = (ls.start + ls_diff) * t;

    return closest_point;
}

std::optional<contact_2d_t>
circle_to_line_segment(const circle_2d_t &s, const line_segment_2d_t &ls) {
    // calculate the closest point on the line segment to the circle
    glm::vec2 closest_point = closest_point_on_line_segment(s.center, ls);

    // calculate the distance between the closest point and the circle center
    glm::vec2 diff = s.center - closest_point;
    float squared_distance = glm::dot(diff, diff);

    // if the distance is less than the radius of the circle then we have a
    // collision
    if (squared_distance < s.radius * s.radius) {
        // calculate the normal
        glm::vec2 normal = glm::normalize(diff);

        // calculate the penetration
        float penetration = s.radius - glm::sqrt(squared_distance);

        // calculate the contact point
        glm::vec2 contact_point = s.center - (normal * s.radius);

        return contact_2d_t{normal, contact_point, penetration};
    }
    return std::nullopt;
}

} // namespace zo