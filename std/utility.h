#ifndef UTILITY_H
#define UTILITY_H
#include <type_traits.h>
#include <iterator.h>
namespace std
{
template<class T> constexpr inline std::remove_reference_t<T> &&move(T &&t) noexcept
{
    return static_cast<std::remove_reference_t<T> &&>(t);
}

template<class T> constexpr T &&forward(std::remove_reference_t<T> &t) noexcept
{
    return static_cast<T &&>(t);
}

template<class T> constexpr T &&forward(std::remove_reference_t<T> &&t) noexcept
{
    return static_cast<T &&>(t);
}

template<class InputIt, class OutputIt> constexpr OutputIt copy(InputIt first, InputIt last, OutputIt d_first)
{
    for (; first != last; (void)++first, (void)++d_first)
        *d_first = *first;
    return d_first;
}

template<class InputIt, class OutputIt, class UnaryPred>
constexpr OutputIt copy_if(InputIt first, InputIt last, OutputIt d_first, UnaryPred pred)
{
    for (; first != last; ++first)
        if (pred(*first))
        {
            *d_first = *first;
            ++d_first;
        }
    return d_first;
}

template<class ForwardIt, class T = typename std::iterator_traits<ForwardIt>::value_type>
constexpr void fill(ForwardIt first, ForwardIt last, const T &value)
{
    for (; first != last; ++first)
        *first = value;
}
} // namespace std
#endif