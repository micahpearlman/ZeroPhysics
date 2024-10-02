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
#include "broad_phase.hpp"
#include <optional>
#include <vector>

namespace zo {

class CollisionSystem2dImpl : public CollisionSystem2d {
  public:
    CollisionSystem2dImpl(size_t max_colliders, BroadPhaseType broad_phase_type);
    ~CollisionSystem2dImpl() = default;

    void destroyCollider(collider_handle_2d_t hndl) override;
    void destroyCollider(std::unique_ptr<Collider2d> collider) override;

    /// @brief create a collider of a specific type
    /// @param type
    /// @return
    std::optional<collider_handle_2d_t> createCollider(ColliderType type);

    /// @brief Get specialized collider data (CircleCollider2dImpl::Data or
    /// LineCollider2dImpl::Data) from the collider handle.
    /// @tparam T
    /// @param hndl the collider handle
    /// @return The collider data
    template <typename T> T &getColliderData(const collider_handle_2d_t &hndl) {
        if constexpr (std::is_same_v<T, CircleCollider2dImpl::Data>) {
            return _circle_collider_pool[hndl.index];
        } else if constexpr (std::is_same_v<T, LineCollider2dImpl::Data>) {
            return _line_collider_pool[hndl.index];
        } else {
            static_assert(std::is_same_v<T, CircleCollider2dImpl::Data> ||
                              std::is_same_v<T, LineCollider2dImpl::Data>,
                          "Invalid collider type");
        }
    }

    /// @brief Get specialized collider data (CircleCollider2dImpl::Data or
    /// LineCollider2dImpl::Data) from the collider handle.
    /// @tparam T
    /// @param hndl the collider handle
    /// @return The collider data
    template <typename T>
    const T &getColliderData(const collider_handle_2d_t &hndl) const {
        if constexpr (std::is_same_v<T, CircleCollider2dImpl::Data>) {
            return _circle_collider_pool[hndl.index];
        } else if constexpr (std::is_same_v<T, LineCollider2dImpl::Data>) {
            return _line_collider_pool[hndl.index];
        } else {
            static_assert(std::is_same_v<T, CircleCollider2dImpl::Data> ||
                              std::is_same_v<T, LineCollider2dImpl::Data>,
                          "Invalid collider type");
        }
    }

    const Collider2dImpl::Data &
    getBaseColliderData(const collider_handle_2d_t &hndl) const;

    void generateCollisionPairs() override;

    /// @brief Get the collision pairs
    /// @return
    const std::vector<CollisionPair> &collisionPairs() const {
        return _collision_pairs;
    }

    /// @brief Get the collider store
    /// @return const ComponentStore<ColliderHandle>& the collider store
    const ComponentStore<ColliderHandle> &colliders() const {
        return _colliders;
    }

  private:
    MemoryPool<CircleCollider2dImpl::Data> _circle_collider_pool;
    MemoryPool<LineCollider2dImpl::Data>   _line_collider_pool;
    ComponentStore<ColliderHandle>         _colliders;

    std::unique_ptr<BroadPhase> _broad_phase = nullptr;

    std::vector<CollisionPair> _collision_pairs;
};

} // namespace zo
#endif // __zoPhysicsCollisionSys2dImpl_h__
