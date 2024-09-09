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
    virtual ~PhysicsSystem2dImpl() = default;
    void update(float dt) override {
        // do nothing
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

    const ComponentRegistry<glm::vec2>& globalForces() const override {
        return _global_forces;
    }

  protected:
    ComponentRegistry<glm::vec2> _global_forces;
};

std::shared_ptr<PhysicsSystem2d> PhysicsSystem2d::create() {
    PhysicsSystem2dImpl *impl = new PhysicsSystem2dImpl();
    return std::shared_ptr<PhysicsSystem2d>(impl);
}

} // namespace zo