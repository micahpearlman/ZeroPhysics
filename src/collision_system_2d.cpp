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

std::shared_ptr<CollisionSystem2d> CollisionSystem2d::create() {
    return std::make_shared<CollisionSystem2dImpl>();
}

collider_handle_2d_t CollisionSystem2dImpl::createCollider(ColliderType type) {
    switch (type)
    {
    case ColliderType::CIRCLE:
        /* code */
        break;
    
    default:
        break;
    }
}

} // namespace zo
