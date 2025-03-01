#ifndef CONCEPTS_H
#define CONCEPTS_H
#include <type_traits.h>
#include <utility.h>
namespace std
{
namespace detail
{
template<class T, class U>
concept SameHelper = std::is_same_v<T, U>;
}

template<class T, class U>
concept same_as = detail::SameHelper<T, U> && detail::SameHelper<U, T>;

template<class From, class To>
concept convertible_to = std::is_convertible_v<From, To> && requires { static_cast<To>(std::declval<From>()); };

template< class T >
concept destructible = std::is_nothrow_destructible_v<T>;

template< class T, class... Args >
concept constructible_from = std::destructible<T> && std::is_constructible_v<T, Args...>;

template<typename T, typename U>
struct common_reference {
private:
    template<typename X, typename Y>
    static auto test(int) -> decltype(true ? declval<X>() : declval<Y>());
    template<typename, typename>
    static void test(...);
    
public:
    using type = decltype(test<T, U>(0));
};

template< class... T >
using common_reference_t = std::common_reference<T...>::type;

template< class T, class U >
concept common_reference_with =
    std::same_as<std::common_reference_t<T, U>, std::common_reference_t<U, T>> &&
    std::convertible_to<T, std::common_reference_t<T, U>> &&

    std::convertible_to<U, std::common_reference_t<T, U>>;


template<class LHS, class RHS>
concept assignable_from =
    std::is_lvalue_reference_v<LHS> &&
    std::common_reference_with<const std::remove_reference_t<LHS> &, const std::remove_reference_t<RHS> &> &&
    requires(LHS lhs, RHS &&rhs) {
        { lhs = std::forward<RHS>(rhs) } -> std::same_as<LHS>;
    };

template<class T>
concept movable =
    std::is_object_v<T> && std::move_constructible<T> && std::assignable_from<T &, T> && std::swappable<T>;

template<class T>
concept copy_constructible =
    std::move_constructible<T> && std::constructible_from<T, T &> && std::convertible_to<T &, T> &&
    std::constructible_from<T, const T &> && std::convertible_to<const T &, T> && std::constructible_from<T, const T> &&
    std::convertible_to<const T, T>;

template<class T>
concept copyable = std::copy_constructible<T> && std::movable<T> && std::assignable_from<T &, T &> &&
                   std::assignable_from<T &, const T &> && std::assignable_from<T &, const T>;
} // namespace std
#endif