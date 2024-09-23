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

#include <cstdint>

namespace zo {

struct ColliderHandle {
    uint8_t  type : 4;
    uint32_t index : 28;
};

} // namespace zo

#endif // __types_impl_h__