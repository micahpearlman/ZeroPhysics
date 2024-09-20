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
    PhysicsSystem2dImpl(size_t max_num_objects, float iterations);
    virtual ~PhysicsSystem2dImpl() = default;

    void              update(float dt) override;
    force_handle_2d_t addGlobalForce(const glm::vec2 &f) override;
    void              removeGlobalForce(force_handle_2d_t id) override;
    std::optional<glm::vec2>
    getGlobalForce(force_handle_2d_t id) const override;
    const ComponentStore<glm::vec2> &globalForces() const override;
    phy_obj_handle_2d_t              createPhysicsObject() override;
    void destroyPhysicsObject(phy_obj_handle_2d_t hndl) override;
    bool isPhysicsObjectValid(phy_obj_handle_2d_t hndl) const override;


  public:
    float lastTimeStep() const { return _last_time_step; }

    ComponentStore<PhysicsObject2dImpl::Data> &
    physicsObjects() {
        return _physics_objects;
    }

    PhysicsObject2dImpl::Data &
    physicsComponent(phy_obj_handle_2d_t hndl) {
        return _physics_objects.get(hndl)->get();
    }

  private:
    float                     _last_time_step = 1 / 60.0f;
    int                       _iterations = 1;
    ComponentStore<glm::vec2> _global_forces;
    ComponentStore<PhysicsObject2dImpl::Data>
        _physics_objects;

    std::shared_ptr<CollisionSystem2dImpl> _collision_system;
};

} // namespace zo

#endif // __zoPhysicsSystem2dImpl_h__
