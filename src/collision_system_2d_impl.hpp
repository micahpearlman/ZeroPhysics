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
#include <optional>
#include <vector>

namespace zo {

class CollisionSystem2dImpl : public CollisionSystem2d {
  public:
    CollisionSystem2dImpl(size_t max_colliders);
    ~CollisionSystem2dImpl() = default;

    void destroyCollider(collider_handle_2d_t hndl) override;
    void destroyCollider(std::unique_ptr<Collider2d> collider) override;

    /// @brief create a collider of a specific type
    /// @param type
    /// @return
    std::optional<collider_handle_2d_t> createCollider(ColliderType type);

    template <typename T> T &getColliderData(const collider_handle_2d_t &hndl) {
        if constexpr (std::is_same_v<T, CircleCollider2dImpl::Data>) {
            return *_circle_collider_pool.idxToPtr(hndl.index);
        } else if constexpr (std::is_same_v<T, LineCollider2dImpl::Data>) {
            return *_line_collider_pool.idxToPtr(hndl.index);
        } else {
            static_assert(std::is_same_v<T, CircleCollider2dImpl::Data> ||
                              std::is_same_v<T, LineCollider2dImpl::Data>,
                          "Invalid collider type");
        }
    }

    Collider2dImpl::Data &getBaseColliderData(const collider_handle_2d_t &hndl);

    void generateCollisionPairs() override;

    /// @brief Get the collision pairs
    /// @return
    std::vector<CollisionPair> &collisionPairs() {
        return _collision_pairs;
    }

  private:
    MemoryPool<CircleCollider2dImpl::Data> _circle_collider_pool;
    MemoryPool<LineCollider2dImpl::Data>   _line_collider_pool;
    ComponentStore<ColliderHandle>         _colliders;

    std::vector<CollisionPair> _collision_pairs;
};

} // namespace zo
#endif // __zoPhysicsCollisionSys2dImpl_h__
