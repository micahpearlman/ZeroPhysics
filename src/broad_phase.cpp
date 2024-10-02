#include "broad_phase.hpp"
#include "collision_system_2d_impl.hpp"
#include <unordered_map>
#include <unordered_set>
#include <iostream>

namespace zo {
void NaiveBroadPhase::generateCollisionPairs() {
    const ComponentStore<collider_handle_2d_t> &colliders =
        _col_sys.colliders();
    _collision_pairs.clear();
    for (int i = 0; i < colliders.size(); i++) {
        const ColliderHandle &c1 = colliders.at(i);
        for (int p = i + 1; p < colliders.size(); p++) {
            const ColliderHandle &c2 = colliders.at(p);
            // ignore self collision
            if (c1.handle == c2.handle) {
                continue;
            }
            CollisionPair pair;
            pair.a = c1;
            pair.b = c2;
            _collision_pairs.push_back(pair);
        }
    }
}

void GridBroadPhase::generateCollisionPairs() {
    std::unordered_map<std::pair<int, int>, std::vector<ColliderHandle>,
                       GridBroadPhase::pair_hash>
        grid_map;

    const ComponentStore<ColliderHandle> &colliders = _col_sys.colliders();
    _collision_pairs.clear();
    const float inv_grid_size = 1.0f / _grid_size;
    for (int i = 0; i < colliders.size(); i++) {
        const ColliderHandle &hndl = colliders.at(i);
        const auto      &base_col_data = _col_sys.getBaseColliderData(hndl);
        const aabb_2d_t &aabb = base_col_data.aabb;


        const glm::vec2 mn = aabb.mn * inv_grid_size;
        const glm::vec2 mx = aabb.mx * inv_grid_size;
        const int       mn_x = std::floor(mn.x);
        const int       mn_y = std::floor(mn.y);
        const int       mx_x = std::floor(mx.x);
        const int       mx_y = std::floor(mx.y);

        if (hndl.type == uint8_t(ColliderType::LINE)) {
            std::cout << "Line Collider: " << mn_x << " " << mn_y << " " << mx_x << " " << mx_y << std::endl;
        }

        if (hndl.type == uint8_t(ColliderType::CIRCLE)) {
            std::cout << "Circle Collider: " << mn_x << " " << mn_y << " " << mx_x << " " << mx_y << std::endl;
        }


        for (int x = mn_x; x <= mx_x; x++) {
            for (int y = mn_y; y <= mx_y; y++) {
                const std::pair<int, int> grid_key{x, y};
                grid_map[grid_key].emplace_back(hndl);
            }
        }
    }

    // generate collision pair set to avoid duplicates
    std::unordered_set<CollisionPair, GridBroadPhase::collision_pair_hash> collision_pair_set;
    CollisionPair pair;
    for (const auto &[cell, colliders] : grid_map) {
        for (int i = 0; i < colliders.size(); i++) {
            const ColliderHandle &c1 = colliders[i];
            for (int p = i + 1; p < colliders.size(); p++) {
                const ColliderHandle &c2 = colliders[p];
                // ignore self collision
                if (c1.handle == c2.handle) {
                    continue;
                }                
                pair.a = c1;
                pair.b = c2;
                collision_pair_set.insert(pair);

            }
        }
    }

    for (const auto &pair : collision_pair_set) {
        _collision_pairs.emplace_back(pair);
    }
}
} // namespace zo
