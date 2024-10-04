/**
 * @file zoPhysicsSystem2dImpl.hpp
 * @author Micah Pearlman (micahpearlman@gmail.com)
 * @brief
 * @version 0.1
 * @date 2024-09-17
 *
 * @copyright Copyright (c) 2024
 *
 */
#ifndef __zoPhysicsSystem2dImpl_h__
#define __zoPhysicsSystem2dImpl_h__

#include <zero_physics/physics_system_2d.hpp>
#include <zero_physics/memory.hpp>
#include "physics_object_2d_impl.hpp"
#include "collision_system_2d_impl.hpp"

namespace zo {
class PhysicsSystem2dImpl : public PhysicsSystem2d {
  public:
    PhysicsSystem2dImpl(size_t max_num_objects, float iterations, BroadPhaseType broad_phase_type);
    virtual ~PhysicsSystem2dImpl() = default;

    void update(float dt) override;

    void setGravity(const glm::vec2 &gravity) override { _gravity = gravity; }
    glm::vec2 gravity() const override { return _gravity; }

    force_handle_2d_t addGlobalForce(const glm::vec2 &f) override;
    void              removeGlobalForce(force_handle_2d_t id) override;
    std::optional<glm::vec2>
    getGlobalForce(force_handle_2d_t id) const override;
    const ComponentStore<glm::vec2> &globalForces() const override;

    std::unique_ptr<PhysicsObject2d> createPhysicsObject() override;
    bool isPhysicsHandleValid(phy_obj_handle_2d_t hndl) const override;
    void destroyPhysicsObject(phy_obj_handle_2d_t hndl) override;
    void destroyPhysicsObject(std::unique_ptr<PhysicsObject2d> &obj) override;

    CollisionSystem2d &collisionSystem() override { return *_collision_system; }

  public: // Implementation specific
    /// @brief Get the last time step
    /// @return float the last time step
    float lastTimeStep() const { return _last_time_step; }

    /// @brief Get the physics object store
    /// @return The physics object store
    ComponentStore<PhysicsObject2dImpl::Data> &physicsObjects() {
        return _physics_objects;
    }

    /// @brief Get the physics object data from the handle
    /// @param hndl the handle
    /// @return PhysicsObject2dImpl::Data& the physics object data
    PhysicsObject2dImpl::Data &physicsObjectData(phy_obj_handle_2d_t hndl) {
        return _physics_objects.get(hndl)->get();
    }

    /// @brief Map a collider to a physics object.
    /// @param c_hndl
    /// @param p_hndl
    void mapColliderToPhysicsObject(collider_handle_2d_t c_hndl,
                                    phy_obj_handle_2d_t  p_hndl) {
        _collider_map[c_hndl.handle] = p_hndl;
    }

    /// @brief Unmap a collider from a physics object.
    /// @param c_hndl
    void unmapColliderFromPhysicsObject(collider_handle_2d_t c_hndl) {
        if (_collider_map.contains(c_hndl.handle) == false) {
            return;
        }
        _collider_map.erase(c_hndl.handle);
    }

    /// @brief Get the physics object from the collider handle
    /// @param c_hndl
    /// @return phy_obj_handle_2d_t
    std::optional<phy_obj_handle_2d_t>
    physicsObjectMappedToCollider(collider_handle_2d_t c_hndl) {
        if (_collider_map.contains(c_hndl.handle) == false) {
            return std::nullopt;
        }
        if (isPhysicsHandleValid(_collider_map[c_hndl.handle]) == false) {
            _collider_map.erase(c_hndl.handle);
            return std::nullopt;
        }
        return _collider_map[c_hndl.handle];
    }

  private:
    float                                     _last_time_step = 1 / 60.0f;
    int                                       _iterations = 1;

    glm::vec2                                 _gravity = {0, 0};

    ComponentStore<glm::vec2>                 _global_forces;
    ComponentStore<PhysicsObject2dImpl::Data> _physics_objects;

    std::shared_ptr<CollisionSystem2dImpl> _collision_system;

    // map collider to physics object
    std::unordered_map<uint32_t, phy_obj_handle_2d_t> _collider_map;
};

} // namespace zo

#endif // __zoPhysicsSystem2dImpl_h__
