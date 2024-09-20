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

std::unique_ptr<Collider2d> CollisionSystem2dImpl::createCollider(Collider2d::ColliderType type) {
    switch (type) {
    case  Collider2d::ColliderType::CIRCLE:
        {
            ColliderDataVariant* data_variant = _collider_data_pool.allocate();
            if (data_variant == nullptr) {
                return nullptr;
            }
            data_variant->emplace<CircleCollider2dImpl::Data>();
            collider_handle_2d_t hndl = static_cast<collider_handle_2d_t>(_collider_data_pool.ptrToIdx(data_variant));
            return std::make_unique<CircleCollider2dImpl>(*this, hndl);
            // return std::make_unique<CircleCollider2dImpl>(data_variant);
        }
        break;
    case  Collider2d::ColliderType::LINE:
        /* code */
        break;

    default:
        break;
    }

    return nullptr;
}

} // namespace zo
