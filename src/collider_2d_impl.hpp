/**
 * @file collider_2d_impl.hpp
 * @author Micah Pearlman (micahpearlman@gmail.com)
 * @brief
 * @version 0.1
 * @date 2024-09-17
 *
 * @copyright Copyright (c) 2024
 *
 */
#ifndef __zoPhysicsCollider2dImpl_h__
#define __zoPhysicsCollider2dImpl_h__

#include <zero_physics/collider_2d.hpp>
#include <zero_physics/types.hpp>

namespace zo {
class Collider2dImpl : public Collider2d {
  protected:
    struct Data {
        bool     isSensor;
        float    friction;
        float    restitution;
        uint16_t categoryBits;
        uint16_t maskBits;
    };

  public:
    virtual ~Collider2dImpl() = default;
    void  setSensor(bool isSensor) override;
    bool  isSensor() const override;
    void  setFriction(float friction) override;
    float friction() const override;
    void  setRestitution(float restitution) override;
    float restitution() const override;
    void  setFilter(uint16_t categoryBits, uint16_t maskBits) override;
    void  getFilter(uint16_t &categoryBits, uint16_t &maskBits) const override;
    collider_handle_2d_t handle() const override;
};
} // namespace zo
#endif // __zoPhysicsCollider2dImpl_h__
