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
    : _collider_data_pool(max_colliders) {}

std::shared_ptr<CollisionSystem2d> CollisionSystem2d::create(size_t max_colliders) {
    return std::make_shared<CollisionSystem2dImpl>(max_colliders);
}

collider_handle_2d_t CollisionSystem2dImpl::createCollider(Collider2d::ColliderType type) {
    switch (type) {
    case  Collider2d::ColliderType::CIRCLE:
        /* code */
        break;

    default:
        break;
    }
}

} // namespace zo
