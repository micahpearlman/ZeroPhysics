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

class CollisionSystem2dImpl;
class Collider2dImpl : virtual public Collider2d {
  protected:
    struct alignas(std::max_align_t) Data {
        bool         is_sensor;
        float        friction;
        float        restitution;
        uint16_t     category_bits;
        uint16_t     mask_bits;
        ColliderType type;
    };

    CollisionSystem2dImpl &_sys;
    collider_handle_2d_t   _hndl;

  public:
    Collider2dImpl(CollisionSystem2dImpl &collision_system,
                   collider_handle_2d_t handle, ColliderType type);
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
    ColliderType         type() const override;

  private:
    Data       &data();
    const Data &data() const;
};

class CircleCollider2dImpl : public Collider2dImpl, public CircleCollider2d {
  public:
    struct alignas(std::max_align_t) Data : Collider2dImpl::Data {
        circle_2d_t circle;
    };

  public:
    CircleCollider2dImpl(CollisionSystem2dImpl &collision_system,
                         collider_handle_2d_t handle, circle_2d_t circle);
    virtual ~CircleCollider2dImpl() = default;
    void        setRadius(float radius) override;
    float       radius() const override;
    void        setCenter(const glm::vec2 &center) override;
    glm::vec2   center() const override;
    void        setCircle(const circle_2d_t &circle) override;
    circle_2d_t circle() const override;
};

class LineCollider2dImpl : public Collider2dImpl, public LineCollider2d {
  public:
    struct alignas(std::max_align_t) Data : Collider2dImpl::Data {
        line_segment_2d_t line;
    };

  public:
    LineCollider2dImpl(CollisionSystem2dImpl &collision_system,
                       collider_handle_2d_t handle, line_segment_2d_t line);
    virtual ~LineCollider2dImpl() = default;
    void              setStart(const glm::vec2 &start) override;
    glm::vec2         start() const override;
    void              setEnd(const glm::vec2 &end) override;
    glm::vec2         end() const override;
    void              setLine(const line_segment_2d_t &line) override;
    line_segment_2d_t line() const override;
};
} // namespace zo
#endif // __zoPhysicsCollider2dImpl_h__
