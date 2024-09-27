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
#include "physics_object_2d_impl.hpp"
#include <iostream>

namespace zo {

std::shared_ptr<PhysicsSystem2d> PhysicsSystem2d::create(size_t max_num_objects,
                                                         int    iterations) {
    PhysicsSystem2dImpl *impl =
        new PhysicsSystem2dImpl(max_num_objects, iterations);
    return std::shared_ptr<PhysicsSystem2d>(impl);
}


PhysicsSystem2dImpl::PhysicsSystem2dImpl(size_t max_number_object,
                                         float  iterations)
    : _iterations(iterations) {
    // create the collision system
    // HARDWIRED: the collision system colliders is 3 times the number of
    // physics objects
    std::shared_ptr<CollisionSystem2d> collision_sys =
        CollisionSystem2d::create(max_number_object * 3);
    _collision_system =
        std::static_pointer_cast<CollisionSystem2dImpl>(collision_sys);
}

void PhysicsSystem2dImpl::update(float dt) {

    // sum all global forces
    glm::vec2 global_force_sum(0);
    for (auto &f : _global_forces) {
        global_force_sum += f;
    }
    for (int i = 0; i < _iterations; i++) {
        float iter_dt = dt / float(_iterations);
        for (PhysicsObject2dImpl::Data &data : _physics_objects) {
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

            // update the collider position
            if (data.collider.type != uint16_t(ColliderType::MAX)) {

                if (data.collider.type == uint8_t(ColliderType::CIRCLE)) {
                    auto &collider =
                        _collision_system
                            ->getColliderData<CircleCollider2dImpl::Data>(
                                data.collider);
                    collider.circle.center = data.position;
                } else if (data.collider.type == uint8_t(ColliderType::LINE)) {
                    auto &collider =
                        _collision_system
                            ->getColliderData<LineCollider2dImpl::Data>(
                                data.collider);
                    if (data.collider_vertex == 0) {
                        collider.line.start = data.position;
                    } else {
                        collider.line.end = data.position;
                    }
                }
            }
        }
    }

    // update the collision system
    _collision_system->generateCollisionPairs();

    // resolve collisions
    for (const CollisionPair &pair : _collision_system->collisionPairs()) {
        // auto &a = _physics_objects.get(pair.a)->get();
        // auto &b = _physics_objects.get(pair.b)->get();
        // if (a.mass <= 0 && b.mass <= 0) {
        //     continue;
        // }
        // if (a.mass <= 0) {
        //     // resolve b
        //     glm::vec2 normal = pair.normal;
        //     glm::vec2 contact = pair.contact;
        //     float     penetration = pair.penetration;
        //     b.position += normal * penetration;
        //     glm::vec2 relative_velocity = b.velocity;
        //     float     separating_velocity = glm::dot(relative_velocity, normal);
        //     if (separating_velocity < 0) {
        //         b.velocity -= separating_velocity * normal;
        //     }
        // } else if (b.mass <= 0) {
        //     // resolve a
        //     glm::vec2 normal = pair.normal;
        //     glm::vec2 contact = pair.contact;
        //     float     penetration = pair.penetration;
        //     a.position -= normal * penetration;
        //     glm::vec2 relative_velocity = a.velocity;
        //     float     separating_velocity = glm::dot(relative_velocity, normal);
        //     if (separating_velocity < 0) {
        //         a.velocity -= separating_velocity * normal;
        //     }
        // } else {
        //     // resolve both
        //     glm::vec2 normal = pair.normal;
        //     glm::vec2 contact = pair.contact;
        //     float     penetration = pair.penetration;
        //     a.position -= normal * (penetration / 2.0f);
        //     b.position += normal * (penetration / 2.0f);
        //     glm::vec2 relative_velocity = a.velocity - b.velocity;
        //     float     separating_velocity = glm::dot(relative_velocity, normal);
        //     if (separating_velocity < 0) {
        //         a.velocity -= separating_velocity * normal;
        //         b.velocity += separating_velocity * normal;
        //     }
        // }
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

std::unique_ptr<PhysicsObject2d> PhysicsSystem2dImpl::createPhysicsObject() {
    PhysicsObject2dImpl::Data data;
    phy_obj_handle_2d_t       hndl = _physics_objects.add(data);
    return std::make_unique<PhysicsObject2dImpl>(*this, hndl);
}

void PhysicsSystem2dImpl::destroyPhysicsObject(phy_obj_handle_2d_t hndl) {
    auto phy_data = _physics_objects.get(hndl);
    if (phy_data.has_value() == false) {
        return;
    }
    PhysicsObject2dImpl::Data *data = &phy_data->get();
    if (data->collider.index != 0xfffffff) {
        _collision_system->destroyCollider(data->collider);
    }
    _physics_objects.remove(hndl);
}

void PhysicsSystem2dImpl::destroyPhysicsObject(std::unique_ptr<PhysicsObject2d> &obj) {
    auto hndl = obj->handle();
    destroyPhysicsObject(hndl);
}



bool PhysicsSystem2dImpl::isPhysicsHandleValid(phy_obj_handle_2d_t hndl) const {
    return _physics_objects.get(hndl).has_value();
}

} // namespace zo