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

std::shared_ptr<PhysicsSystem2d>
PhysicsSystem2d::create(size_t max_num_objects, int iterations,
                        BroadPhaseType broad_phase_type) {
    PhysicsSystem2dImpl *impl =
        new PhysicsSystem2dImpl(max_num_objects, iterations, broad_phase_type);
    return std::shared_ptr<PhysicsSystem2d>(impl);
}

PhysicsSystem2dImpl::PhysicsSystem2dImpl(size_t max_number_object,
                                         float  iterations, BroadPhaseType broad_phase_type)
    : _iterations(iterations) {
    // create the collision system
    // HARDWIRED: the collision system colliders is 3 times the number of
    // physics objects
    std::shared_ptr<CollisionSystem2d> collision_sys =
        CollisionSystem2d::create(max_number_object * 3, broad_phase_type);
    _collision_system =
        std::static_pointer_cast<CollisionSystem2dImpl>(collision_sys);
}

void PhysicsSystem2dImpl::update(float dt) {

    // sum all global forces
    glm::vec2 global_force_sum(0);
    for (const glm::vec2 &f : _global_forces) {
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

                    // update the aabb
                    collider.aabb.mn = collider.circle.center -
                                       glm::vec2{collider.circle.radius,
                                                 collider.circle.radius};
                    collider.aabb.mx = collider.circle.center +
                                        glm::vec2{collider.circle.radius,
                                                     collider.circle.radius};
                } else if (data.collider.type == uint8_t(ColliderType::LINE)) {
                    auto &collider =
                        _collision_system
                            ->getColliderData<LineCollider2dImpl::Data>(
                                data.collider);
                    if (data.collider_vertex == 0) {
                        collider.line.line.start = data.position;
                    } else {
                        collider.line.line.end = data.position;
                    }

                    // update the aabb
                    const glm::vec2 thickness = glm::vec2{collider.line.radius, collider.line.radius};
                    collider.aabb.mn = glm::min(collider.line.line.start-thickness, collider.line.line.end+thickness);
                    collider.aabb.mx = glm::max(collider.line.line.start-thickness, collider.line.line.end+thickness);
                    
                }
            }
        }
    }

    // update the collision system
    _collision_system->generateCollisionPairs();

    // resolve collisions
    for (const CollisionPair &pair : _collision_system->collisionPairs()) {
        auto phy_obj_a = physicsObjectMappedToCollider(pair.a);
        auto phy_obj_b = physicsObjectMappedToCollider(pair.b);
        if (phy_obj_a.has_value() == false && phy_obj_b.has_value() == false) {
            continue;
        }
        float     total_mass = 0;
        glm::vec2 velocity_a(0);
        glm::vec2 velocity_b(0);

        if (phy_obj_a.has_value()) {
            const PhysicsObject2dImpl::Data &data =
                physicsObjectData(phy_obj_a.value());
            if (data.mass > 0) {
                total_mass += data.mass;
                velocity_a = data.prev_position - data.position;
            }
        }
        if (phy_obj_b.has_value()) {
            const PhysicsObject2dImpl::Data &data =
                physicsObjectData(phy_obj_b.value());
            if (data.mass > 0) {
                total_mass += data.mass;
                velocity_b = data.prev_position - data.position;
            }
        }

        // if both objects are static then skip
        if (total_mass <= 0) {
            continue;
        }

        const Collider2dImpl::Data &col_data_a =
            _collision_system->getBaseColliderData(pair.a);
        const Collider2dImpl::Data &col_data_b =
            _collision_system->getBaseColliderData(pair.b);

        // calculate the relative velocities along the collision normal
        float rel_velo_along_norm =
            glm::dot(velocity_a - velocity_b, pair.contact.normal);

        // if not already separating then resolve the collision by moving apart
        // with an impulse.
        glm::vec2 impulse_vector = glm::vec2(0);
        if (rel_velo_along_norm > 0) {

            // calculate the average of the two restitution values
            float restitution =
                0.5f * (col_data_a.restitution + col_data_b.restitution);

            // calculate impulse vector
            glm::vec2 rel_velo = velocity_a - velocity_b;
            impulse_vector = glm::reflect(rel_velo, pair.contact.normal);
            impulse_vector *= restitution;
        }

        // move apart
        if (phy_obj_a.has_value()) {
            PhysicsObject2dImpl::Data &data =
                physicsObjectData(phy_obj_a.value());
            if (data.mass > 0) {
                // move apart by collision normal and penetration depth
                data.position += pair.contact.normal * pair.contact.penetration;

                // adjust previous position based on impulse and mass
                data.prev_position =
                    data.position + (impulse_vector * (data.mass / total_mass));

                // data.prev_position = data.position;  // DEBUG
            }
        }

        if (phy_obj_b.has_value()) {
            PhysicsObject2dImpl::Data &data =
                physicsObjectData(phy_obj_b.value());

            if (data.mass > 0) {
                // move apart by collision normal and penetration depth
                // (opposite direction)
                data.position +=
                    pair.contact.normal * -pair.contact.penetration;

                // adjust previous position based on impulse and mass (opposite
                // direction)
                data.prev_position =
                    data.position - (impulse_vector * (data.mass / total_mass));
                // data.prev_position = data.position; // DEBUG
            }
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

void PhysicsSystem2dImpl::destroyPhysicsObject(
    std::unique_ptr<PhysicsObject2d> &obj) {
    auto hndl = obj->handle();
    destroyPhysicsObject(hndl);
}

bool PhysicsSystem2dImpl::isPhysicsHandleValid(phy_obj_handle_2d_t hndl) const {
    return _physics_objects.get(hndl).has_value();
}

} // namespace zo