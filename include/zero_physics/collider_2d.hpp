/**
 * @file collider_2d.hpp
 * @author Micah Pearlman (micahpearlman@gmail.com)
 * @brief
 * @version 0.1
 * @date 2024-09-09
 *
 * @copyright Copyright (c) 2024
 *
 */
#ifndef __zoPhysicsCollider2d_h__
#define __zoPhysicsCollider2d_h__

#include <zero_physics/types.hpp>
#include <glm/glm.hpp>
#include <memory>

namespace zo {

class CollisionSystem2d;

/**
 * @brief Base 2d Collider interface.
 * This is a view into the collider data which is managed by the collision
 * system.
 *
 */
class Collider2d {
  public:
    virtual ~Collider2d() = default;
    virtual void  setSensor(bool isSensor) = 0;
    virtual bool  isSensor() const = 0;
    virtual void  setFriction(float friction) = 0;
    virtual float friction() const = 0;
    virtual void  setRestitution(float restitution) = 0;
    virtual float restitution() const = 0;
    virtual void  setFilter(uint16_t categoryBits, uint16_t maskBits) = 0;
    virtual void  getFilter(uint16_t &categoryBits,
                            uint16_t &maskBits) const = 0;
    virtual const aabb_2d_t     &aabb() const = 0;
    virtual collider_handle_2d_t handle() const = 0;
    virtual ColliderType         type() const = 0;

    template <typename T> T &as() { return *dynamic_cast<T *>(this); }
};

/**
 * @brief Circle collider interface.
 * This is a view into the circle collider data which is managed by the
 * collision system.
 *
 */
class CircleCollider2d : virtual public Collider2d {
  public:
    virtual ~CircleCollider2d() = default;
    virtual void        setRadius(float radius) = 0;
    virtual float       radius() const = 0;
    virtual void        setCenter(const glm::vec2 &center) = 0;
    virtual glm::vec2   center() const = 0;
    virtual void        setCircle(const circle_2d_t &circle) = 0;
    virtual circle_2d_t circle() const = 0;

    ColliderType type() const override { return ColliderType::CIRCLE; }

    static std::unique_ptr<CircleCollider2d>
    create(CollisionSystem2d &collision_system);
};

class LineCollider2d : virtual public Collider2d {
  public:
    virtual ~LineCollider2d() = default;
    virtual void                    setStart(const glm::vec2 &start) = 0;
    virtual glm::vec2               start() const = 0;
    virtual void                    setEnd(const glm::vec2 &end) = 0;
    virtual glm::vec2               end() const = 0;
    virtual void                    setThickness(float thickness) = 0;
    virtual float                   thickness() const = 0;
    virtual void                    setLine(const thick_line_segment_2d_t &line) = 0;
    virtual thick_line_segment_2d_t line() const = 0;

    ColliderType type() const override { return ColliderType::LINE; }

    static std::unique_ptr<LineCollider2d>
    create(CollisionSystem2d &collision_system);
};

class BoxCollider2d : virtual public Collider2d {
  public:
    virtual ~BoxCollider2d() = default;
    virtual void  setWidth(float width) = 0;
    virtual float width() const = 0;
    virtual void  setHeight(float height) = 0;
    virtual float height() const = 0;

    ColliderType type() const override { return ColliderType::BOX; }

    static std::unique_ptr<LineCollider2d>
    create(CollisionSystem2d &collision_system);
};
}; // namespace zo

#endif // __zoPhysicsCollider2d_h__