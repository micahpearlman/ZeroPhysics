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

    /// @brief  Set the mass of the physics object.
    /// @param mass 
    virtual void                setMass(float mass) = 0;

    /// @brief  Get the mass of the physics object.
    /// @return float
    virtual float               mass() const = 0;

    /// @brief  Set the position of the physics object.
    /// @param p
    virtual void                setPosition(const glm::vec2 &p) = 0;

    /// @brief  Get the position of the physics object.
    /// @return glm::vec2
    virtual glm::vec2           position() const = 0;

    /// @brief  Set the velocity of the physics object.
    /// @param v
    virtual void                setVelocity(const glm::vec2 &v) = 0;

    /// @brief  Get the velocity of the physics object.
    /// @return glm::vec2
    virtual glm::vec2           velocity() const = 0;

    /// @brief  Set the acceleration of the physics object.
    /// @param a
    virtual void                setAcceleration(const glm::vec2 &a) = 0;

    /// @brief  Get the acceleration of the physics object.
    /// @return glm::vec2
    virtual glm::vec2           acceleration() const = 0;

    /// @brief  Add a force to the physics object.
    /// @param f
    virtual void                addForce(const glm::vec2 &f) = 0;

    /// @brief  zero out the forces on the physics object.
    virtual void                zeroForce() = 0;

    /// @brief Check if the physics object is valid.
    /// @return bool true if the physics object is valid
    virtual bool                isValid() const = 0;

    /// @brief Set the physics object to be static or non static.
    /// @param isStatic true if the physics object is to be static otherwise false.
    virtual void                setStatic(bool isStatic) = 0;

    /// @brief Check if the physics object is static.
    /// @return bool true if the physics object is static
    virtual bool                isStatic() const = 0;

    /// @brief  Get the handle to the physics object.
    /// @return phy_obj_handle_2d_t handle to the physics object
    virtual phy_obj_handle_2d_t handle() const = 0;

};
} // namespace zo
#endif // __zo_physics_object_h__