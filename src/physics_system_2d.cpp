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

PhysicsSystem2dImpl::PhysicsSystem2dImpl(size_t         max_number_object,
                                         float          iterations,
                                         BroadPhaseType broad_phase_type)
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
            glm::vec2 acceleration = (force / data.mass) + gravity();
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
                    const glm::vec2 thickness =
                        glm::vec2{collider.line.radius, collider.line.radius};
                    collider.aabb.mn =
                        glm::min(collider.line.line.start - thickness,
                                 collider.line.line.end + thickness);
                    collider.aabb.mx =
                        glm::max(collider.line.line.start - thickness,
                                 collider.line.line.end + thickness);
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

        // if both objects do not have physics objects then skip
        if (phy_obj_a.has_value() == false && phy_obj_b.has_value() == false) {
            continue;
        }

        float     inv_mass_a = 0;
        float     inv_mass_b = 0;
        glm::vec2 velocity_a(0);
        glm::vec2 velocity_b(0);
        bool      is_static_a = false;
        bool      is_static_b = false;

        if (phy_obj_a.has_value()) {
            const PhysicsObject2dImpl::Data &data =
                physicsObjectData(phy_obj_a.value());

            // not static
            if (data.mass > 0) {
                velocity_a = data.position - data.prev_position;
                inv_mass_a = 1.0f / data.mass;
            } else {
                inv_mass_a = 0;
                is_static_a = true;
            }
        } else {
            // static collision only object so make it's mass infinite
            inv_mass_a = 0;
            is_static_a = true;
        }

        if (phy_obj_b.has_value()) {
            const PhysicsObject2dImpl::Data &data =
                physicsObjectData(phy_obj_b.value());

            // check if the object is static
            if (data.mass > 0) {
                inv_mass_b = 1.0f / data.mass;
                velocity_b = data.position - data.prev_position;
            } else {
                // static object so make it's mass infinite
                inv_mass_b = 0;
                is_static_b = true;
            }
        } else {
            inv_mass_b = 0;
            is_static_b = true;
        }

        // if both objects are static then skip
        if (is_static_a && is_static_b) {
            continue;
        }


        const Collider2dImpl::Data &col_data_a =
            _collision_system->getBaseColliderData(pair.a);
        const Collider2dImpl::Data &col_data_b =
            _collision_system->getBaseColliderData(pair.b);

        // calculate the relative velocities along the collision normal
        // Vn = (Vb - Va) . N
        // See full impulse calculation below.
        const float Vn = glm::dot(velocity_b - velocity_a, pair.contact.normal);

        // if not already separating then resolve the collision by moving apart
        // with an impulse.
        float J = 0;
        if (Vn < 0) {

            // See: https://en.wikipedia.org/wiki/Collision_response
            // See: https://en.wikipedia.org/wiki/Coefficient_of_restitution
            // See: https://en.wikipedia.org/wiki/Impulse_(physics)
            // See: https://en.wikipedia.org/wiki/Inelastic_collision
            // https://physics.stackexchange.com/questions/598480/calculating-new-velocities-of-n-dimensional-particles-after-collision
            //
            // calculate the inelastic (i.e., with restitution) impulse
            // magnitude:
            //
            //   Relative velocity along the collision normal:
            //     Vn = (Vb - Va) . N
            //
            //          -(e + 1) * Vn
            // J = -------------------------
            //            1/ma + 1/mb
            //
            // where:
            //   e is the coefficient of restitution
            //   Vn is the relative velocity along the collision normal
            //   Va is the velocity of object A
            //   Vb is the velocity of object B
            //   ma is the mass of object A
            //   mb is the mass of object B
            //   J is the impulse magnitude

            // calculate the average of the two restitution values
            const float e =
                0.5f * (col_data_a.restitution + col_data_b.restitution);

            // calculate impulse magnitude
            J = -(e + 1.0) * Vn / (inv_mass_a + inv_mass_b);

        } else {
            // objects are already separating
            continue;
        }

        // move apart
        if (is_static_a == false && phy_obj_a.has_value()) {
            PhysicsObject2dImpl::Data &data =
                physicsObjectData(phy_obj_a.value());
            // move apart by collision normal and penetration depth
            // data.position += pair.contact.normal *
            // pair.contact.penetration;

            // apply impulse
            // See: https://en.wikipedia.org/wiki/Collision_response
            //
            //  See the calculation of the impulse magnitude above.
            //
            //  Va' = Va - (J / ma) * N
            //  where: 
            //      Va' is the new velocity of object A
            //      Va is the current velocity of object A
            //      N is the collision normal
            //      J is the impulse magnitude (see above for calculation)
            //      ma is the mass of object A
            glm::vec2 Va_prime = velocity_a - (J * inv_mass_a) * pair.contact.normal;

            // update the previous position
            // Remember that the velocity is implicit in the verlet integrator
            // so we need to update the previous position to reflect the new
            // velocity
            data.prev_position = data.position - Va_prime;

            // adjust previous position based on impulse and mass
            // data.prev_position =
            //     data.position +
            //     (impulse_vector * (1.0f - (data.mass / total_mass)));
            // PhysicsObject2dImpl::applyImpulse(data, impulse_vector, dt);
            float     start_speed = glm::length(velocity_a);
            glm::vec2 new_velocity = data.position - data.prev_position;
            float     end_speed = glm::length(new_velocity);
            // float     vab = glm::length(va);
            // float     impulse_s = glm::length(impulse);

            std::cout << "OBJ A: " << std::endl;
            std::cout << "\tstart speed: " << start_speed << std::endl;
            std::cout << "\tend speed: " << end_speed << std::endl;
            // std::cout << "vab: " << vab << std::endl;
            std::cout << "\tJ: " << J << std::endl;

            // data.prev_position = data.position; // DEBUG
        }

        if (is_static_b == false && phy_obj_b.has_value()) {
            PhysicsObject2dImpl::Data &data =
                physicsObjectData(phy_obj_b.value());

            // apply impulse
            // See: https://en.wikipedia.org/wiki/Collision_response
            //
            //  See the calculation of the impulse magnitude above.
            //
            //  Vb' = Vb + (J / ma) * N
            //  where: 
            //      Vb' is the new velocity of object B
            //      Vb is the current velocity of object B
            //      N is the collision normal
            //      J is the impulse magnitude (see above for calculation)
            //      ma is the mass of object A
            glm::vec2 Vb_prime = velocity_b + (J * inv_mass_b) * pair.contact.normal;

            // update the previous position
            // Remember that the velocity is implicit in the verlet integrator
            // so we need to update the previous position to reflect the new
            // velocity
            data.prev_position = data.position - Vb_prime;


            float     start_speed = glm::length(velocity_b);
            glm::vec2 new_velocity = data.position - data.prev_position;
            float     end_speed = glm::length(new_velocity);
            // float     vbs = glm::length(vb);
            // float     impulse_s = glm::length(impulse);

            std::cout << "OBJ B: " << std::endl;
            std::cout << "\tstart speed: " << start_speed << std::endl;
            std::cout << "\tend speed: " << end_speed << std::endl;
            // std::cout << "vbs: " << vbs << std::endl;
            std::cout << "\tJ: " << J << std::endl;

            // adjust previous position based on impulse and mass (opposite
            // direction)
            // data.prev_position =
            //     data.position -
            //     (impulse_vector * (1.0f - (data.mass / total_mass)));
            // PhysicsObject2dImpl::applyImpulse(data, impulse_vector, dt);

            // data.prev_position = data.position; // DEBUG
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