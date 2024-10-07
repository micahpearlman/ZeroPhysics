/**
 * @file physics_object_2d.cpp
 * @author Micah Pearlman (micahpearlman@gmail.com)
 * @brief
 * @version 0.1
 * @date 2024-09-17
 *
 * @copyright Copyright (c) 2024
 *
 */
#include "physics_object_2d_impl.hpp"
#include "physics_system_2d_impl.hpp"

namespace zo {
PhysicsObject2dImpl::PhysicsObject2dImpl(PhysicsSystem2dImpl &sys,
                                         phy_obj_handle_2d_t  hndl)
    : _sys(sys), _hndl(hndl) {}

PhysicsObject2dImpl::~PhysicsObject2dImpl() {
    if (isValid()) {
        _sys.destroyPhysicsObject(_hndl);
    }
}

void PhysicsObject2dImpl::setMass(float mass) { data().mass = mass; }

float PhysicsObject2dImpl::mass() const { return data().mass; }

void PhysicsObject2dImpl::setPosition(const glm::vec2 &p) {
    data().position = p;
    data().prev_position = p;
}

glm::vec2 PhysicsObject2dImpl::position() const { return data().position; }

void PhysicsObject2dImpl::setVelocity(const glm::vec2 &v) {
    // because velocity isn't explicit in verlet we will "trick"
    // velocity by setting the previous position to the current
    // position
    // minus the velocity
    data().prev_position = data().position - (v * _sys.lastTimeStep());
}

glm::vec2 PhysicsObject2dImpl::velocity() const {
    // remember that velocity isn't explicit in verlet so we will
    // calculate it
    return data().position - data().prev_position;
}

void PhysicsObject2dImpl::setAcceleration(const glm::vec2 &a) {
    data().acceleration = a;
}

glm::vec2 PhysicsObject2dImpl::acceleration() const {
    return data().acceleration;
}

void PhysicsObject2dImpl::addForce(const glm::vec2 &f) { data().force += f; }

void PhysicsObject2dImpl::zeroForce() { data().force = glm::vec2(0); }

bool PhysicsObject2dImpl::isValid() const {
    return _sys.isPhysicsHandleValid(_hndl);
}

void PhysicsObject2dImpl::setStatic(bool is_static) {
    setMass(is_static ? -1.0f : 1.0f);
}

bool PhysicsObject2dImpl::isStatic() const { return data().mass <= 0.0f; }

void PhysicsObject2dImpl::setCollider(collider_handle_2d_t col_hndl, uint32_t vertex) {
    data().collider_vertex = vertex;
    data().collider = col_hndl;
    _sys.mapColliderToPhysicsObject(col_hndl, handle());
}

void PhysicsObject2dImpl::setCollider(Collider2d &collider, uint32_t vertex) {
    setCollider(collider.handle(), vertex);
}

phy_obj_handle_2d_t PhysicsObject2dImpl::handle() const { return _hndl; }

PhysicsObject2dImpl::Data &PhysicsObject2dImpl::data() {
    return _sys.physicsObjectData(_hndl);
}

const PhysicsObject2dImpl::Data &PhysicsObject2dImpl::data() const {
    return _sys.physicsObjectData(_hndl);
}

void PhysicsObject2dImpl::applyImpulse(PhysicsObject2dImpl::Data &data, const glm::vec2 &impulse, const float time_step) {
    // ignore static objects
    if (data.mass <= 0) {
        return;
    }
    // derive velocity.  This is a verlet integrator so velocity is
    // implicit so we will calculate it from the previous position to the
    // current position
    glm::vec2 velocity = (data.prev_position - data.position) / time_step;

    // calculate the impulse preserving momentum
    glm::vec2 delta_velocity = impulse / data.mass;

    // new velocity
    glm::vec2 new_velocity = velocity + delta_velocity;

    // update the previous position to reflect the new velocity
    // again this is a verlet integrator so we set the previous position
    data.prev_position = data.position - new_velocity * time_step;

}

} // namespace zo