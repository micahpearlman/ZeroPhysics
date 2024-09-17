/**
 * @file zoPysicsComponent2dImpl.hpp
 * @author Micah Pearlman (micahpearlman@gmail.com)
 * @brief
 * @version 0.1
 * @date 2024-09-17
 *
 * @copyright Copyright (c) 2024
 *
 */
#ifndef __zoPhysicsComponent2dImpl_h__
#define __zoPhysicsComponent2dImpl_h__

#include "zoPhysicsComponent2d.hpp"
#include "zoPhysicsSystem2dImpl.hpp"

namespace zo {
/// @brief A view of the VerletComponentData
class PhysicsComponent2dView : public PhysicsComponent2d {
  public:
    PhysicsComponent2dView(PhysicsSystem2dImpl &sys, phy_obj_handle_2d_t hndl);

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

    PhysicsSystem2dImpl::VerletComponentData       &data();
    const PhysicsSystem2dImpl::VerletComponentData &data() const;

  private:
    PhysicsSystem2dImpl &_sys;
    phy_obj_handle_2d_t  _hndl;
};

} // namespace zo

#endif // __zoPhysicsComponent2dImpl_h__
