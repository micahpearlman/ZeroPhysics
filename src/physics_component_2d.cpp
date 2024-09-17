/**
 * @file physics_component_2d.cpp
 * @author Micah Pearlman (micahpearlman@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2024-09-17
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include "physics_component_2d_impl.hpp"
#include "physics_system_2d_impl.hpp"

namespace zo {
PhysicsComponent2dImpl::PhysicsComponent2dImpl(PhysicsSystem2dImpl &sys,
                                               phy_obj_handle_2d_t  hndl)
    : _sys(sys), _hndl(hndl) {}

void  PhysicsComponent2dImpl::setMass(float mass) { data().mass = mass; }
float PhysicsComponent2dImpl::mass() const { return data().mass; }
void  PhysicsComponent2dImpl::setPosition(const glm::vec2 &p) {
    data().position = p;
    data().prev_position = p;
}
glm::vec2 PhysicsComponent2dImpl::position() const { return data().position; }
void      PhysicsComponent2dImpl::setVelocity(const glm::vec2 &v) {
    // because velocity isn't explicit in verlet we will "trick"
    // velocity by setting the previous position to the current
    // position
    // minus the velocity
    data().prev_position = data().position - (v * _sys.lastTimeStep());
}
glm::vec2 PhysicsComponent2dImpl::velocity() const {
    // remember that velocity isn't explicit in verlet so we will
    // calculate it
    return data().prev_position - data().position;
}
void PhysicsComponent2dImpl::setAcceleration(const glm::vec2 &a) {
    data().acceleration = a;
}
glm::vec2 PhysicsComponent2dImpl::acceleration() const {
    return data().acceleration;
}
void PhysicsComponent2dImpl::addForce(const glm::vec2 &f) { data().force += f; }
void PhysicsComponent2dImpl::zeroForce() { data().force = glm::vec2(0); }
bool PhysicsComponent2dImpl::isValid() const {
    return _sys.isPhysicsComponentValid(_hndl);
}
void PhysicsComponent2dImpl::setStatic(bool is_static) {
    setMass(is_static ? -1.0f : 1.0f);
}
bool PhysicsComponent2dImpl::isStatic() const { return data().mass <= 0.0f; }
phy_obj_handle_2d_t PhysicsComponent2dImpl::handle() const { return _hndl; }

PhysicsComponent2dImpl::Data &PhysicsComponent2dImpl::data() {
    return _sys.physicsComponent(_hndl);
}

const PhysicsComponent2dImpl::Data &
PhysicsComponent2dImpl::data() const {
    return _sys.physicsComponent(_hndl);
}

} // namespace zo