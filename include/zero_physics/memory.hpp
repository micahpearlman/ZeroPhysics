/**
 * @file memory.hpp
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
 * @brief A O(1) create/destroy memory pool.
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
        size_t idx = ptrToIdx(ptr);
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

    void deallocate(size_t idx) {
        if (idx > _pool_size || idx < 0) {
            return; // out of bounds
        }

        deallocate(idxToPtr(idx));

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

    /**
     * @brief Get the index of the object in the memory pool.
     *
     * @param ptr A pointer to the object.
     * @return size_t The index of the object in the memory pool.
     */
    size_t ptrToIdx(T *ptr) {
        return reinterpret_cast<MemoryPoolElement *>(ptr) - &_pool.front();
    }

    /**
     * @brief Get the object at the given index.
     *
     * @param idx The index of the object.
     * @return T* A pointer to the object.
     */
    T *idxToPtr(size_t idx) { return &_pool[idx].object; }
    const T *idxToPtr(size_t idx) const { return &_pool[idx].object; }
    T &operator[](size_t idx) { return _pool[idx].object; }
    const T &operator[](size_t idx) const { return _pool[idx].object; }

  private:

    /// @brief A union to store the object *OR* the next free index.
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
 * to store components as it provides data locality and O(1) access, addition
 * and removal of elements, with the trade off of having extra memory overhead
 * for mapping elements to handles.
 * Use this when you need to store a large number of components and iterate over
 * them sequentially very fast.
 *
 * @tparam T The type of component to store.
 */
template <typename T> class ComponentStore {
  public:
    using handle_t = uint32_t;

    /**
     * @brief Add a component to the ComponentStore.
     *
     * @param component The component to add.
     * @return The handle of the added component.
     */
    handle_t add(const T &component) {
        _components.push_back(component);
        _handle_to_idx[_next_handle] = _components.size() - 1;
        _idx_to_handle[_components.size() - 1] = _next_handle;
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
    void remove(handle_t remove_hndl) {
        if (_handle_to_idx.contains(remove_hndl) == false) {
            return;
        }
        size_t remove_idx = _handle_to_idx[remove_hndl];
        size_t last_idx = _components.size() - 1;
        handle_t last_hndl = _idx_to_handle[last_idx];

        // delete the handle from the handle to index map
        _handle_to_idx.erase(remove_hndl);

        // delete the index from the index to handle map
        _idx_to_handle.erase(remove_idx);

        if (remove_idx == last_idx) {
            // if the element to remove is the last element, just remove it
            _components.pop_back();
            return;
        }
        // update the component array by moving the last element to 
        // the removed element's position
        std::swap(_components[remove_idx], _components[last_idx]);

        // update the handle to index map
        // remember the last element's handle is now the removed element's handle
        _handle_to_idx[last_hndl] = remove_idx;

        // update the index to handle map
        // remember the last element's index is now the removed element's index
        _idx_to_handle[remove_idx] = last_hndl;


        // remove the last element as it is now the element we want to remove
        _components.pop_back();

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

    void clear() {
        _components.clear();
        _handle_to_idx.clear();
        _idx_to_handle.clear();
        _next_handle = 0;
    }

    // STL iterator support
    std::vector<T>::iterator begin() { return _components.begin(); }
    std::vector<T>::iterator end() { return _components.end(); }
    std::vector<T>::iterator begin() const { return _components.begin(); }
    std::vector<T>::iterator end() const { return _components.end(); }


    // accessors to the underlying data
    size_t size() const { return _components.size(); }
    T& at(int idx) { return _components[idx]; }
    const T& at(int idx) const { return _components[idx]; } 

  private:
    std::vector<T>                         _components;
    std::unordered_map<handle_t, uint32_t> _handle_to_idx;
    std::unordered_map<uint32_t, handle_t> _idx_to_handle;
    handle_t                               _next_handle = 0;
};
}; // namespace zo

#endif // __zoMemory_h__