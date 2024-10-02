/**
 * @file types_impl.hpp
 * @author Micah Pearlman (micahpearlman@gmail.com)
 * @brief
 * @version 0.1
 * @date 2024-09-23
 *
 * @copyright Copyright (c) 2024
 *
 */
#ifndef __types_impl_h__
#define __types_impl_h__

#include <zero_physics/types.hpp>
#include <cstdint>

namespace zo {

/// @brief Implementation of a collider handle.  See types.hpp
union ColliderHandle {
    struct {
        /// @brief The type of collider (see: ColliderType). Packed into 4 bits.
        uint8_t type : 4;

        /// @brief The index of the collider in the memory pool. Packed into 28
        /// bits. Note: the memory pool must be less than 2^28.
        uint32_t index : 28;
    };

    /// @brief The packed handle
    uint32_t handle;
};

/// @brief Implementation of a collision pair. See types.hpp
struct CollisionPair {
    ColliderHandle a;
    ColliderHandle b;
    contact_2d_t   contact;
};

} // namespace zo

#endif // __types_impl_h__