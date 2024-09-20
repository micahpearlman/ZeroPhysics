/**
 * @file physics_object_2d.hpp
 * @author Micah Pearlman (micahpearlman@gmail.com)
 * @brief
 * @version 0.1
 * @date 2024-09-09
 *
 * @copyright Copyright (c) 2024
 *
 */
#ifndef __zo_physics_object_h__
#define __zo_physics_object_h__
#include <zero_physics/types.hpp>
#include <glm/glm.hpp>
#include <memory>
namespace zo {
class PhysicsSystem2d;
/// @brief Interface for a 2d physics object. This is a view into the physics
/// object data which is managed by the physics system.
class PhysicsObject2d {

  public:
    virtual ~PhysicsObject2d() = default;
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
#endif // __zo_physics_object_h__