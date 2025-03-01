/**
 * @file vector.h
 * @brief A simplified implementation of a dynamic array similar to std::vector.
 * @note This header is a part of the C++ standard library.
 * This vector implementation provides contiguous storage, dynamic resizing,
 * and random access to elements. It is modeled after the C++ standard container,
 * as described on cppreference:
 * https://en.cppreference.com/w/cpp/container/vector (associated with live page on feb 11, 2025)
 */

#ifndef VECTOR_H
#define VECTOR_H

#include <algorithm.h>
#include <initializer_list.h>
#include <iterator.h>
#include <stddef.h>
#include <stdexcept.h>
#include <type_traits.h>

namespace std
{

/**
 * @brief A dynamic array that stores elements contiguously.
 *
 * This template class provides a simplified version of the standard
 * vector container. Elements are stored in a contiguous block of memory
 * and the container supports dynamic resizing, random access, and efficient
 * insertion/removal at the end.
 *
 * @tparam T Type of elements stored in the vector. Must not be void.
 */
template<typename T> class vector final
{
    static_assert(!std::is_void<T>::value, "vector cannot be instantiated with void type");

  public:
    /// Element type.
    using value_type = T;
    /// Unsigned integral type.
    using size_type = size_t;
    /// Reference to an element.
    using reference = T &;
    /// Const reference to an element.
    using const_reference = const T &;
    /// Iterator type (pointer to T).
    using iterator = T *;
    /// Constant iterator type.
    using const_iterator = const T *;
    /// Reverse iterator type.
    using reverse_iterator = std::reverse_iterator<iterator>;
    /// Constant reverse iterator type.
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    /**
     * @brief Default constructor.
     *
     * Constructs an empty vector with no elements.
     */
    constexpr vector() = default;

    /**
     * @brief Constructs a vector with a specified number of copies of a value.
     *
     * @param count Number of elements to initialize.
     * @param value Value to assign to each element (default is a default-constructed T).
     *
     * If count is zero, no memory is allocated.
     */
    constexpr explicit vector(size_type count, const T &value = T()) noexcept
        : _size{count}
        , _capacity{count}
    {
        if (_size > 0)
        {
            _data = new T[_size];
            for (size_type i = 0; i < _size; i++)
            {
                _data[i] = value;
            }
            return;
        }
        _data = nullptr;
    }

    /**
     * @brief Copy constructor.
     *
     * Constructs a new vector as a copy of the provided vector.
     *
     * @param other The vector to copy.
     */
    constexpr vector(const vector &other) noexcept
        : _size{other._size}
        , _capacity{other._capacity}
    {
        if (_size > 0)
        {
            _data = new T[_size];
            for (size_type i = 0; i < _size; i++)
            {
                _data[i] = other._data[i];
            }
            return;
        }
        _data = nullptr;
    }

    /**
     * @brief Move constructor.
     *
     * Constructs a vector by transferring the resources from another vector.
     *
     * @param other The vector to move from.
     *
     * After the move, the other vector is left in a valid, empty state.
     */
    constexpr vector(vector &&other) noexcept
        : _size{other._size}
        , _capacity{other._capacity}
        , _data{other._data}
    {
        other._data = nullptr;
        other._size = 0;
        other._capacity = 0;
    }

    constexpr vector(std::initializer_list<T> list) noexcept
        : _size{list.size()}
        , _capacity{list.size()}
    {
        _data = new T[_size];
        size_type i = 0;
        for (auto item : list)
        {
            _data[i++] = item;
        }
    }

    /**
     * @brief Destructor.
     *
     * Releases all memory allocated for the vector.
     */
    ~vector()
    {
        if (_data != nullptr)
        {
            delete[] _data;
        }
    }

    /**
     * @brief Copy assignment operator.
     *
     * Replaces the contents with a copy of the contents of another vector.
     *
     * @param other The vector to copy from.
     * @return Reference to this vector.
     */
    constexpr vector &operator=(const vector &other) noexcept
    {
        if (this != &other)
        {
            vector temp(other);
            swap(temp);
        }
        return *this;
    }

    /**
     * @brief Move assignment operator.
     *
     * Replaces the contents by moving the contents from another vector.
     *
     * @param other The vector to move from.
     * @return Reference to this vector.
     */
    constexpr vector &operator=(vector &&other) noexcept
    {
        if (this != &other)
        {
            swap(other);
        }
        return *this;
    }

    constexpr vector &operator=(std::initializer_list<T> list) noexcept
    {
        vector temp(list);
        swap(temp);
        return *this;
    }

    // Capacity functions

    /**
     * @brief Returns the number of elements.
     *
     * @return The number of elements in the vector.
     */
    constexpr size_type size() const noexcept
    {
        return _size;
    }

    /**
     * @brief Returns the maximum number of elements that the vector can hold.
     *
     * @return The maximum number of elements that the vector can hold.
     */
    constexpr size_type max_size() const noexcept
    {
        return size_type(-1);
    }

    /**
     * @brief Returns the capacity of the vector.
     *
     * @return The number of elements that can be held in currently allocated storage.
     */
    constexpr size_type capacity() const noexcept
    {
        return _capacity;
    }

    /**
     * @brief Checks if the vector is empty.
     *
     * @return true if the vector contains no elements, false otherwise.
     */
    constexpr bool empty() const noexcept
    {
        return _size == 0;
    }

    /**
     * @brief Reserves storage.
     *
     * Increases the capacity of the vector to at least new_cap. If new_cap is greater than the current capacity,
     * additional memory is allocated.
     *
     * @param new_cap The new capacity to reserve.
     */
    void reserve(size_type new_cap) noexcept
    {
        ensure_capacity(new_cap);
    }

    /**
     * @brief Shrinks the capacity to fit the current size.
     *
     * Requests the container to reduce its capacity to fit its size.
     */
    void shrink_to_fit() noexcept
    {
        ensure_capacity(_size);
        _capacity = _size;
    }
    
    /**
     * @brief Provides unchecked access to the element at specified position.
     *
     * @param pos Position of the element.
     * @return Reference to the element at position pos.
     *
     * Calling operator[] on an out-of-range index results in undefined behavior.
     */
    reference operator[](size_type pos) noexcept
    {
        return at(pos);
    }

    /**
     * @brief Provides unchecked access to the element at specified position (const version).
     *
     * @param pos Position of the element.
     * @return Const reference to the element at position pos.
     *
     * Calling operator[] on an out-of-range index results in undefined behavior.
     */
    const_reference operator[](size_type pos) const noexcept
    {
        return at(pos);
    }

    /**
     * @brief Insert an element or a range of elements at specified position.
     *
     * @param position Position of the element to insert before.
     * @param x Element to insert.
     * @return Iterator pointing to the inserted element.
     *
     * If the vector's capacity is exhausted, its capacity is increased and a new block of memory is allocated.
     * The elements from position to the end of the vector are shifted to the right.
     * The new element is constructed in-place at the specified position.
     */
    constexpr iterator insert(const_iterator position, const T &x)
    {
        size_type pos_index = std::distance(cbegin(), position);
        ensure_capacity(_size + 1);
        std::move_backward(_data + pos_index, _data + _size, _data + _size + 1);
        _data[pos_index] = x;
        ++_size;
        return _data + pos_index;
    }

    /**
     * @brief Insert an element or a range of elements at specified position (move version).
     *
     * @param position Position of the element to insert before.
     * @param x Element to insert.
     * @return Iterator pointing to the inserted element.
     *
     * If the vector's capacity is exhausted, its capacity is increased and a new block of memory is allocated.
     * The elements from position to the end of the vector are shifted to the right.
     * The new element is move-constructed in-place at the specified position.
     */
    constexpr iterator insert(const_iterator position, T &&x)
    {
        size_type pos_index = static_cast<size_type>(std::distance(cbegin(), position));
        ensure_capacity(_size + 1);
        std::move_backward(_data + pos_index, _data + _size, _data + _size + 1);
        _data[pos_index] = std::move(x);
        ++_size;
        return _data + pos_index;
    }

    /**
     * @brief Insert a range of elements at specified position.
     *
     * @param position Position of the element to insert before.
     * @param n Number of elements to insert.
     * @param x Element to insert.
     * @return Iterator pointing to the first inserted element.
     *
     * If the vector's capacity is exhausted, its capacity is increased and a new block of memory is allocated.
     * The elements from position to the end of the vector are shifted to the right.
     * The new elements are copy-constructed in-place at the specified position.
     */
    constexpr iterator insert(const_iterator position, size_type n, const T &x)
    {
        size_type pos_index = std::distance(cbegin(), position);
        ensure_capacity(_size + n);
        std::move_backward(_data + pos_index, _data + _size, _data + _size + n);
        std::fill(_data + pos_index, _data + pos_index + n, x);
        _size += n;
        return _data + pos_index;
    }

    /**
     * @brief Insert a range of elements at specified position.
     *
     * @param position Position of the element to insert before.
     * @param first Beginning of the range of elements to insert.
     * @param last End of the range of elements to insert.
     * @return Iterator pointing to the first inserted element.
     *
     * If the vector's capacity is exhausted, its capacity is increased and a new block of memory is allocated.
     * The elements from position to the end of the vector are shifted to the right.
     * The new elements are copy-constructed in-place at the specified position.
     */
    template<class InputIter> constexpr iterator insert(const_iterator position, InputIter first, InputIter last)
    {
        size_type pos_index = std::distance(cbegin(), position);
        size_type n = std::distance(first, last);
        ensure_capacity(_size + n);
        std::move_backward(_data + pos_index, _data + _size, _data + _size + n);
        std::copy(first, last, _data + pos_index);
        _size += n;
        return _data + pos_index;
    }

    /**
     * @brief Insert a range of elements at specified position from an initializer list.
     *
     * @param position Position of the element to insert before.
     * @param il Initializer list of elements to insert.
     * @return Iterator pointing to the first inserted element.
     *
     * If the vector's capacity is exhausted, its capacity is increased and a new block of memory is allocated.
     * The elements from position to the end of the vector are shifted to the right.
     * The new elements are copy-constructed in-place at the specified position.
     */
    constexpr iterator insert(const_iterator position, std::initializer_list<T> il)
    {
        return insert(position, il.begin(), il.end());
    }

    /**
     * @brief Erase an element at specified position.
     *
     * @param pos Position of the element to erase.
     *
     * The elements from pos to the end of the vector are shifted to the left.
     * The size of the vector is decremented.
     */
    void erase(size_type pos)
    {
        for (size_type i = pos; i < _size - 1; i++)
        {
            _data[i] = _data[i + 1];
        }
        _size--;
    }

    /**
     * @brief Provides access to the element at specified position with bounds checking.
     *
     * @param pos Position of the element.
     * @return Reference to the element at position pos.
     *
     * If pos is not within the range of the vector, a fatal error is logged.
     */
    reference at(size_type pos)
    {
        if (pos >= _size)
        {
            throw std::out_of_range();
        }
        return _data[pos];
    }

    /**
     * @brief Provides access to the element at specified position with bounds checking (const version).
     *
     * @param pos Position of the element.
     * @return Const reference to the element at position pos.
     *
     * If pos is not within the range of the vector, a fatal error is logged.
     */
    const_reference at(size_type pos) const
    {
        if (pos >= _size)
        {
            throw std::out_of_range();
        }
        return _data[pos];
    }

    /**
     * @brief Returns an iterator pointing to the first element in the vector.
     *
     * @return Iterator pointing to the first element in the vector.
     *
     * If the vector is empty, the returned iterator will be equal to end().
     */
    constexpr iterator begin() noexcept
    {
        return iterator(_data);
    }

    /**
     * @brief Returns a constant iterator pointing to the first element in the vector.
     *
     * @return Constant iterator pointing to the first element in the vector.
     *
     * If the vector is empty, the returned iterator will be equal to end().
     */
    constexpr const_iterator begin() const noexcept
    {
        return const_iterator(_data);
    }

    /**
     * @brief Returns an iterator pointing to the after the last element in the vector.
     *
     * @return Iterator pointing to the after the last element in the vector.
     *
     * This iterator serves as a placeholder for the end of the vector.
     */
    constexpr iterator end() noexcept
    {
        return iterator(_data + _size);
    }

    /**
     * @brief Returns a constant iterator pointing to the after the last element in the vector.
     *
     * @return Constant iterator pointing to the after the last element in the vector.
     *
     * This iterator serves as a placeholder for the end of the vector.
     */
    constexpr const_iterator end() const noexcept
    {
        return const_iterator(_data + _size);
    }

    /**
     * @brief Returns a reverse iterator pointing to the last element in the vector.
     *
     * @return Reverse iterator pointing to the last element in the vector.
     *
     * If the vector is empty, the returned iterator will be equal to rend().
     */
    constexpr reverse_iterator rbegin() noexcept
    {
        return reverse_iterator(end());
    }

    /**
     * @brief Returns a constant reverse iterator pointing to the last element in the vector.
     *
     * @return Constant reverse iterator pointing to the last element in the vector.
     *
     * If the vector is empty, the returned iterator will be equal to rend().
     */
    constexpr const_reverse_iterator rbegin() const noexcept
    {
        return const_reverse_iterator(end());
    }

    /**
     * @brief Returns a reverse iterator pointing to the before the first element in the vector.
     *
     * @return Reverse iterator pointing to the before the first element in the vector.
     *
     * This iterator serves as a placeholder for the beginning of the vector.
     */
    constexpr reverse_iterator rend() noexcept
    {
        return reverse_iterator(begin());
    }

    /**
     * @brief Returns a constant reverse iterator pointing to the before the first element in the vector.
     *
     * @return Constant reverse iterator pointing to the before the first element in the vector.
     *
     * This iterator serves as a placeholder for the beginning of the vector.
     */
    constexpr const_reverse_iterator rend() const noexcept
    {
        return const_reverse_iterator(begin());
    }

    /**
     * @brief Returns a constant iterator pointing to the first element in the vector.
     *
     * @return Constant iterator pointing to the first element in the vector.
     *
     * If the vector is empty, the returned iterator will be equal to cend().
     */
    constexpr const_iterator cbegin() const noexcept
    {
        return begin();
    }

    /**
     * @brief Returns a constant iterator pointing to the after the last element in the vector.
     *
     * @return Constant iterator pointing to the after the last element in the vector.
     *
     * This iterator serves as a placeholder for the end of the vector.
     */
    constexpr const_iterator cend() const noexcept
    {
        return end();
    }

    /**
     * @brief Returns a constant reverse iterator pointing to the last element in the vector.
     *
     * @return Constant reverse iterator pointing to the last element in the vector.
     *
     * If the vector is empty, the returned iterator will be equal to crend().
     */
    constexpr const_reverse_iterator crbegin() const noexcept
    {
        return rbegin();
    }

    /**
     * @brief Returns a constant reverse iterator pointing to the before the first element in the vector.
     *
     * @return Constant reverse iterator pointing to the before the first element in the vector.
     *
     * This iterator serves as a placeholder for the beginning of the vector.
     */
    constexpr const_reverse_iterator crend() const noexcept
    {
        return rend();
    }

    /**
     * @brief Returns a reference to the first element.
     *
     * @return Reference to the first element in the vector.
     *
     * Calling front() on an empty vector results in undefined behavior.
     */
    reference front() noexcept
    {
        return at(0);
    }

    /**
     * @brief Returns a const reference to the first element.
     *
     * @return Const reference to the first element in the vector.
     *
     * Calling front() on an empty vector results in undefined behavior.
     */
    const_reference front() const noexcept
    {
        return at(0);
    }

    /**
     * @brief Returns a reference to the last element.
     *
     * @return Reference to the last element in the vector.
     *
     * Calling back() on an empty vector results in undefined behavior.
     */
    reference back() noexcept
    {
        return at(_size - 1);
    }

    /**
     * @brief Returns a const reference to the last element.
     *
     * @return Const reference to the last element in the vector.
     *
     * Calling back() on an empty vector results in undefined behavior.
     */
    const_reference back() const noexcept
    {
        return at(_size - 1);
    }

    /**
     * @brief Returns a pointer to the underlying array.
     *
     * @return Pointer to the first element stored in the vector.
     */
    T *data() noexcept
    {
        return _data;
    }

    /**
     * @brief Returns a const pointer to the underlying array.
     *
     * @return Const pointer to the first element stored in the vector.
     */
    const T *data() const noexcept
    {
        return _data;
    }

    // Modifiers

    /**
     * @brief Clears the contents of the vector.
     *
     * Erases all elements and deallocates memory. After a call to clear(),
     * size() and capacity() both return 0.
     */
    void clear() noexcept
    {
        _size = 0;
        _capacity = 0;
        if (_data != nullptr)
        {
            delete[] _data;
        }
        _data = nullptr;
    }

    /**
     * @brief Appends an element to the end of the vector.
     *
     * @param value The element to append.
     *
     * The vector's size is increased by one. If necessary, the vector's capacity is expanded.
     */
    void push_back(const T &value)
    {
        ensure_capacity(_size + 1);
        _data[_size++] = value;
    }

    /**
     * @brief Constructs an element in-place at the end of the vector.
     *
     * @tparam Args Parameter pack for the constructor of T.
     * @param args Arguments forwarded to the constructor of T.
     *
     * If there is insufficient capacity, the vector is reallocated.
     */
    template<typename... Args> void emplace_back(Args &&...args)
    {
        if (_size == _capacity)
        {
            reserve(_capacity == 0 ? 1 : _capacity * 2);
        }
        new (_data + _size) T(static_cast<Args &&>(args)...);
        ++_size;
    }

    /**
     * @brief Appends an element to the end of the vector using move semantics.
     *
     * @param value The element to append.
     *
     * The vector's size is increased by one. If necessary, the vector's capacity is expanded.
     */
    void push_back(T &&value)
    {
        ensure_capacity(_size + 1);
        _data[_size++] = value;
    }

    /**
     * @brief Removes the last element from the vector.
     *
     * Reduces the size by one. Calling pop_back() on an empty vector results in undefined behavior.
     */
    void pop_back()
    {
        if (_size > 0)
        {
            --_size;
        }
    }

    /**
     * @brief Resizes the vector to contain count elements.
     *
     * If count is greater than the current size, additional elements are appended and initialized
     * with a copy of value. If count is less than the current size, the vector is reduced to its first count elements.
     *
     * @param count The new size of the vector.
     * @param value The value to initialize new elements with (if any).
     */
    void resize(size_type count, const T &value = T())
    {
        if (count > _size)
        {
            while (_size < count)
            {
                push_back(value);
            }
        }
        else
        {
            while (_size > count)
            {
                pop_back();
            }
        }
    }

    /**
     * @brief Swaps the contents with another vector.
     *
     * @param other The vector to swap with.
     *
     * Exchanges the underlying data pointers and the size/capacity values.
     */
    void swap(vector &other) noexcept
    {
        std::swap(_data, other._data);
        size_type temp_size = _size;
        size_type temp_capacity = _capacity;

        _size = other._size;
        _capacity = other._capacity;

        other._size = temp_size;
        other._capacity = temp_capacity;
    }

  private:
    T *_data = nullptr;      ///< Pointer to the dynamically allocated array of elements.
    size_type _size = 0;     ///< The number of elements in the vector.
    size_type _capacity = 0; ///< The current allocated capacity.

    /**
     * @brief Ensures that the vector has at least new_capacity storage.
     *
     * If new_capacity is greater than the current capacity, a new block of memory is allocated,
     * the existing elements are copied using memcpy (assuming trivially copyable types),
     * and the old memory is released.
     *
     * @param new_capacity The required minimum capacity.
     */
    void ensure_capacity(size_type new_capacity) noexcept
    {
        if (new_capacity > _capacity)
        {
            size_type new_cap = (_capacity == 0) ? 8 : _capacity * 2;
            if (new_cap < new_capacity)
            {
                new_cap = new_capacity;
            }

            T *new_ptr = new T[new_cap];
            if (_data != nullptr && _size > 0)
            {
                memcpy(new_ptr, _data, _size * sizeof(T));
            }
            delete[] _data;
            _data = new_ptr;
            _capacity = new_cap;
        }
    }
};
} // namespace std
#endif
