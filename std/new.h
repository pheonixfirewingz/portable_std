#ifndef NEW_H
#define NEW_H
#include <stddef.h>
namespace os
{
    void *operator_new(std::size_t size);
    void *operator_new_array(std::size_t size);
    void *operator_new_aligned(std::size_t size, std::size_t alignment);
    void operator_delete(void *ptr);
    void operator_delete_array(void *ptr);
}
// Global operator overloads
inline void *operator new(std::size_t size)
{
    return os::operator_new(size);
}

inline void *operator new[](std::size_t size)
{
    return os::operator_new_array(size);
}

inline void *operator new(std::size_t size, std::align_val_t alignment)
{
    return os::operator_new_aligned(size, static_cast<std::size_t>(alignment));
}

inline void *operator new[](std::size_t size, std::align_val_t alignment)
{
    std::size_t align = static_cast<std::size_t>(alignment);
    void *ptr = os::operator_new_aligned(size + sizeof(std::size_t), align);
    *static_cast<std::size_t *>(ptr) = size;
    return static_cast<char *>(ptr) + sizeof(std::size_t);
}

inline void operator delete(void *ptr) noexcept
{
    os::operator_delete(ptr);
}

inline void operator delete[](void *ptr) noexcept
{
    os::operator_delete_array(ptr);
}

inline void operator delete(void *ptr, std::size_t) noexcept
{
    os::operator_delete(ptr);
}

inline void operator delete[](void *ptr, std::size_t) noexcept
{
    os::operator_delete_array(ptr);
}

inline void operator delete(void *ptr, std::align_val_t) noexcept
{
    os::operator_delete(ptr);
}

inline void operator delete[](void *ptr, std::align_val_t) noexcept
{
    os::operator_delete_array(ptr);
}

// Placement new operators
inline void *operator new(std::size_t, void *ptr) noexcept
{
    return ptr;
}
inline void *operator new[](std::size_t, void *ptr) noexcept
{
    return ptr;
}
#endif