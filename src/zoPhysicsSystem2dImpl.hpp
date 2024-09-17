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

#include <zoPhysicsSystem2d.hpp>
#include "zoPhysicsMemory.hpp"

namespace zo {
class PhysicsSystem2dImpl : public PhysicsSystem2d {
  public:
    /// @brief The data for a physics component
    struct alignas(std::max_align_t) VerletComponentData {
        glm::vec2 position = {0, 0};
        glm::vec2 prev_position = {0, 0};
        glm::vec2 acceleration = {0, 0};
        glm::vec2 force = {0, 0};

        float mass = 1;
    };

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

    ComponentStore<VerletComponentData> &physicsComponents() {
        return _physics_components;
    }

    VerletComponentData &physicsComponent(phy_obj_handle_2d_t hndl) {
        return _physics_components.get(hndl)->get();
    }

  private:
    float                               _last_time_step = 1 / 60.0f;
    int                                 _iterations = 1;
    ComponentStore<glm::vec2>           _global_forces;
    ComponentStore<VerletComponentData> _physics_components;
};

} // namespace zo

#endif // __zoPhysicsSystem2dImpl_h__
