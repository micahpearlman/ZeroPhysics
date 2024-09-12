/**
 * @file zoPhysicsCollisionSys.hpp
 * @author Micah Pearlman (micahpearlman@gmail.com)
 * @brief Manages collision detection.  Ultimately returns a list of collision pairs.
 * @version 0.1
 * @date 2024-09-12
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef __zoPhysicsCollisionSys_hpp__
#define __zoPhysicsCollisionSys_hpp__
#include <memory>
namespace zo {
class CollisionSystem2d {
public:
    static std::shared_ptr<CollisionSystem2d> create();
};

}
#endif // __zoPhysicsCollisionSys_hpp__