#ifndef INITIALIZER_LIST_H
#define INITIALIZER_LIST_H
#include <stddef.h>
namespace std {

template<typename T>
class [[nodiscard]] initializer_list final {
    public:
    using const_iterator = const T*;
    using size_type = std::size_t;

    constexpr initializer_list() noexcept : data_(nullptr), size_(0) { }
    constexpr initializer_list(const T* data, size_type size) noexcept
      : data_(data), size_(size) { }

    [[nodiscard]] constexpr const_iterator begin() const noexcept { return data_; }
    [[nodiscard]] constexpr const_iterator end() const noexcept { return data_ + size_; }
    [[nodiscard]] constexpr size_type size() const noexcept { return size_; }

private:
    const T* data_;
    size_type size_;
};
} // namespace std
#endif
