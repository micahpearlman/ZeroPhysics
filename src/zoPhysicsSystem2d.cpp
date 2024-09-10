/**
 * @file zoPhysicsSystem2d.cpp
 * @author Micah Pearlman (micahpearlman@gmail.com)
 * @brief
 * @version 0.1
 * @date 2024-09-09
 *
 * @copyright Copyright (c) 2024
 *
 */
#include "zoPhysicsSystem2d.hpp"
#include "zoPhysicsMemory.hpp"

namespace zo {

class PhysicsSystem2dImpl : public PhysicsSystem2d {
  public:
    /// @brief The data for a physics component
    struct VerletComponentData {
        glm::vec2 position = {0, 0};
        glm::vec2 prev_position = {0, 0};
        glm::vec2 acceleration = {0, 0};
        glm::vec2 force = {0, 0};

        float mass = 1;
    };

    /// @brief A view of the VerletComponentData
    class PhysicsComponent2dView : public PhysicsComponent2d {
      public:
        PhysicsComponent2dView(glm::vec2 &p, glm::vec2 &pp, glm::vec2 &a,
                               float &m, glm::vec2 &f, PhysicsSystem2dImpl &sys,
                               phy_obj_handle_t hndl)
            : _position(p), _prev_position(pp), _acceleration(a), _mass(m),
              _force(f), _sys(sys), _hndl(hndl) {}

        void  setMass(float mass) override { _mass = mass; }
        float mass() const override { return _mass; }
        void  setPosition(const glm::vec2 &p) override {
            _position = p;
            _prev_position = p;
        }
        glm::vec2 position() const override { return _position; }
        void      setVelocity(const glm::vec2 &v) override {
            // because velocity isn't explicit in verlet we will "trick"
            // velocity by setting the previous position to the current position
            // minus the velocity
            _prev_position = _position - (v * _sys.lastTimeStep());
        }
        glm::vec2 velocity() const override {
            // remember that velocity isn't explicit in verlet so we will
            // calculate it
            return _prev_position - _position;
        }
        void setAcceleration(const glm::vec2 &a) override { _acceleration = a; }
        glm::vec2        acceleration() const override { return _acceleration; }
        void             addForce(const glm::vec2 &f) override { _force += f; }
        void             zeroForce() override { _force = glm::vec2(0); }
        phy_obj_handle_t handle() const { return _hndl; }
        bool             isValid() const override {
            return _sys.isPhysicsComponentValid(_hndl);
        }

      private:
        glm::vec2           &_position;
        glm::vec2           &_prev_position;
        glm::vec2           &_acceleration;
        float               &_mass;
        glm::vec2           &_force;
        PhysicsSystem2dImpl &_sys;
        phy_obj_handle_t     _hndl;
    };

  public:
    PhysicsSystem2dImpl(float iterations) : _iterations(iterations) {}
    virtual ~PhysicsSystem2dImpl() = default;
    void update(float dt) override {
        // sum all global forces
        glm::vec2 global_force_sum(0);
        for (auto &f : _global_forces) {
            global_force_sum += f;
        }
        for (int i = 0; i < _iterations; i++) {
            float iter_dt = dt / float(_iterations);
            for (auto &phy_obj : _physics_components) {
                VerletComponentData &data = phy_obj;
                glm::vec2            force = data.force;
                force += global_force_sum;
                glm::vec2 acceleration = force / data.mass;
                glm::vec2 new_position = data.position +
                                         (data.position - data.prev_position) +
                                         acceleration * iter_dt * iter_dt;
                data.prev_position = data.position;
                data.position = new_position;
                data.acceleration = acceleration;
                data.force = glm::vec2(0);
            }
        }
    }
    force_handle_t addGlobalForce(const glm::vec2 &f) override {
        return _global_forces.add(f);
    }
    void removeGlobalForce(force_handle_t id) override {
        _global_forces.remove(id);
    }
    std::optional<glm::vec2> getGlobalForce(force_handle_t id) const override {
        return _global_forces.get(id);
    }

    const ComponentStore<glm::vec2> &globalForces() const override {
        return _global_forces;
    }

    phy_obj_handle_t createPhysicsComponent() override {
        VerletComponentData data;
        return _physics_components.add(data);
    }

    void destroyPhysicsComponent(phy_obj_handle_t hndl) override {
        _physics_components.remove(hndl);
    }

    std::unique_ptr<PhysicsComponent2d>
    getPhysicsComponentView(phy_obj_handle_t hndl) override {
        auto result = _physics_components.get(hndl);
        if (result == std::nullopt) {
            return nullptr;
        }
        VerletComponentData &data = result->get();
        return std::make_unique<PhysicsComponent2dView>(
            data.position, data.prev_position, data.acceleration, data.mass,
            data.force, *this, hndl);
    }

    bool isPhysicsComponentValid(phy_obj_handle_t hndl) const override {
        return _physics_components.get(hndl) != std::nullopt;
    }

    float lastTimeStep() const { return _last_time_step; }

  protected:
    float                               _last_time_step = 1 / 60.0f;
    int                                 _iterations = 1;
    ComponentStore<glm::vec2>           _global_forces;
    ComponentStore<VerletComponentData> _physics_components;
};

std::shared_ptr<PhysicsSystem2d> PhysicsSystem2d::create(int iterations) {
    PhysicsSystem2dImpl *impl = new PhysicsSystem2dImpl(iterations);
    return std::shared_ptr<PhysicsSystem2d>(impl);
}

} // namespace zo