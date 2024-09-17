#include "zoPysicsComponent2dImpl.hpp"

namespace zo {
PhysicsComponent2dView::PhysicsComponent2dView(PhysicsSystem2dImpl &sys,
                                               phy_obj_handle_2d_t  hndl)
    : _sys(sys), _hndl(hndl) {}

void  PhysicsComponent2dView::setMass(float mass) { data().mass = mass; }
float PhysicsComponent2dView::mass() const { return data().mass; }
void  PhysicsComponent2dView::setPosition(const glm::vec2 &p) {
    data().position = p;
    data().prev_position = p;
}
glm::vec2 PhysicsComponent2dView::position() const { return data().position; }
void      PhysicsComponent2dView::setVelocity(const glm::vec2 &v) {
    // because velocity isn't explicit in verlet we will "trick"
    // velocity by setting the previous position to the current
    // position
    // minus the velocity
    data().prev_position = data().position - (v * _sys.lastTimeStep());
}
glm::vec2 PhysicsComponent2dView::velocity() const {
    // remember that velocity isn't explicit in verlet so we will
    // calculate it
    return data().prev_position - data().position;
}
void PhysicsComponent2dView::setAcceleration(const glm::vec2 &a) {
    data().acceleration = a;
}
glm::vec2 PhysicsComponent2dView::acceleration() const {
    return data().acceleration;
}
void PhysicsComponent2dView::addForce(const glm::vec2 &f) { data().force += f; }
void PhysicsComponent2dView::zeroForce() { data().force = glm::vec2(0); }
bool PhysicsComponent2dView::isValid() const {
    return _sys.isPhysicsComponentValid(_hndl);
}
void PhysicsComponent2dView::setStatic(bool is_static) {
    setMass(is_static ? -1.0f : 1.0f);
}
bool PhysicsComponent2dView::isStatic() const { return data().mass <= 0.0f; }
phy_obj_handle_2d_t PhysicsComponent2dView::handle() const { return _hndl; }

PhysicsSystem2dImpl::VerletComponentData &PhysicsComponent2dView::data() {
    return _sys.physicsComponent(_hndl);
}

const PhysicsSystem2dImpl::VerletComponentData &
PhysicsComponent2dView::data() const {
    return _sys.physicsComponent(_hndl);
}

} // namespace zo