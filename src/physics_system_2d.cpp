/**
 * @file physics_system_2d.cpp
 * @author Micah Pearlman (micahpearlman@gmail.com)
 * @brief
 * @version 0.1
 * @date 2024-09-09
 *
 * @copyright Copyright (c) 2024
 *
 */
#include "physics_system_2d_impl.hpp"
#include "physics_component_2d_impl.hpp"

namespace zo {

PhysicsSystem2dImpl::PhysicsSystem2dImpl(float iterations)
    : _iterations(iterations) {}

void PhysicsSystem2dImpl::update(float dt) {
    // sum all global forces
    glm::vec2 global_force_sum(0);
    for (auto &f : _global_forces) {
        global_force_sum += f;
    }
    for (int i = 0; i < _iterations; i++) {
        float iter_dt = dt / float(_iterations);
        for (PhysicsComponent2dImpl::Data &data : _physics_components) {
            if (data.mass <= 0) { // static object
                continue;
            }
            glm::vec2 force = data.force;
            force += global_force_sum;
            glm::vec2 acceleration = force / data.mass;
            glm::vec2 new_position = data.position +
                                     (data.position - data.prev_position) +
                                     acceleration * iter_dt * iter_dt;
            data.prev_position = data.position;
            data.position = new_position;
            data.acceleration = acceleration;
            data.force = glm::vec2(0);
        }
    }
}
force_handle_2d_t PhysicsSystem2dImpl::addGlobalForce(const glm::vec2 &f) {
    return _global_forces.add(f);
}
void PhysicsSystem2dImpl::removeGlobalForce(force_handle_2d_t id) {
    _global_forces.remove(id);
}
std::optional<glm::vec2>
PhysicsSystem2dImpl::getGlobalForce(force_handle_2d_t id) const {
    return _global_forces.get(id);
}

const ComponentStore<glm::vec2> &PhysicsSystem2dImpl::globalForces() const {
    return _global_forces;
}

phy_obj_handle_2d_t PhysicsSystem2dImpl::createPhysicsComponent() {
    PhysicsComponent2dImpl::Data data;
    return _physics_components.add(data);
}

void PhysicsSystem2dImpl::destroyPhysicsComponent(phy_obj_handle_2d_t hndl) {
    _physics_components.remove(hndl);
}


bool PhysicsSystem2dImpl::isPhysicsComponentValid(
    phy_obj_handle_2d_t hndl) const {
    return _physics_components.get(hndl) != std::nullopt;
}

std::shared_ptr<PhysicsSystem2d> PhysicsSystem2d::create(size_t max_num_objects, int iterations) {
    PhysicsSystem2dImpl *impl = new PhysicsSystem2dImpl(iterations);
    return std::shared_ptr<PhysicsSystem2d>(impl);
}

} // namespace zo