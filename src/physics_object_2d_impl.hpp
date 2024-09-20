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

namespace zo {

// forward declare
class PhysicsSystem2dImpl;

/// @brief A view of the physics object data
class PhysicsObject2dImpl : public PhysicsObject2d {
  public:
    /// @brief The data for a physics object
    struct alignas(std::max_align_t) Data {
        glm::vec2 position = {0, 0};
        glm::vec2 prev_position = {0, 0};
        glm::vec2 acceleration = {0, 0};
        glm::vec2 force = {0, 0};

        float mass = 1;
    };

  public:
    PhysicsObject2dImpl(PhysicsSystem2dImpl &sys, phy_obj_handle_2d_t hndl);

    void                setMass(float mass) override;
    float               mass() const override;
    void                setPosition(const glm::vec2 &p) override;
    glm::vec2           position() const override;
    void                setVelocity(const glm::vec2 &v) override;
    glm::vec2           velocity() const override;
    void                setAcceleration(const glm::vec2 &a) override;
    glm::vec2           acceleration() const override;
    void                addForce(const glm::vec2 &f) override;
    void                zeroForce() override;
    bool                isValid() const override;
    void                setStatic(bool is_static) override;
    bool                isStatic() const override;
    phy_obj_handle_2d_t handle() const override;

    PhysicsObject2dImpl::Data       &data();
    const PhysicsObject2dImpl::Data &data() const;

  private:
    PhysicsSystem2dImpl &_sys;
    phy_obj_handle_2d_t  _hndl;
};

} // namespace zo

#endif // __zo_physics_object_2d_impl_h__
