/**
 * @file physics_object_2d_impl.hpp
 * @author Micah Pearlman (micahpearlman@gmail.com)
 * @brief
 * @version 0.1
 * @date 2024-09-17
 *
 * @copyright Copyright (c) 2024
 *
 */
#ifndef __zo_physics_object_2d_impl_h__
#define __zo_physics_object_2d_impl_h__

#include <zero_physics/physics_object_2d.hpp>
#include "types_impl.hpp"

namespace zo {

// forward declare
class PhysicsSystem2dImpl;

/// @brief A view of the physics object data
class PhysicsObject2dImpl : public PhysicsObject2d {

  public:
    /// @brief The data for a physics object
    struct alignas(std::max_align_t) Data {
        glm::vec2            position = {0, 0};
        glm::vec2            prev_position = {0, 0};
        glm::vec2            acceleration = {0, 0};
        glm::vec2            force = {0, 0};
        collider_handle_2d_t collider = {uint16_t(ColliderType::MAX),
                                         0xfffffff};
        uint32_t             collider_vertex = 0;
        float                mass = 1;
    };

  public:
    PhysicsObject2dImpl(PhysicsSystem2dImpl &sys, phy_obj_handle_2d_t hndl);
    virtual ~PhysicsObject2dImpl();

    void      setMass(float mass) override;
    float     mass() const override;
    void      setPosition(const glm::vec2 &p) override;
    glm::vec2 position() const override;
    void      setVelocity(const glm::vec2 &v) override;
    glm::vec2 velocity() const override;
    void      setAcceleration(const glm::vec2 &a) override;
    glm::vec2 acceleration() const override;
    void      addForce(const glm::vec2 &f) override;
    void      zeroForce() override;
    bool      isValid() const override;
    void      setStatic(bool is_static) override;
    bool      isStatic() const override;
    void      setCollider(collider_handle_2d_t hndl, uint32_t vertex) override;
    void      setCollider(Collider2d &collider, uint32_t vertex) override;

    phy_obj_handle_2d_t handle() const override;

    /// @brief Get the underlying physics object data
    /// @return PhysicsObject2dImpl::Data& the physics object data
    PhysicsObject2dImpl::Data &data();

    /// @brief Get the underlying physics object data
    /// @return const PhysicsObject2dImpl::Data& the physics object data
    const PhysicsObject2dImpl::Data &data() const;

    /// @brief Apply an impulse to the physics object
    /// @param impulse the impulse to apply
    /// @param time_step the time step
    static void applyImpulse(PhysicsObject2dImpl::Data &data,
                             const glm::vec2 &impulse, const float time_step);

  private:
    PhysicsSystem2dImpl &_sys;
    phy_obj_handle_2d_t  _hndl;
};

} // namespace zo

#endif // __zo_physics_object_2d_impl_h__
