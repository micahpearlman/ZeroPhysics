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
#include "types_impl.hpp"

namespace zo {

class CollisionSystem2dImpl;

class Collider2dImpl : virtual public Collider2d {
  public:
    struct alignas(std::max_align_t) Data {
        uint8_t   type = uint8_t(ColliderType::MAX);
        bool      is_sensor = false;
        float     friction = 0.0f;
        float     restitution = 0.83;
        uint16_t  category_bits = 0;
        uint16_t  mask_bits = 0;
        aabb_2d_t aabb;
    };

    CollisionSystem2dImpl &_sys;
    collider_handle_2d_t   _hndl;

  public:
    Collider2dImpl(CollisionSystem2dImpl &collision_system,
                   collider_handle_2d_t   handle);
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

    CollisionSystem2dImpl &system() { return _sys; }

  protected:
    virtual Collider2dImpl::Data       &baseData() = 0;
    virtual const Collider2dImpl::Data &baseData() const = 0;
};

class CircleCollider2dImpl : public Collider2dImpl, public CircleCollider2d {
  public:
    struct alignas(std::max_align_t) Data : Collider2dImpl::Data {
        circle_2d_t circle;
    };

  public:
    CircleCollider2dImpl(CollisionSystem2dImpl &collision_system,
                         collider_handle_2d_t   handle);
    virtual ~CircleCollider2dImpl() = default;
    void        setRadius(float radius) override;
    float       radius() const override;
    void        setCenter(const glm::vec2 &center) override;
    glm::vec2   center() const override;
    void        setCircle(const circle_2d_t &circle) override;
    circle_2d_t circle() const override;

    const aabb_2d_t &aabb() const override;

    /// @brief Update the axis aligned bounding box
    void updateAabb();

    /// @brief Get the base collider data
    /// @return Collider2dImpl::Data& base collider data
    Collider2dImpl::Data &baseData() override;

    /// @brief Get the base collider data
    /// @return const Collider2dImpl::Data& base collider data
    const Collider2dImpl::Data &baseData() const override;

    /// @brief Get the circle collider data
    /// @return CircleCollider2dImpl::Data
    CircleCollider2dImpl::Data &data() { return _data; }

    /// @brief Get the circle collider data
    /// @return const CircleCollider2dImpl::Data
    CircleCollider2dImpl::Data const &data() const { return _data; }

  private:
    CircleCollider2dImpl::Data &_data;
};

class LineCollider2dImpl : public Collider2dImpl, public LineCollider2d {
  public:
    struct alignas(std::max_align_t) Data : Collider2dImpl::Data {
        thick_line_segment_2d_t line;
    };

  public:
    LineCollider2dImpl(CollisionSystem2dImpl &collision_system,
                       collider_handle_2d_t   handle);
    virtual ~LineCollider2dImpl() = default;
    void      setStart(const glm::vec2 &start) override;
    glm::vec2 start() const override;
    void      setEnd(const glm::vec2 &end) override;
    glm::vec2 end() const override;
    void      setThickness(float thickness) override;
    float     thickness() const override;
    void      setLine(const thick_line_segment_2d_t &line) override;
    thick_line_segment_2d_t line() const override;

    /// @brief Get the base collider data
    /// @return Collider2dImpl::Data& base collider data
    Collider2dImpl::Data       &baseData() override;
    const Collider2dImpl::Data &baseData() const override;

    /// @brief Update the axis aligned bounding box
    void updateAabb();

    const aabb_2d_t &aabb() const override;

    /// @brief Get the line collider data
    /// @return LineCollider2dImpl::Data
    LineCollider2dImpl::Data &data() { return _data; }

    /// @brief Get the line collider data
    /// @return const LineCollider2dImpl::Data
    LineCollider2dImpl::Data const &data() const { return _data; }

  private:
    LineCollider2dImpl::Data &_data;
};
} // namespace zo
#endif // __zoPhysicsCollider2dImpl_h__
