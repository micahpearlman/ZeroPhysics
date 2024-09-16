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

/**
 * @brief A fast memory pool.
 *
 * @tparam T The type of object to allocate.
 */
template <typename T> class MemoryPool {

  public:
    using value_type = T;
    /**
     * @brief Construct a new Memory Pool object
     *
     * @param chunk_size The size of each element of memory to allocate.
     */
    MemoryPool(size_t pool_size = 1024) : _pool_size(pool_size) {
        // allocate the pool
        _pool.resize(_pool_size);
        _next = 0;

        // generate the free list by setting each element to the next index
        for (size_t i = 0; i < _pool_size; i++) {
            _pool[i].next = i + 1;
        }
        _pool[_pool_size - 1].next = size_t(-1);
    }

    ~MemoryPool() {
        // destroy all allocated objects
        for (size_t i = 0; i < _pool_size; i++) {
            if (i >= _next && i != size_t(-1)) {
                _pool[i].object.~T();
            }
        }
    }

    /**
     * @brief Allocate a new object from the memory pool.
     *
     * @return T* A pointer to the allocated object.
     */
    T *allocate() {
        if (_next == -1) {
            return nullptr;
        }
        size_t free_idx = _next;
        _next = _pool[free_idx].next;

        // placement new to construct object at free index
        T *new_obj = reinterpret_cast<T *>(&_pool[free_idx].object);
        new (new_obj) T();

        return new_obj;
    }

    /**
     * @brief Allocate a range of objects from the memory pool.
     *
     * @param count The number of objects to allocate.
     * @return T* A pointer to the first allocated object.
     */
    T *allocate(size_t count) {
        T *ptr = allocate();
        for (size_t i = 1; i < count; ++i) {
            allocate();
        }
        return ptr;
    }

    /**
     * @brief Deallocate an object from the memory pool.
     *
     * @param ptr A pointer to the object to deallocate.
     */
    void deallocate(T *ptr) {
        int idx = reinterpret_cast<MemoryPoolElement*>(ptr) - &_pool.front();
        if (idx > _pool_size || idx < 0) {
            return; // out of bounds
        }

        // call the destructor
        _pool[idx].object.~T();

        // add the index to the free list
        _pool[idx].next = _next;
        _next = idx;
    }

    /**
     * @brief Deallocate a range of objects from the memory pool.
     *
     * @param ptr A pointer to the first object to deallocate.
     * @param count The number of objects to deallocate.
     */
    void deallocate(T *ptr, size_t count) {
        for (size_t i = 0; i < count; ++i) {
            deallocate(ptr + (i * sizeof(T)));
        }
    }

    /**
     * @brief Constructs an object in place at the given pointer.
     *
     * @tparam U
     * @tparam Args
     * @param ptr
     * @param args
     */
    template <typename U, typename... Args>
    void construct(U *ptr, Args &&...args) {
        new (ptr) U(std::forward<Args>(args)...);
    }

    /**
     * @brief Destroy an object in place at the given pointer.
     *
     * @tparam U
     * @param ptr
     */
    template <typename U> void destroy(U *ptr) { ptr->~U(); }

  private:
    union alignas(std::max_align_t) MemoryPoolElement {
        T      object;
        size_t next;
        MemoryPoolElement() : next(0) {}
    };
    size_t _pool_size = 0;
    // the memory pool with T being the object and size_t being the next free
    // index
    std::vector<MemoryPoolElement> _pool;
    size_t                         _next = 0;
};

/**
 * @brief A component store that uses a vector to store components and a map to
 * store the handle to index mapping. This is intended to be be an efficient way
 * to store components as it provides data locality and O(1) access to
 * components. Adding and removing components is O(1) as well.
 *
 * @tparam T The type of component to store.
 */
template <typename T> class ComponentStore {
  public:
    using handle_t = uint64_t;

    /**
     * @brief Add a component to the ComponentStore.
     *
     * @param component The component to add.
     * @return The handle of the added component.
     */
    handle_t add(const T &component) {
        _components.push_back(component);
        _handle_to_idx[_next_handle] = _components.size() - 1;
        return _next_handle++;
    }

    /**
     * @brief Removes an element from the container based on the given handle.
     *
     * If the handle does not exist in the container, this function does
     * nothing.
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
     * @return An optional containing the value associated with the handle ID if
     * it exists, otherwise std::nullopt.
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
     * @return An optional containing the value associated with the handle ID if
     * it exists, otherwise std::nullopt.
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
    std::vector<T>                         _components;
    std::unordered_map<uint32_t, uint32_t> _handle_to_idx;
    uint32_t                               _next_handle = 0;
};
}; // namespace zo

#endif // __zoMemory_h__