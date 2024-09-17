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
#include <memory>
namespace zo {
class CollisionSystem2d {
public:
    // collider enum
    enum class ColliderType {
        CIRCLE,
        LINE,
        BOX
    };

    
    static std::shared_ptr<CollisionSystem2d> create();

    virtual ~CollisionSystem2d() = default;

    virtual collider_handle_2d_t createCollider(ColliderType type) = 0;
};

}
#endif // __zoPhysicsCollisionSys_hpp__