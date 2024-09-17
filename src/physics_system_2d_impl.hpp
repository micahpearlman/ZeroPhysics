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
#include "physics_component_2d_impl.hpp"
#include "collision_system_2d_impl.hpp"

namespace zo {
class PhysicsSystem2dImpl : public PhysicsSystem2d {
  public:
    PhysicsSystem2dImpl(float iterations);
    virtual ~PhysicsSystem2dImpl() = default;

    void              update(float dt) override;
    force_handle_2d_t addGlobalForce(const glm::vec2 &f) override;
    void              removeGlobalForce(force_handle_2d_t id) override;
    std::optional<glm::vec2>
    getGlobalForce(force_handle_2d_t id) const override;
    const ComponentStore<glm::vec2> &globalForces() const override;
    phy_obj_handle_2d_t              createPhysicsComponent() override;
    void destroyPhysicsComponent(phy_obj_handle_2d_t hndl) override;
    bool isPhysicsComponentValid(phy_obj_handle_2d_t hndl) const override;

    std::unique_ptr<PhysicsComponent2d>
    getPhysicsComponentView(phy_obj_handle_2d_t hndl) override;

  public:
    float lastTimeStep() const { return _last_time_step; }

    ComponentStore<PhysicsComponent2dImpl::Data> &
    physicsComponents() {
        return _physics_components;
    }

    PhysicsComponent2dImpl::Data &
    physicsComponent(phy_obj_handle_2d_t hndl) {
        return _physics_components.get(hndl)->get();
    }

  private:
    float                     _last_time_step = 1 / 60.0f;
    int                       _iterations = 1;
    ComponentStore<glm::vec2> _global_forces;
    ComponentStore<PhysicsComponent2dImpl::Data>
        _physics_components;

    std::shared_ptr<CollisionSystem2dImpl> _collision_system;
};

} // namespace zo

#endif // __zoPhysicsSystem2dImpl_h__
