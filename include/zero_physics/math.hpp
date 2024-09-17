/**
 * @file math.hpp
 * @author Micah Pearlman (micahpearlman@gmail.com)
 * @brief
 * @version 0.1
 * @date 2024-09-13
 *
 * @copyright Copyright (c) 2024
 *
 */
#ifndef __zoPhysicsMath_h__
#define __zoPhysicsMath_h__
#include <zero_physics/types.hpp>
#include <optional>

namespace zo {
/// @brief Calculate the closest point on a line segment to a point
/// @param p
/// @param ls
/// @return glm::vec2 point on line segment closest to p.
glm::vec2 closest_point_on_line_segment(const glm::vec2         &p,
                                        const line_segment_2d_t &ls);

/// @brief collide circle with line segment
/// @param s
/// @param ls
/// @return optional<contact_2d_t> contact information or std::nullopt if no
/// collision
std::optional<contact_2d_t> circle_to_line_segment(const circle_2d_t       &s,
                                                   const line_segment_2d_t &ls);

} // namespace zo

#endif // __zoPhysicsMath_h__