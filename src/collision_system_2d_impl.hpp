/**
 * @file collision_system_2d_impl.hpp
 * @author Micah Pearlman (micahpearlman@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2024-09-17
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef __zoPhysicsCollisionSys2dImpl_h__
#define __zoPhysicsCollisionSys2dImpl_h__
#include <zero_physics/collision_system_2d.hpp>
#include <zero_physics/types.hpp>
#include <zero_physics/memory.hpp>

namespace zo {

class CollisionSystem2dImpl : public CollisionSystem2d {
public:
    static std::shared_ptr<CollisionSystem2d> create();
    virtual collider_handle_2d_t createCollider(ColliderType type) override;

    private:

};

} // namespace zo
#endif // __zoPhysicsCollisionSys2dImpl_h__
