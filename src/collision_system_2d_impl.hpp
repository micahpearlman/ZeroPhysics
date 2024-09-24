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
#include "types_impl.hpp"
namespace zo {

class CollisionSystem2dImpl : public CollisionSystem2d {
  public:
    CollisionSystem2dImpl(size_t max_colliders);
    ~CollisionSystem2dImpl() = default;

    std::unique_ptr<Collider2d>
    createCollider(ColliderType type) override;

    template <typename T> T &getColliderData(collider_handle_2d_t hndl) {
        if constexpr (std::is_same_v<T, CircleCollider2dImpl::Data>) {
            return *_circle_collider_pool.idxToPtr(hndl.index);
        }
        else if constexpr (std::is_same_v<T, LineCollider2dImpl::Data>) {
            return *_line_collider_pool.idxToPtr(hndl.index);
        } else {
            static_assert(std::is_same_v<T, CircleCollider2dImpl::Data> ||
                          std::is_same_v<T, LineCollider2dImpl::Data>, "Invalid collider type");
        }

    }

  private:
    MemoryPool<CircleCollider2dImpl::Data> _circle_collider_pool;
    MemoryPool<LineCollider2dImpl::Data>   _line_collider_pool;
};

} // namespace zo
#endif // __zoPhysicsCollisionSys2dImpl_h__
