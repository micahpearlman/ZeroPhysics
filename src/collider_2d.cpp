/**
 * @file collider_2d.cpp
 * @author Micah Pearlman (micahpearlman@gmail.com)
 * @brief
 * @version 0.1
 * @date 2024-09-17
 *
 * @copyright Copyright (c) 2024
 *
 */
#include "collider_2d_impl.hpp"
#include "collision_system_2d_impl.hpp"
#include <zero_physics/types.hpp>

namespace zo {
Collider2dImpl::Collider2dImpl(CollisionSystem2dImpl &collision_system,
                               collider_handle_2d_t  &handle)
    : _sys(collision_system), _hndl(handle) {}

void Collider2dImpl::setSensor(bool isSensor) {
    baseData().is_sensor = isSensor;
}

bool Collider2dImpl::isSensor() const { return baseData().is_sensor; }

void Collider2dImpl::setFriction(float friction) {
    baseData().friction = friction;
}

float Collider2dImpl::friction() const { return baseData().friction; }

void Collider2dImpl::setRestitution(float restitution) {
    baseData().restitution = restitution;
}

float Collider2dImpl::restitution() const { return baseData().restitution; }

void Collider2dImpl::setFilter(uint16_t categoryBits, uint16_t maskBits) {
    baseData().category_bits = categoryBits;
    baseData().mask_bits = maskBits;
}

void Collider2dImpl::getFilter(uint16_t &categoryBits,
                               uint16_t &maskBits) const {
    categoryBits = baseData().category_bits;
    maskBits = baseData().mask_bits;
}

collider_handle_2d_t Collider2dImpl::handle() const { return _hndl; }

/// CircleCollider2dImpl

CircleCollider2dImpl::CircleCollider2dImpl(
    CollisionSystem2dImpl &collision_system, collider_handle_2d_t &handle)
    : Collider2dImpl(collision_system, handle),
      _data{system().getColliderData<CircleCollider2dImpl::Data>(handle)} {}

void CircleCollider2dImpl::setRadius(float radius) {
    data().circle.radius = radius;
}
float CircleCollider2dImpl::radius() const { return data().circle.radius; }
void  CircleCollider2dImpl::setCenter(const glm::vec2 &center) {
    data().circle.center = center;
}
glm::vec2 CircleCollider2dImpl::center() const { return data().circle.center; }
void      CircleCollider2dImpl::setCircle(const circle_2d_t &circle) {
    data().circle = circle;
}
circle_2d_t CircleCollider2dImpl::circle() const { return data().circle; }

Collider2dImpl::Data       &CircleCollider2dImpl::baseData() { return data(); }
const Collider2dImpl::Data &CircleCollider2dImpl::baseData() const {
    return data();
}

/// LineCollider2dImpl
LineCollider2dImpl::LineCollider2dImpl(CollisionSystem2dImpl &collision_system,
                                       collider_handle_2d_t  &handle)
    : Collider2dImpl(collision_system, handle),
      _data{system().getColliderData<LineCollider2dImpl::Data>(handle)} {}

void LineCollider2dImpl::setStart(const glm::vec2 &start) {
    data().line.start = start;
}

glm::vec2 LineCollider2dImpl::start() const { return data().line.start; }

void LineCollider2dImpl::setEnd(const glm::vec2 &end) { data().line.end = end; }

glm::vec2 LineCollider2dImpl::end() const { return data().line.end; }

void LineCollider2dImpl::setLine(const line_segment_2d_t &line) {
    data().line = line;
}

line_segment_2d_t LineCollider2dImpl::line() const { return data().line; }

Collider2dImpl::Data       &LineCollider2dImpl::baseData() { return data(); }
const Collider2dImpl::Data &LineCollider2dImpl::baseData() const {
    return data();
}

} // namespace zo
