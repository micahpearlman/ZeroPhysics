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

namespace zo {

CollisionSystem2dImpl::CollisionSystem2dImpl(size_t max_colliders)
    : _circle_collider_pool(max_colliders) {

    // make sure the max colliders cannot be greater then 28 bits
    if (max_colliders > (1 << 28)) {
        throw std::runtime_error("max_colliders must be less than 2^28");
    }
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
        return hndl;
    } break;

    default:
        break;
    }

    return std::nullopt;
}

} // namespace zo
