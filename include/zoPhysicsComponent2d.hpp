/**
 * @file zoPhysicsComponent2d.hpp
 * @author Micah Pearlman (micahpearlman@gmail.com)
 * @brief
 * @version 0.1
 * @date 2024-09-09
 *
 * @copyright Copyright (c) 2024
 *
 */
#ifndef __zoPhysicsComponent2d_h__
#define __zoPhysicsComponent2d_h__
#include "zoPhysicsTypes.hpp"
#include <glm/glm.hpp>
namespace zo {
class PhysicsComponent2d {

  public:
    virtual ~PhysicsComponent2d() = default;
    virtual void                setMass(float mass) = 0;
    virtual float               mass() const = 0;
    virtual void                setPosition(const glm::vec2 &p) = 0;
    virtual glm::vec2           position() const = 0;
    virtual void                setVelocity(const glm::vec2 &v) = 0;
    virtual glm::vec2           velocity() const = 0;
    virtual void                setAcceleration(const glm::vec2 &a) = 0;
    virtual glm::vec2           acceleration() const = 0;
    virtual void                addForce(const glm::vec2 &f) = 0;
    virtual void                zeroForce() = 0;
    virtual bool                isValid() const = 0;
    virtual void                setStatic(bool isStatic) = 0;
    virtual bool                isStatic() const = 0;
    virtual phy_obj_handle_2d_t handle() const = 0;
};
} // namespace zo
#endif // __zoPhysicsComponent2d_h__