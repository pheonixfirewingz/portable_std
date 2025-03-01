/*!
 * @file algorithm.h
 * @brief Utility algorithms for the Luna Voxel Engine
 * @namespace std
 */
#ifndef ALGORITHM_H
#define ALGORITHM_H
#include <utility.h>
namespace std
{
/*!
 * @brief Clamps a value between a minimum and maximum range
 * @tparam T The type of the value to clamp
 * @param value The value to clamp
 * @param min The minimum allowed value
 * @param max The maximum allowed value
 * @return The clamped value
 * @note This function is constexpr and noexcept
 */
template<typename T> constexpr T clamp(T value, T min, T max) noexcept
{
    return value < min ? min : (value > max ? max : value);
}
/*!
 * @brief Performs linear interpolation between two values
 * @tparam T The type of the values to interpolate
 * @param a Starting value
 * @param b Ending value
 * @param t Interpolation factor (0.0 to 1.0)
 * @return The interpolated value
 * @note This function is constexpr and noexcept
 */
template<typename T> constexpr T lerp(T a, T b, float t) noexcept
{
    return a + (b - a) * t;
}
/*!
 * @brief Returns the minimum of two values
 * @tparam T The type of the values to compare
 * @param a First value
 * @param b Second value
 * @return The smaller of the two values
 * @note This function is constexpr and noexcept
 */
template<typename T> constexpr T min(T a, T b) noexcept
{
    return a < b ? a : b;
}
/*!
 * @brief Returns the maximum of two values
 * @tparam T The type of the values to compare
 * @param a First value
 * @param b Second value
 * @return The larger of the two values
 * @note This function is constexpr and noexcept
 */
template<typename T> constexpr T max(T a, T b) noexcept
{
    return a > b ? a : b;
}
/*!
 * @brief Returns the absolute value
 * @tparam T The type of the value
 * @param a The value to get the absolute value of
 * @return The absolute value
 * @note This function is constexpr and noexcept
 */
template<typename T> constexpr T abs(T a) noexcept
{
    return a < 0 ? -a : a;
}
/*!
 * @brief Returns the sign of a value (-1 or 1)
 * @tparam T The type of the value
 * @param a The value to get the sign of
 * @return -1 if negative, 1 if positive or zero
 * @note This function is constexpr and noexcept
 */
template<typename T> constexpr T sign(T a) noexcept
{
    return a < 0 ? -1 : 1;
}
/*!
 * @brief Swaps two values
 * @tparam T The type of the values to swap
 * @param a First value
 * @param b Second value
 * @note This function is constexpr and noexcept
 */
template<typename T> constexpr void swap(T &a, T &b) noexcept
{
    T tmp = a;
    a = b;
    b = tmp;
}

/*!
 * @brief Move elements from one range to another, starting from the end
 * @details This function is a version of std::move_backward that is constexpr
 *          and noexcept.
 * @tparam iter_one The type of the beginning and end of the range to move
 * @tparam iter_two The type of the beginning of the range to move into
 * @param first The beginning of the range to move
 * @param last The end of the range to move
 * @param d_last The beginning of the range to move into
 * @return An iterator pointing to the element after the last element moved
 * @note This function is constexpr and noexcept
 * @see https://en.cppreference.com/w/cpp/algorithm/move_backward
 */
template<class iter_one, class iter_two>
iter_two constexpr inline move_backward(iter_one first, iter_one last, iter_two d_last)
{
    // Move elements from the end
    while (first != last)
    {
        // Move the element at the end of the range
        *(--d_last) = std::move(*(--last));
    }
 
    // Return the iterator pointing to the element after the last element moved
    return d_last;
}

/*!
 * @brief Partitions a range around a pivot element
 * @tparam Iterator The iterator type
 * @tparam Compare The comparison function type
 * @param beg Beginning iterator
 * @param end End iterator
 * @param cmp Comparison function
 * @return Iterator to the pivot element
 */
template<typename Iterator, typename Compare> Iterator partition(Iterator beg, Iterator end, Compare cmp)
{
    // Using the last element as the pivot
    Iterator pivot = end - 1;
    Iterator i = beg - 1;

    for (Iterator j = beg; j < pivot; ++j)
    {
        if (cmp(*j, *pivot))
        { // If element is smaller than the pivot
            ++i;
            swap(*i, *j); // Swap elements using custom my_swap
        }
    }

    // Place the pivot element in its correct sorted position
    swap(*(i + 1), *pivot);
    return i + 1; // Return the pivot's new position
}
/*!
 * @brief Performs quicksort on a range
 * @tparam Iterator The iterator type
 * @tparam Compare The comparison function type
 * @param beg Beginning iterator
 * @param end End iterator
 * @param cmp Comparison function
 */
template<typename Iterator, typename Compare> void quicksort(Iterator beg, Iterator end, Compare cmp)
{
    if (beg >= end)
        return;

    // Partition the range [beg, end) and get the pivot element's position
    Iterator pivot = partition(beg, end, cmp);

    // Recursively sort the two halves
    quicksort(beg, pivot, cmp);     // Sort the left half
    quicksort(pivot + 1, end, cmp); // Sort the right half
}
/*!
 * @brief Compares two memory blocks
 * @param aptr Pointer to first memory block
 * @param bptr Pointer to second memory block
 * @param size Number of bytes to compare
 * @return -1 if first block is less, 1 if second block is less, 0 if equal
 */
inline int memcmp(const void *aptr, const void *bptr, unsigned long size)
{
    const unsigned char *a = static_cast<const unsigned char *>(aptr);
    const unsigned char *b = static_cast<const unsigned char *>(bptr);
    for (unsigned long i = 0; i < size; i++)
    {
        if (a[i] < b[i])
            return -1;
        else if (b[i] < a[i])
            return 1;
    }
    return 0;
}
/*!
 * @brief Copies memory from source to destination
 * @param dstptr Destination pointer
 * @param srcptr Source pointer
 * @param size Number of bytes to copy
 * @return Pointer to the destination memory
 */
inline void *memcpy(void *dstptr, const void *srcptr, unsigned long size)
{
    if (!dstptr || !srcptr)
        return nullptr; // Check for null pointers
    unsigned char *dst = static_cast<unsigned char *>(dstptr);
    const unsigned char *src = static_cast<const unsigned char *>(srcptr);
    for (unsigned long i = 0; i < size; i++)
        dst[i] = src[i];
    return dstptr;
}
/*!
 * @brief Sets a block of memory to a specific value
 * @param bufptr Pointer to the memory block
 * @param value Value to set
 * @param size Number of bytes to set
 * @return Pointer to the memory block
 */
inline void *memset(void *bufptr, int value, unsigned long size)
{
    unsigned char *buf = static_cast<unsigned char *>(bufptr);
    for (unsigned long i = 0; i < size; i++)
        buf[i] = static_cast<unsigned char>(value);
    return bufptr;
}
/*!
 * @brief Moves memory from source to destination, handling overlapping regions
 * @param dstptr Destination pointer
 * @param srcptr Source pointer
 * @param size Number of bytes to move
 * @return Pointer to the destination memory
 */
inline void *memmove(void *dstptr, const void *srcptr, unsigned long size)
{
    unsigned char *dst = static_cast<unsigned char *>(dstptr);
    const unsigned char *src = static_cast<const unsigned char *>(srcptr);
    if (dst < src)
    {
        for (unsigned long i = 0; i < size; i++)
            dst[i] = src[i];
    }
    else
    {
        for (unsigned long i = size; i != 0; i--)
            dst[i - 1] = src[i - 1];
    }
    return dstptr;
}
} // namespace std
#endif
