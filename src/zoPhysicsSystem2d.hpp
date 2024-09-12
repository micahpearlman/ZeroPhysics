/**
 * @file zoPhysicsSystem2d.hpp
 * @author Micah Pearlman (micahpearlman@gmail.com)
 * @brief Public phyiscs API
 * @version 0.1
 * @date 2024-09-09
 *
 * @copyright Copyright (c) 2024
 *
 */
#ifndef __zoPhysicsSystem2d_h__
#define __zoPhysicsSystem2d_h__
#include "zoPhysicsComponent2d.hpp"
#include "zoPhysicsMemory.hpp"
#include "zoPhysicsTypes.hpp"
#include <optional>
#include <memory>
#include <glm/glm.hpp>
namespace zo {
class PhysicsSystem2d {

  public:
    virtual ~PhysicsSystem2d() = default;
    /// @brief create a physics system
    /// @param iterations number of iterations to perform per update time step
    /// @return the physics system
    static std::shared_ptr<PhysicsSystem2d> create(int iterations);

    virtual void update(float dt) = 0;

    /// @brief  Add a global force to the physics system.
    //         This force will be applied to all physics components.
    /// @param force
    /// @return
    virtual force_handle_t addGlobalForce(const glm::vec2 &force) = 0;

    /// @brief  Remove a global force from the physics system.
    /// @param hndl
    virtual void removeGlobalForce(force_handle_t hndl) = 0;

    /// @brief  Get a global force from the physics system.
    /// @param hndl
    /// @return optional<glm::vec2>
    virtual std::optional<glm::vec2>
    getGlobalForce(force_handle_t hndl) const = 0;

    /// @brief  Get the global forces in the physics system.
    /// @return const ComponentStore<glm::vec2>&
    virtual const ComponentStore<glm::vec2> &globalForces() const = 0;

    /// @brief Create a physics object.
    /// @return a handle to the physics object
    virtual phy_obj_handle_t   createPhysicsComponent() = 0;

    /// @brief Destroy a physics object.
    /// @param hndl The handle to the physics object to destroy.
    virtual void               destroyPhysicsComponent(phy_obj_handle_t hndl) = 0;

    /// @brief Get the view of the internal physics component for the given physics object handle.
    /// @param hndl The handle to the physics object.
    /// @return PhysicsComponent2d a view of the physics component.
    virtual std::unique_ptr<PhysicsComponent2d> getPhysicsComponentView(phy_obj_handle_t hndl) = 0;

    virtual bool isPhysicsComponentValid(phy_obj_handle_t hndl) const = 0;



};
} // namespace zo

#endif // __zoPhysicsSystem2d_h__
