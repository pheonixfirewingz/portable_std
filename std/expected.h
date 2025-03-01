#ifndef EXPECTED_H
#define EXPECTED_H
#include <is_type.h>
namespace LunaVoxelEngine
{
namespace Utils
{
template<typename T, typename E> class Expected
{
    union {
        T val;
        E err;
    };
    bool has_val;

  public:
    constexpr Expected(const T &v)
        : val(v)
        , has_val(true)
    {
    }
    constexpr Expected(T &&v)
        : val(static_cast<T &&>(v))
        , has_val(true)
    {
    }

    constexpr Expected()
        : has_val(true)
    {

        new (&val) T();
    }

    ~Expected()
    {
        if (!is_void_v<T> && has_val)
        {
            val.~T();
        }
        else
        {
            err.~E();
        }
    }

    Expected(const Expected &other)
        : has_val(other.has_val)
    {
        if (has_val)
        {

            new (&val) T(other.val);
        }
        else
        {
            new (&err) E(other.err);
        }
    }

    Expected &operator=(const Expected &other)
    {
        if (this != &other)
        {
            this->~Expected();
            has_val = other.has_val;
            if (has_val)
            {

                new (&val) T(other.val);
            }
            else
            {
                new (&err) E(other.err);
            }
        }
        return *this;
    }

    static constexpr Expected<T, E> Unexpected(E &&e)
    {
        return Expected(static_cast<E &&>(e), nullptr);
    }

    constexpr bool has_value() const noexcept
    {
        return has_val;
    }

    constexpr bool has_error() const noexcept
    {
        return !has_val;
    }

    constexpr const T &value() const &
    {
        return val;
    }

    constexpr T &&value() &&
    {

        return static_cast<T &&>(val);
    }

    constexpr const E &error() const &
    {
        return err;
    }

    constexpr explicit operator bool() const noexcept
    {
        return has_val;
    }

    template<typename F> constexpr auto and_then(F &&f) &
    {
        using U = decltype(f(value()));
        if (has_value())
        {
            return f(value());
        }
        return U::Unexpected(error());
    }

  private:
    constexpr Expected(E &&e, nullptr_t)
        : err(static_cast<E &&>(e))
        , has_val(false)
    {
    }
};
} // namespace Utils
} // namespace LunaVoxelEngine
#endif
