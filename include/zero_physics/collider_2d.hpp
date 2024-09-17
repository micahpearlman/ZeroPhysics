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

namespace zo {
    class Collider2d {
    public:
        virtual ~Collider2d() = default;
        virtual void setSensor(bool isSensor) = 0;
        virtual bool isSensor() const = 0;
        virtual void setFriction(float friction) = 0;
        virtual float friction() const = 0;
        virtual void setRestitution(float restitution) = 0;
        virtual float restitution() const = 0;
        virtual void setFilter(uint16_t categoryBits, uint16_t maskBits) = 0;
        virtual void getFilter(uint16_t& categoryBits, uint16_t& maskBits) const = 0;
        virtual collider_handle_2d_t handle() const = 0;
    };

    class CircleCollider2d : public Collider2d {
    public:
        virtual ~CircleCollider2d() = default;
        virtual void setRadius(float radius) = 0;
        virtual float radius() const = 0;
    };

    class LineCollider2d : public Collider2d {
    public:
        virtual ~LineCollider2d() = default;
        virtual void setStart(const glm::vec2& start) = 0;
        virtual glm::vec2 start() const = 0;
        virtual void setEnd(const glm::vec2& end) = 0;
        virtual glm::vec2 end() const = 0;
    };

    class BoxCollider2d : public Collider2d {
    public:
        virtual ~BoxCollider2d() = default;
        virtual void setWidth(float width) = 0;
        virtual float width() const = 0;
        virtual void setHeight(float height) = 0;
        virtual float height() const = 0;
    };
};

#endif // __zoPhysicsCollider2d_h__