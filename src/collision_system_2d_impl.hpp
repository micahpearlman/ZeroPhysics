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
    CollisionSystem2dImpl(size_t max_colliders);
    ~CollisionSystem2dImpl() = default;

    std::unique_ptr<Collider2d>
    createCollider(Collider2d::ColliderType type) override;

    template <typename T>
    T &getColliderData(collider_handle_2d_t hndl) {
        ColliderDataVariant* data_variant = _collider_data_pool.idxToPtr(hndl);
        return std::get<T>(*data_variant);
    }
  private:
    using ColliderDataVariant =
        std::variant<CircleCollider2dImpl::Data, LineCollider2dImpl::Data>;
    MemoryPool<ColliderDataVariant> _collider_data_pool;
};

} // namespace zo
#endif // __zoPhysicsCollisionSys2dImpl_h__
