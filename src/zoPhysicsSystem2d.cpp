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
        PhysicsComponent2dView(PhysicsSystem2dImpl &sys, phy_obj_handle_2d_t hndl)
            : _sys(sys), _hndl(hndl) {}

        void  setMass(float mass) override { data().mass = mass; }
        float mass() const override { return data().mass; }
        void  setPosition(const glm::vec2 &p) override {
            data().position = p;
            data().prev_position = p;
        }
        glm::vec2 position() const override { return data().position; }
        void      setVelocity(const glm::vec2 &v) override {
            // because velocity isn't explicit in verlet we will "trick"
            // velocity by setting the previous position to the current position
            // minus the velocity
            data().prev_position = data().position - (v * _sys.lastTimeStep());
        }
        glm::vec2 velocity() const override {
            // remember that velocity isn't explicit in verlet so we will
            // calculate it
            return data().prev_position - data().position;
        }
        void setAcceleration(const glm::vec2 &a) override {
            data().acceleration = a;
        }
        glm::vec2 acceleration() const override { return data().acceleration; }
        void      addForce(const glm::vec2 &f) override { data().force += f; }
        void      zeroForce() override { data().force = glm::vec2(0); }
        bool      isValid() const override {
            return _sys.isPhysicsComponentValid(_hndl);
        }
        void setStatic(bool is_static) override {
            setMass(is_static ? -1.0f : 1.0f);
        }
        bool isStatic() const override { return data().mass <= 0.0f; }
        phy_obj_handle_2d_t handle() const override { return _hndl; }

        VerletComponentData &data() { return _sys.physicsComponent(_hndl); }
        const VerletComponentData &data() const { return _sys.physicsComponent(_hndl); }

      private:
        PhysicsSystem2dImpl &_sys;
        phy_obj_handle_2d_t     _hndl;
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
            for (VerletComponentData &data : _physics_components) {
                if (data.mass <= 0) { // static object
                    continue;
                }
                glm::vec2 force = data.force;
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
    force_handle_2d_t addGlobalForce(const glm::vec2 &f) override {
        return _global_forces.add(f);
    }
    void removeGlobalForce(force_handle_2d_t id) override {
        _global_forces.remove(id);
    }
    std::optional<glm::vec2> getGlobalForce(force_handle_2d_t id) const override {
        return _global_forces.get(id);
    }

    const ComponentStore<glm::vec2> &globalForces() const override {
        return _global_forces;
    }

    phy_obj_handle_2d_t createPhysicsComponent() override {
        VerletComponentData data;
        return _physics_components.add(data);
    }

    void destroyPhysicsComponent(phy_obj_handle_2d_t hndl) override {
        _physics_components.remove(hndl);
    }

    std::unique_ptr<PhysicsComponent2d>
    getPhysicsComponentView(phy_obj_handle_2d_t hndl) override {
        auto result = _physics_components.get(hndl);
        if (result == std::nullopt) {
            return nullptr;
        }
        VerletComponentData &data = result->get();
        return std::make_unique<PhysicsComponent2dView>(*this, hndl);
    }

    bool isPhysicsComponentValid(phy_obj_handle_2d_t hndl) const override {
        return _physics_components.get(hndl) != std::nullopt;
    }

    float lastTimeStep() const { return _last_time_step; }

    ComponentStore<VerletComponentData> &physicsComponents() {
        return _physics_components;
    }

    VerletComponentData &physicsComponent(phy_obj_handle_2d_t hndl) {
        return _physics_components.get(hndl)->get();
    }

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