/**
 * @file broad_phase.hpp
 * @author Micah Pearlman (micahpearlman@gmail.com)
 * @brief Broad phase collision detection interface.
 * @version 0.1
 * @date 2024-10-02
 *
 * @copyright Copyright (c) 2024
 *
 */
#ifndef __broaphase_h__
#define __broaphase_h__
#include "types_impl.hpp"
#include <vector>

namespace zo {
class CollisionSystem2dImpl;

class BroadPhase {
  public:
    virtual ~BroadPhase() = default;

    BroadPhase(CollisionSystem2dImpl &collision_system)
        : _col_sys(collision_system) {}

    virtual void                              generateCollisionPairs() = 0;
    virtual const std::vector<CollisionPair> &collisionPairs() const = 0;

  protected:
    CollisionSystem2dImpl &_col_sys;
};

/// @brief Naive broad phase collision detection O(N^2)
/// essentially checks all pairs of colliders
class NaiveBroadPhase : public BroadPhase {
  public:
    NaiveBroadPhase(CollisionSystem2dImpl &collision_system)
        : BroadPhase(collision_system) {}

    void generateCollisionPairs() override;

    const std::vector<CollisionPair> &collisionPairs() const override {
        return _collision_pairs;
    }

  private:
    std::vector<CollisionPair> _collision_pairs;
};

class GridBroadPhase : public BroadPhase {
  public:
    GridBroadPhase(CollisionSystem2dImpl &collision_system, int grid_size)
        : BroadPhase(collision_system), _grid_size(grid_size) {}

    void generateCollisionPairs() override;

    const std::vector<CollisionPair> &collisionPairs() const override {
        return _collision_pairs;
    }

  private:
    // Custom hash function for std::pair<int, int>
    struct pair_hash {
        template <class T1, class T2>
        std::size_t operator()(const std::pair<T1, T2> &pair) const {
            auto hash1 = std::hash<T1>{}(pair.first);
            auto hash2 = std::hash<T2>{}(pair.second);
            return hash1 ^ (hash2 << 1); // Combine the two hash values
        }
    };

    struct collision_pair_hash {
        std::size_t operator()(const CollisionPair &pair) const {
            auto hash1 = std::hash<uint32_t>{}(pair.a.handle);
            auto hash2 = std::hash<uint32_t>{}(pair.b.handle);
            return hash1 ^ (hash2 << 1); // Combine the two hash values
        }
    };

  private:
    std::vector<CollisionPair> _collision_pairs;
    int                        _grid_size = 50;
};

} // namespace zo
#endif // __broaphase_h__
