#ifndef IS_TYPE_H
#define IS_TYPE_H
#include <stddef.h>
namespace std
{
struct true_type
{
    static constexpr bool value = true;
    constexpr operator bool() const noexcept
    {
        return value;
    }
};

struct false_type
{
    static constexpr bool value = false;
    constexpr operator bool() const noexcept
    {
        return value;
    }
};

template<class T> struct is_array : std::false_type
{
};

template<class T> struct is_array<T[]> : std::true_type
{
};

template<class T, std::size_t N> struct is_array<T[N]> : std::true_type
{
};

template<typename T> struct is_union
{
    /*!
     * @note This implementation uses a common compiler intrinsic (available in GCC, Clang, and many versions of MSVC)
     * called __is_union,
     */
    static constexpr bool value = __is_union(T);
};

template<typename T> inline constexpr bool is_union_v = is_union<T>::value;

template<typename T> struct is_void : false_type
{
};
template<> struct is_void<void> : true_type
{
};
template<> struct is_void<const void> : true_type
{
};
template<> struct is_void<volatile void> : true_type
{
};
template<> struct is_void<const volatile void> : true_type
{
};

template<typename T> inline constexpr bool is_void_v = is_void<T>::value;

template<typename T> struct is_integral : false_type
{
};
template<> struct is_integral<bool> : true_type
{
};
template<> struct is_integral<char> : true_type
{
};
template<> struct is_integral<signed char> : true_type
{
};
template<> struct is_integral<unsigned char> : true_type
{
};
template<> struct is_integral<short> : true_type
{
};
template<> struct is_integral<unsigned short> : true_type
{
};
template<> struct is_integral<int> : true_type
{
};
template<> struct is_integral<unsigned int> : true_type
{
};
template<> struct is_integral<long> : true_type
{
};
template<> struct is_integral<unsigned long> : true_type
{
};
template<> struct is_integral<long long> : true_type
{
};
template<> struct is_integral<unsigned long long> : true_type
{
};

template<typename T> inline constexpr bool is_integral_v = is_integral<T>::value;

template<typename T> struct is_floating_point : false_type
{
};
template<> struct is_floating_point<float> : true_type
{
};
template<> struct is_floating_point<double> : true_type
{
};
template<> struct is_floating_point<long double> : true_type
{
};

template<typename T> inline constexpr bool is_floating_point_v = is_floating_point<T>::value;

template<typename T, typename U> struct is_same : false_type
{
};
template<typename T> struct is_same<T, T> : true_type
{
};

template<typename T, typename U> inline constexpr bool is_same_v = is_same<T, U>::value;

template<class T>
struct is_arithmetic : std::integral_constant<bool, std::is_integral<T>::value || std::is_floating_point<T>::value>
{
};

template<class T> constexpr bool is_arithmetic_v = is_arithmetic<T>::value;

template<bool B, typename T = void> struct enable_if
{
};
template<typename T> struct enable_if<true, T>
{
    using type = T;
};

template<bool B, typename T = void> using enable_if_t = typename enable_if<B, T>::type;

template<typename T> struct remove_cv
{
    using type = T;
};
template<typename T> struct remove_cv<const T>
{
    using type = T;
};
template<typename T> struct remove_cv<volatile T>
{
    using type = T;
};
template<typename T> struct remove_cv<const volatile T>
{
    using type = T;
};

template<typename T> using remove_cv_t = typename remove_cv<T>::type;

template<typename T> struct is_pointer : false_type
{
};
template<typename T> struct is_pointer<T *> : true_type
{
};
template<typename T> struct is_pointer<T *const> : true_type
{
};
template<typename T> struct is_pointer<T *volatile> : true_type
{
};
template<typename T> struct is_pointer<T *const volatile> : true_type
{
};

template<typename T> inline constexpr bool is_pointer_v = is_pointer<T>::value;

template<typename T> struct is_lvalue_reference : false_type
{
};
template<typename T> struct is_lvalue_reference<T &> : true_type
{
};

template<typename T> inline constexpr bool is_lvalue_reference_v = is_lvalue_reference<T>::value;

template<typename T> struct is_rvalue_reference : false_type
{
};
template<typename T> struct is_rvalue_reference<T &&> : true_type
{
};

template<typename T> inline constexpr bool is_rvalue_reference_v = is_rvalue_reference<T>::value;

template<typename T> struct is_reference : true_type
{
    static constexpr bool value = is_lvalue_reference_v<T> || is_rvalue_reference_v<T>;
};

template<typename T> inline constexpr bool is_reference_v = is_reference<T>::value;

template<class T> struct remove_reference
{
    typedef T type;
};
template<class T> struct remove_reference<T &>
{
    typedef T type;
};
template<class T> struct remove_reference<T &&>
{
    typedef T type;
};

template<class T> using remove_reference_t = typename remove_reference<T>::type;

template<class T> struct type_identity
{
    using type = T;
};

namespace detail
{
template<class T> auto try_add_lvalue_reference(int) -> std::type_identity<T &>;
template<class T> auto try_add_lvalue_reference(...) -> std::type_identity<T>;
template<class T> auto try_add_rvalue_reference(int) -> std::type_identity<T &&>;
template<class T> auto try_add_rvalue_reference(...) -> std::type_identity<T>;
} // namespace detail

template<class T> struct add_lvalue_reference : decltype(detail::try_add_lvalue_reference<T>(0))
{
};

template<class T> struct add_rvalue_reference : decltype(detail::try_add_rvalue_reference<T>(0))
{
};

template<typename T> typename std::add_rvalue_reference<T>::type declval() noexcept
{
    static_assert(false, "declval not allowed in an evaluated context");
}

namespace detail
{
template<class T> auto test_returnable(int) -> decltype(void(static_cast<T (*)()>(nullptr)), std::true_type{});
template<class> auto test_returnable(...) -> std::false_type;

template<class From, class To>
auto test_implicitly_convertible(int)
    -> decltype(void(std::declval<void (&)(To)>()(std::declval<From>())), std::true_type{});
template<class, class> auto test_implicitly_convertible(...) -> std::false_type;
} // namespace detail

template<class T, T v> struct integral_constant
{
    static constexpr T value = v;
    using value_type = T;
    using type = integral_constant;
    constexpr operator value_type() const noexcept
    {
        return value;
    }
    constexpr value_type operator()() const noexcept
    {
        return value;
    }
};

template<class T>
struct is_scalar
    : std::integral_constant<bool, std::is_arithmetic<T>::value || std::is_enum<T>::value ||
                                       std::is_pointer<T>::value || std::is_member_pointer<T>::value ||
                                       std::is_null_pointer<T>::value>
{
};

template<class From, class To>
struct is_convertible
    : std::integral_constant<bool, (decltype(detail::test_returnable<To>(0))::value &&
                                    decltype(detail::test_implicitly_convertible<From, To>(0))::value) ||
                                       (std::is_void<From>::value && std::is_void<To>::value)>
{
};

template<class From, class To> constexpr bool is_convertible_v = is_convertible<From, To>::value;

template<typename t>
    struct is_destructible : std::integral_constant < bool
    , requires(t object)
{
    object.~t();
}>{};

template<typename T> struct is_trivially_destructible
{
#if defined(_MSC_VER)
    // MSVC: use its intrinsic for checking trivial destructibility.
    static constexpr bool value = __has_trivial_destructor(T);
#elif defined(__is_trivially_destructible)
    // For GCC/Clang, if available.
    static constexpr bool value = __is_trivially_destructible(T);
#else
    // Fallback in case the above are not available.
    static constexpr bool value = __has_trivial_destructor(T);
#endif
};

template<typename T> inline constexpr bool is_trivially_destructible_v = is_trivially_destructible<T>::value;

template<typename t>
    struct is_nothrow_destructible : std::integral_constant < bool
    , requires(t object)
{
    {
        object.~t()
    } noexcept;
}>{};

template<typename T> inline constexpr bool is_nothrow_destructible_v = is_nothrow_destructible<T>::value;

template<class T>
struct is_object
    : std::integral_constant<bool, std::is_scalar<T>::value || std::is_array<T>::value || std::is_union<T>::value ||
                                       std::is_class<T>::value>
{
};
template<class T> constexpr bool is_object_v = is_object<T>::value;

template<typename T, typename... Args> class is_constructible
{
  private:
    template<typename U, typename = decltype(U(std::declval<Args>()...))> static char test(int);
    template<typename U> static long test(...);

  public:
    static constexpr bool value = sizeof(test<T>(0)) == sizeof(char);
};

template<typename T, typename... Args> constexpr bool is_constructible_v = is_constructible<T, Args...>::value;
} // namespace std
#endif