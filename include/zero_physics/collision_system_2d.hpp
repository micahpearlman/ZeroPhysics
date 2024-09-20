/**
 * @file collision_system_2d.hpp
 * @author Micah Pearlman (micahpearlman@gmail.com)
 * @brief Manages collision detection.  Ultimately returns a list of collision pairs.
 * @version 0.1
 * @date 2024-09-12
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef __zoPhysicsCollisionSys_hpp__
#define __zoPhysicsCollisionSys_hpp__
#include <zero_physics/types.hpp>
#include <zero_physics/collider_2d.hpp>
#include <memory>
namespace zo {
class CollisionSystem2d {
public:

    
    /// @brief Create collision system implementation.
    /// @param max_colliders Maximum number of colliders.
    /// @return 
    static std::shared_ptr<CollisionSystem2d> create(size_t max_colliders);

    virtual ~CollisionSystem2d() = default;

    virtual collider_handle_2d_t createCollider(Collider2d::ColliderType type) = 0;
};

}
#endif // __zoPhysicsCollisionSys_hpp__