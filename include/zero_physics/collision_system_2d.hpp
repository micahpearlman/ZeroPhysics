/**
 * @file collision_system_2d.hpp
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
#include <zero_physics/types.hpp>
#include <zero_physics/collider_2d.hpp>
#include <memory>
#include <optional>
namespace zo {
class CollisionSystem2d {
public:

    
    /// @brief Create collision system implementation.
    /// @param max_colliders Maximum number of colliders.
    /// @return 
    static std::shared_ptr<CollisionSystem2d> create(size_t max_colliders);

    virtual ~CollisionSystem2d() = default;



    /// @brief Create a collider of a specific type.
    /// The collider will be added into the collision system.
    /// @tparam T The type of collider to create (CircleCollider2d,LineCollider2d, etc)
    /// @return Unique pointer to the collider
    template<typename T>
    std::unique_ptr<T> createCollider() {
        if constexpr (std::is_same_v<T, CircleCollider2d>) {
            return CircleCollider2d::create(*this);
        } else if constexpr (std::is_same_v<T, LineCollider2d>) {
            return LineCollider2d::create(*this);
        } else {
            static_assert(std::is_same_v<T, CircleCollider2d> || std::is_same_v<T, LineCollider2d>, "Invalid collider type");
        }
    }

    /// @brief Destroy a collider.
    /// @param hndl The handle to the collider to destroy.
    virtual void destroyCollider(collider_handle_2d_t hndl) = 0;

    /// @brief Destroy a collider.
    /// @param collider The collider to destroy.
    virtual void destroyCollider(std::unique_ptr<Collider2d> collider) = 0;

    /// @brief Run the collision system generating collision pairs.
    virtual void generateCollisionPairs() = 0;



};

}
#endif // __zoPhysicsCollisionSys_hpp__