/**
 * @file zoPhysicsTypes.hpp
 * @author Micah Pearlman (micahpearlman@gmail.com)
 * @brief
 * @version 0.1
 * @date 2024-09-10
 *
 * @copyright Copyright (c) 2024
 *
 */
#ifndef __zoPhysicsTypes_h__
#define __zoPhysicsTypes_h__
#include <cstdint>
#include <glm/glm.hpp>

namespace zo {
// handles
using force_handle_2d_t = uint64_t;
using phy_obj_handle_2d_t = uint64_t;

// collider handle
union ColliderHandle;
using collider_handle_2d_t = ColliderHandle;

// collider pair
struct CollisionPair;
using collider_pair_t = CollisionPair;

// collider enum
enum class ColliderType { CIRCLE = 1, LINE = 2, BOX = 3, MAX = 4 };

struct line_segment_2d_t {
    glm::vec2 start;
    glm::vec2 end;
};

struct aabb_2d_t {
    glm::vec2 min;
    glm::vec2 max;
};

struct circle_2d_t {
    glm::vec2 center;
    float     radius;
};

struct ray_2d_t {
    glm::vec2 origin;
    glm::vec2 direction;
};

struct contact_2d_t {
    glm::vec2 normal;
    glm::vec2 point;
    float     penetration;
};

} // namespace zo
#endif // __zoPhysicsTypes_h__
