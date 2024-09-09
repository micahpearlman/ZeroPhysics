/**
 * @file zoPhysicsMemory.hpp
 * @author Micah Pearlman (micahpearlman@gmail.com)
 * @brief
 * @version 0.1
 * @date 2024-09-09
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef __zoMemory_h__
#define __zoMemory_h__
#include <vector>
#include <unordered_map>
namespace zo {

template <typename T> class ComponentRegistry {
  public:
    using handle_t = uint32_t;
    
    handle_t add(T component) {
        _components.push_back(component);
        _handle_to_idx[_next_handle] = _components.size() - 1;
        return _next_handle++;
    }

    void remove(handle_t id) {
        if (_handle_to_idx.contains(id) == false) {
            return;
        }
        size_t idx = _handle_to_idx[id];
        size_t last_idx = _components.size() - 1;

        std::swap(_components[idx], _components[last_idx]);
        std::swap(_handle_to_idx[id], _handle_to_idx[last_idx]);
        _components.pop_back();
        _handle_to_idx[_handle_to_idx[id]] = idx;
        _handle_to_idx.erase(id);
    }

    std::optional<T> get(handle_t id) const {
        if (_handle_to_idx.contains(id)) {
            uint32_t idx = _handle_to_idx.at(id);
            return _components[idx];
        }
        return std::nullopt;
    }

    // Iterator support
    auto begin() { return _components.begin(); }
    auto end() { return _components.end(); }
    auto begin() const { return _components.begin(); }
    auto end() const { return _components.end(); }    

  private:
    std::vector<T> _components;
    std::unordered_map<uint32_t, uint32_t> _handle_to_idx;
    uint32_t _next_handle = 0;
};
}; // namespace zo

#endif // __zoMemory_h__