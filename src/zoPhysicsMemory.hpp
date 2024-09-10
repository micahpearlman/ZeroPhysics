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
#include <optional>
namespace zo {

template <typename T> class ComponentStore {
  public:
    using handle_t = uint64_t;
    
    /**
     * @brief Add a component to the ComponentStore.
     * 
     * @param component The component to add.
     * @return The handle of the added component.
     */
    handle_t add(const T& component) {
        _components.push_back(component);
        _handle_to_idx[_next_handle] = _components.size() - 1;
        return _next_handle++;
    }

    /**
     * @brief Removes an element from the container based on the given handle.
     * 
     * If the handle does not exist in the container, this function does nothing.
     * 
     * @param hndl The handle of the element to be removed.
     */
    void remove(handle_t hndl) {
        if (_handle_to_idx.contains(hndl) == false) {
            return;
        }
        size_t idx = _handle_to_idx[hndl];
        size_t last_idx = _components.size() - 1;

        // update the component array
        std::swap(_components[idx], _components[last_idx]);

        // update the handle to index map
        std::swap(_handle_to_idx[hndl], _handle_to_idx[last_idx]);

        // remove the last element as it is now the element we want to remove
        _components.pop_back();

        // remove from map
        _handle_to_idx.erase(hndl);
    }

    /**
     * Retrieves the value associated with the given handle ID.
     *
     * @param hndl The handle ID to retrieve the value for.
     * @return An optional containing the value associated with the handle ID if it exists, otherwise std::nullopt.
     */
    std::optional<std::reference_wrapper<const T>> get(handle_t hndl) const {
        if (_handle_to_idx.contains(hndl)) {
            uint32_t idx = _handle_to_idx.at(hndl);
            return std::cref(_components[idx]);
        }
        return std::nullopt;
    }

    /**
     * Retrieves the value associated with the given handle ID.
     *
     * @param hndl The handle ID to retrieve the value for.
     * @return An optional containing the value associated with the handle ID if it exists, otherwise std::nullopt.
     */
    std::optional<std::reference_wrapper<T>> get(handle_t hndl) {
        if (_handle_to_idx.contains(hndl)) {
            uint32_t idx = _handle_to_idx.at(hndl);
            return std::ref(_components[idx]);
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