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
#include <memory>
#include <glm/glm.hpp>
namespace zo {
class PhysicsSystem2d {
  public:
    using force_handle_t = uint32_t;
    using component_handle_t = uint32_t;

  public:
    virtual ~PhysicsSystem2d() = default;
    static std::shared_ptr<PhysicsSystem2d> create();

    virtual void           update(float dt) = 0;
    virtual force_handle_t addGlobalForce(const glm::vec2 &f) = 0;
    virtual void           removeGlobalForce(force_handle_t id) = 0;
    virtual std::optional<glm::vec2>
    getGlobalForce(force_handle_t id) const = 0;
    virtual const ComponentRegistry<glm::vec2> &globalForces() const = 0;

    // virtual std::shared_ptr<PhysicsComponent2d> createPhysicsComponent() = 0;

    // void addRigidBody( RigidBody* rb );
    // void removeRigidBody( RigidBody* rb );
    // void addConstraint( Constraint* c );
    // void removeConstraint( Constraint* c );
    // void addForceGenerator( ForceGenerator* fg );
    // void removeForceGenerator( ForceGenerator* fg );
    // void addCollisionPrimitive( CollisionPrimitive* cp );
    // void removeCollisionPrimitive( CollisionPrimitive* cp );
    // void addCollisionPair( CollisionPair* cp );
    // void removeCollisionPair( CollisionPair* cp );
    // void addCollisionResolver( CollisionResolver* cr );
    // void removeCollisionResolver( CollisionResolver* cr );
    // void addContact( Contact* c );
    // void removeContact( Contact* c );
    // void addContactResolver( ContactResolver* cr );
    // void removeContactResolver( ContactResolver* cr );
    // void addJoint( Joint* j );
    // void removeJoint( Joint* j );
    // void addJointResolver( JointResolver* jr );
    // void removeJointResolver( JointResolver* jr );
    // void addMaterial( Material* m );
    // void removeMaterial( Material* m );
    // void addMaterialResolver( MaterialResolver* mr );
    // void removeMaterialResolver( MaterialResolver* mr );
    // void addMaterialPair( MaterialPair* mp );
    // void removeMaterialPair( MaterialPair* mp );
    // void addMaterialPairResolver( MaterialPairResolver* mpr );
    // void removeMaterialPairResolver( MaterialPairResolver* mpr );
    // void addMaterialPairContactResolver( MaterialPairContactResolver* mpcr );
    // void removeMaterialPairContactResolver( MaterialPairContactResolver* mpcr
    // ); void addMaterialPairContact( MaterialPairContact* mpc ); void
    // removeMaterialPairContact( MaterialPairContact* mpc ); void
    // addMaterialPairContactGenerator( MaterialPairContactGenerator* mpcg );
    // void removeMaterialPairContactGenerator( MaterialPairContactGenerator*
    // mpcg ); void addMaterialPairContactGeneratorResolver(
    // MaterialPairContactGeneratorResolver* mpcgr ); void
    // removeMaterialPairContactGeneratorResolver(
    // MaterialPairContactGeneratorResolver* mpcgr ); void
    // addMaterialPairContactGeneratorContactResolver(
    // MaterialPairContactGeneratorContactResolver* mpcgcr ); void
    // removeMaterialPairContactGeneratorContactResolver(
    // MaterialPairContactGeneratorContactResolver* mpcgcr ); void
    // addMaterialPair
};
} // namespace zo

#endif // __zoPhysicsSystem2d_h__
