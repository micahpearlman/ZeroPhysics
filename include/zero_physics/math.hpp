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
glm::vec2 closestPointOnLineSegment(const glm::vec2         &p,
                                    const line_segment_2d_t &ls);

/// @brief collide circle with line segment
/// @param c circle
/// @param ls line segment
/// @param contact contact information if collision
/// @return optional<contact_2d_t> contact information or std::nullopt if no
/// collision
bool circleToLineSegment(const circle_2d_t       &c,
                                                const line_segment_2d_t &ls,
                                                contact_2d_t &contact);

/// @brief collide circle with circle
/// @param c1 circle 1
/// @param c2 circle 2
/// @param contact contact information if collision
/// @return optional<contact_2d_t> contact information or std::nullopt if no
/// collision
bool circleToCircle(const circle_2d_t &c1,
                                           const circle_2d_t &c2,
                                           contact_2d_t &contact);

} // namespace zo

#endif // __zoPhysicsMath_h__