/**
 * @file zoPhysicsCollisionSys2dImpl.hpp
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
#include <zoPhysicsCollisionSys2d.hpp>
#include <zoPhysicsTypes.hpp>
#include <zoPhysicsMemory.hpp>

namespace zo {

class CollisionSystem2dImpl : public CollisionSystem2d {
public:
    static std::shared_ptr<CollisionSystem2d> create();
    virtual collider_handle_2d_t createCollider(ColliderType type) override;

    private:

};

} // namespace zo
#endif // __zoPhysicsCollisionSys2dImpl_h__
