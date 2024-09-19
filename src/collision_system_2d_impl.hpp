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
#include "collider_2d_impl.hpp"
#include <variant>
namespace zo {

class CollisionSystem2dImpl : public CollisionSystem2d {
  public:
    static std::shared_ptr<CollisionSystem2d> create();
    virtual collider_handle_2d_t
    createCollider(Collider2d::ColliderType type) override;

  private:
    using ColliderDataVariant = std::variant<CircleCollider2dImpl::Data, LineCollider2dImpl::Data>;
    MemoryPool<ColliderDataVariant> _collider_data_pool;
};

} // namespace zo
#endif // __zoPhysicsCollisionSys2dImpl_h__
