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
struct ColliderHandle;
using collider_handle_2d_t = ColliderHandle;

struct line_segment_2d_t {
    union {
        struct {
            glm::vec2 start;
            glm::vec2 end;
        } s;

        std::array<glm::vec2, 2> a;

    } v;
};

struct aabb_2d_t {
    glm::vec2 min;
    glm::vec2 max;
};

struct circle_2d_t {
    union {
        struct {
            glm::vec2 center;
        } s;

        std::array<glm::vec2, 1> a;
    } v;

    float radius;
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
