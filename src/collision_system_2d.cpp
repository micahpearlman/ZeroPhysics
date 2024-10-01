/**
 * @file collision_system_2d.cpp
 * @author Micah Pearlman (micahpearlman@gmail.com)
 * @brief
 * @version 0.1
 * @date 2024-09-17
 *
 * @copyright Copyright (c) 2024
 *
 */
#include "physics_system_2d_impl.hpp"
#include <zero_physics/math.hpp>

namespace zo {

CollisionSystem2dImpl::CollisionSystem2dImpl(size_t max_colliders)
    : _circle_collider_pool(max_colliders) {

    // make sure the max colliders cannot be greater then 28 bits
    if (max_colliders > (1 << 28)) {
        throw std::runtime_error("max_colliders must be less than 2^28");
    }
}

void CollisionSystem2dImpl::destroyCollider(collider_handle_2d_t hndl) {
    switch (hndl.type) {
    case uint8_t(ColliderType::CIRCLE): {
        _circle_collider_pool.deallocate(hndl.index);
    } break;
    case uint8_t(ColliderType::LINE): {
        _line_collider_pool.deallocate(hndl.index);
    } break;
    default:
        break;
    }
}

void CollisionSystem2dImpl::destroyCollider(
    std::unique_ptr<Collider2d> collider) {
    destroyCollider(collider->handle());
}

std::shared_ptr<CollisionSystem2d>
CollisionSystem2d::create(size_t max_colliders) {
    return std::make_shared<CollisionSystem2dImpl>(max_colliders);
}

std::optional<collider_handle_2d_t>
CollisionSystem2dImpl::createCollider(ColliderType type) {
    switch (type) {
    case ColliderType::CIRCLE: {
        CircleCollider2dImpl::Data *data = _circle_collider_pool.allocate();
        if (data == nullptr) {
            return std::nullopt;
        }
        collider_handle_2d_t hndl = {
            uint8_t(ColliderType::CIRCLE),
            uint32_t(_circle_collider_pool.ptrToIdx(data))};
        _colliders.add(hndl);
        return hndl;
    } break;
    case ColliderType::LINE: {
        LineCollider2dImpl::Data *data = _line_collider_pool.allocate();
        if (data == nullptr) {
            return std::nullopt;
        }
        collider_handle_2d_t hndl = {
            uint8_t(ColliderType::LINE),
            uint32_t(_line_collider_pool.ptrToIdx(data))};
        _colliders.add(hndl);
        return hndl;
    } break;

    default:
        break;
    }

    return std::nullopt;
}

Collider2dImpl::Data &
CollisionSystem2dImpl::getBaseColliderData(const collider_handle_2d_t& hndl) {
    switch (hndl.type) {
    case uint8_t(ColliderType::CIRCLE): {
        return getColliderData<CircleCollider2dImpl::Data>(hndl);
    } break;
    case uint8_t(ColliderType::LINE): {
        return getColliderData<LineCollider2dImpl::Data>(hndl);
    } break;
    default:
        break;
    }
    throw std::runtime_error("Unsupported collider type");
}

void CollisionSystem2dImpl::generateCollisionPairs() {
    // clear the collision pairs
    _collision_pairs.clear();

    // generate collision pairs
    // O(n^2) collision detection. TODO: implement broadphase collision
    // detection
    for (int i = 0; i < _colliders.size(); i++) {
        const ColliderHandle &c1 = _colliders.at(i);
        for (int p = i + 1; p < _colliders.size(); p++) {
            const ColliderHandle &c2 = _colliders.at(p);
            if (c1.index == c2.index && c1.type == c2.type) {
                continue;
            }

            contact_2d_t contact;

            // check for collision
            if (c1.type == uint8_t(ColliderType::CIRCLE) &&
                c2.type == uint8_t(ColliderType::CIRCLE)) {
                const auto &c1_data = getColliderData<CircleCollider2dImpl::Data>(c1);
                const auto &c2_data = getColliderData<CircleCollider2dImpl::Data>(c2);

                if (circleToCircle(c1_data.circle, c2_data.circle, contact)) {
                    CollisionPair pair = {c1, c2, contact};
                    _collision_pairs.emplace_back(pair);
                }

            } else if (c1.type == uint8_t(ColliderType::LINE) &&
                       c2.type == uint8_t(ColliderType::LINE)) {
                const auto &c1_data = getColliderData<LineCollider2dImpl::Data>(c1);
                const auto &c2_data = getColliderData<LineCollider2dImpl::Data>(c2);
                // do line to line collision
                // if (lineToLine(c1_data, c2_data)) {
                //     CollisionPair pair = {c1, c2};
                //     _collision_pairs.push_back(pair);
                // }
            } else if (c1.type == uint8_t(ColliderType::CIRCLE) &&
                       c2.type == uint8_t(ColliderType::LINE)) {
                const auto &c1_data = getColliderData<CircleCollider2dImpl::Data>(c1);                
                const auto &c2_data = getColliderData<LineCollider2dImpl::Data>(c2);
                if (circleToLineSegment(c1_data.circle, c2_data.line, contact)) {
                    CollisionPair pair = {c1, c2, contact};
                    _collision_pairs.emplace_back(pair);
                }
            } else if (c1.type == uint8_t(ColliderType::LINE) &&
                       c2.type == uint8_t(ColliderType::CIRCLE)) {
                const auto &c1_data = getColliderData<LineCollider2dImpl::Data>(c1);
                const auto &c2_data = getColliderData<CircleCollider2dImpl::Data>(c2);
                if (circleToLineSegment(c2_data.circle, c1_data.line, contact)) {

                    CollisionPair pair = {c1, c2, contact};
                    // flip normal so it points out of the circle
                    pair.contact.normal = -pair.contact.normal;

                    _collision_pairs.emplace_back(pair);
                }
            }
        }
    }
}

} // namespace zo
