#include "broad_phase.hpp"
#include "collision_system_2d_impl.hpp"

namespace zo {
void NaiveBroadPhase::generateCollisionPairs() {
    const ComponentStore<collider_handle_2d_t> &colliders = _col_sys.colliders();
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
} // namespace zo
