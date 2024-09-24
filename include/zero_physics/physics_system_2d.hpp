/**
 * @file physics_system_2d.hpp
 * @author Micah Pearlman (micahpearlman@gmail.com)
 * @brief Public physics API
 * @version 0.1
 * @date 2024-09-09
 *
 * @copyright Copyright (c) 2024
 *
 */
#ifndef __zoPhysicsSystem2d_h__
#define __zoPhysicsSystem2d_h__
#include <zero_physics/physics_object_2d.hpp>
#include <zero_physics/memory.hpp>
#include <zero_physics/types.hpp>
#include <zero_physics/collision_system_2d.hpp>
#include <optional>
#include <memory>
#include <glm/glm.hpp>
namespace zo {
class PhysicsSystem2d {

  public:
    virtual ~PhysicsSystem2d() = default;
    /// @brief create a physics system
    /// @param max_num_objects maximum number of physics objects
    /// @param iterations number of iterations to perform per update time step
    /// @return the physics system
    static std::shared_ptr<PhysicsSystem2d> create(size_t max_num_objects,
                                                   int    iterations);

    /// @brief  Update the physics system.
    /// @param dt The time step to update the physics system by.
    virtual void update(float dt) = 0;

    /// @brief  Add a global force to the physics system.
    //         This force will be applied to all physics objects.
    /// @param force
    /// @return
    virtual force_handle_2d_t addGlobalForce(const glm::vec2 &force) = 0;

    /// @brief  Remove a global force from the physics system.
    /// @param hndl
    virtual void removeGlobalForce(force_handle_2d_t hndl) = 0;

    /// @brief  Get a global force from the physics system.
    /// @param hndl
    /// @return optional<glm::vec2>
    virtual std::optional<glm::vec2>
    getGlobalForce(force_handle_2d_t hndl) const = 0;

    /// @brief  Get the global forces in the physics system.
    /// @return const ComponentStore<glm::vec2>&
    virtual const ComponentStore<glm::vec2> &globalForces() const = 0;

    /// @brief Create a physics object.
    /// @return a handle to the physics object
    virtual std::unique_ptr<PhysicsObject2d> createPhysicsObject() = 0;

    /// @brief checks if the physics handle is valid
    /// @param hndl 
    /// @return true if the handle is valid
    virtual bool isPhysicsHandleValid(phy_obj_handle_2d_t hndl) const = 0;

    /// @brief Get the collision system
    /// @return 
    virtual CollisionSystem2d &collisionSystem() = 0;

};
} // namespace zo

#endif // __zoPhysicsSystem2d_h__
