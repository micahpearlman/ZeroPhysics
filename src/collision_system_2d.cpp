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

std::shared_ptr<CollisionSystem2d>
CollisionSystem2d::create(size_t         max_colliders,
                          BroadPhaseType broad_phase_type) {
    return std::make_shared<CollisionSystem2dImpl>(max_colliders,
                                                   broad_phase_type);
}

CollisionSystem2dImpl::CollisionSystem2dImpl(size_t         max_colliders,
                                             BroadPhaseType broad_phase_type)
    : _circle_collider_pool(max_colliders) {

    // make sure the max colliders cannot be greater then 28 bits
    if (max_colliders > (1 << 28)) {
        throw std::runtime_error("max_colliders must be less than 2^28");
    }

    switch (broad_phase_type) {
    case BroadPhaseType::NAIVE: {
        _broad_phase = std::make_unique<NaiveBroadPhase>(*this);
    } break;
    case BroadPhaseType::GRID: {
        _broad_phase = std::make_unique<GridBroadPhase>(*this, 50); // HARDWIRED! TODO: make grid size configurable
    } break;
    default:
        throw std::runtime_error("Unsupported broad phase type");
        break;
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

const Collider2dImpl::Data &CollisionSystem2dImpl::getBaseColliderData(
    const collider_handle_2d_t &hndl) const {
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
    _broad_phase->generateCollisionPairs();

    // get the broad phase collision pairs
    const std::vector<CollisionPair> &pairs = _broad_phase->collisionPairs();

    // do narrow phase collision detection
    for (const CollisionPair &pair : pairs) {
        contact_2d_t contact = {};

        if (pair.a.type == uint8_t(ColliderType::CIRCLE) &&
            pair.b.type == uint8_t(ColliderType::CIRCLE)) {
            const auto &c1_data =
                getColliderData<CircleCollider2dImpl::Data>(pair.a);
            const auto &c2_data =
                getColliderData<CircleCollider2dImpl::Data>(pair.b);

            if (circleToCircle(c1_data.circle, c2_data.circle, contact)) {
                _collision_pairs.emplace_back(pair);
                _collision_pairs.back().contact = contact;
            }
        } else if (pair.a.type == uint8_t(ColliderType::CIRCLE) &&
                   pair.b.type == uint8_t(ColliderType::LINE)) {
            const auto &c1_data =
                getColliderData<CircleCollider2dImpl::Data>(pair.a);
            const auto &c2_data =
                getColliderData<LineCollider2dImpl::Data>(pair.b);
            if (circleToThickLineSegment(c1_data.circle, c2_data.line, contact)) {
                _collision_pairs.emplace_back(pair);                
                _collision_pairs.back().contact = contact;
            }
        } else if (pair.a.type == uint8_t(ColliderType::LINE) &&
                   pair.b.type == uint8_t(ColliderType::CIRCLE)) {
            const auto &c1_data =
                getColliderData<LineCollider2dImpl::Data>(pair.a);
            const auto &c2_data =
                getColliderData<CircleCollider2dImpl::Data>(pair.b);

            // NOTE: circle to line segment can only do circle to line ordering
            // in this case we need to flip the order from A to B to B to A
            // this preserves the collision normal
            if (circleToThickLineSegment(c2_data.circle, c1_data.line, contact)) {
                CollisionPair pair_flip = {pair.b, pair.a, contact};
                _collision_pairs.emplace_back(pair_flip);
                _collision_pairs.back().contact = contact;
            }
        }
    }
#if 0
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
                const auto &c1_data =
                    getColliderData<CircleCollider2dImpl::Data>(c1);
                const auto &c2_data =
                    getColliderData<CircleCollider2dImpl::Data>(c2);

                if (circleToCircle(c1_data.circle, c2_data.circle, contact)) {
                    CollisionPair pair = {c1, c2, contact};
                    _collision_pairs.emplace_back(pair);
                }

            } else if (c1.type == uint8_t(ColliderType::LINE) &&
                       c2.type == uint8_t(ColliderType::LINE)) {
                const auto &c1_data =
                    getColliderData<LineCollider2dImpl::Data>(c1);
                const auto &c2_data =
                    getColliderData<LineCollider2dImpl::Data>(c2);
                // do line to line collision
                // if (lineToLine(c1_data, c2_data)) {
                //     CollisionPair pair = {c1, c2};
                //     _collision_pairs.push_back(pair);
                // }
            } else if (c1.type == uint8_t(ColliderType::CIRCLE) &&
                       c2.type == uint8_t(ColliderType::LINE)) {
                const auto &c1_data =
                    getColliderData<CircleCollider2dImpl::Data>(c1);
                const auto &c2_data =
                    getColliderData<LineCollider2dImpl::Data>(c2);
                if (circleToLineSegment(c1_data.circle, c2_data.line,
                                        contact)) {
                    CollisionPair pair = {c1, c2, contact};
                    _collision_pairs.emplace_back(pair);
                }
            } else if (c1.type == uint8_t(ColliderType::LINE) &&
                       c2.type == uint8_t(ColliderType::CIRCLE)) {
                const auto &c1_data =
                    getColliderData<LineCollider2dImpl::Data>(c1);
                const auto &c2_data =
                    getColliderData<CircleCollider2dImpl::Data>(c2);
                if (circleToLineSegment(c2_data.circle, c1_data.line,
                                        contact)) {

                    CollisionPair pair = {c1, c2, contact};
                    // flip normal so it points out of the circle
                    pair.contact.normal = -pair.contact.normal;

                    _collision_pairs.emplace_back(pair);
                }
            }
        }
    }
#endif // 0
}

} // namespace zo
