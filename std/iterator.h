#ifndef ITERATOR_H
#define ITERATOR_H

#include <stddef.h>
#include <concepts.h>
#include <type_traits.h>
#include <utility.h>

namespace std {
    struct input_iterator_tag {};
    struct output_iterator_tag {};
    struct forward_iterator_tag    : public input_iterator_tag {};
    struct bidirectional_iterator_tag : public forward_iterator_tag {};
    struct random_access_iterator_tag : public bidirectional_iterator_tag {};
    struct contiguous_iterator_tag : public random_access_iterator_tag {};

    template<typename T>
    constexpr T* addressof(T& arg) noexcept {
        return reinterpret_cast<T*>(
            &const_cast<char&>(reinterpret_cast<const volatile char&>(arg))
        );
    }

    template<typename Iterator>
    struct iterator_traits {
        using difference_type   = typename Iterator::difference_type;
        using value_type        = typename Iterator::value_type;
        using pointer           = typename Iterator::pointer;
        using reference         = typename Iterator::reference;
        using iterator_category = typename Iterator::iterator_category;
    };

    template<typename T>
    struct iterator_traits<T*> {
        using difference_type   = std::ptrdiff_t;
        using value_type        = T;
        using pointer           = T*;
        using reference         = T&;
        using iterator_category = random_access_iterator_tag;
    };

    template<typename T>
    struct iterator_traits<const T*> {
        using difference_type   = std::ptrdiff_t;
        using value_type        = T;
        using pointer           = const T*;
        using reference         = const T&;
        using iterator_category = random_access_iterator_tag;
    };

    template<typename Iterator>
    class reverse_iterator {
    protected:
        Iterator current;
    public:
        using iterator_type   = Iterator;
        using traits          = iterator_traits<Iterator>;
        using difference_type = typename traits::difference_type;
        using value_type      = typename traits::value_type;
        using pointer         = typename traits::pointer;
        using reference       = typename traits::reference;

        constexpr reverse_iterator() : current() {}
        explicit constexpr reverse_iterator(Iterator x) : current(x) {}
        template<typename U>
        constexpr reverse_iterator(const reverse_iterator<U>& other) : current(other.base()) {}

        constexpr Iterator base() const { return current; }

        constexpr reference operator*() const {
            Iterator tmp = current;
            return *--tmp;
        }
        constexpr pointer operator->() const {
            return addressof(operator*());
        }
        constexpr reverse_iterator& operator++() {
            --current;
            return *this;
        }
        constexpr reverse_iterator operator++(int) {
            reverse_iterator tmp(*this);
            --current;
            return tmp;
        }
        constexpr reverse_iterator& operator--() {
            ++current;
            return *this;
        }
        constexpr reverse_iterator operator--(int) {
            reverse_iterator tmp(*this);
            ++current;
            return tmp;
        }
        constexpr reverse_iterator operator+(difference_type n) const {
            return reverse_iterator(current - n);
        }
        constexpr reverse_iterator& operator+=(difference_type n) {
            current -= n;
            return *this;
        }
        constexpr reverse_iterator operator-(difference_type n) const {
            return reverse_iterator(current + n);
        }
        constexpr reverse_iterator& operator-=(difference_type n) {
            current += n;
            return *this;
        }
        constexpr reference operator[](difference_type n) const {
            return *(*this + n);
        }
    };

    template<typename Iterator1, typename Iterator2>
    constexpr bool operator==(const reverse_iterator<Iterator1>& x,
                                const reverse_iterator<Iterator2>& y) {
        return x.base() == y.base();
    }
    template<typename Iterator1, typename Iterator2>
    constexpr bool operator!=(const reverse_iterator<Iterator1>& x,
                                const reverse_iterator<Iterator2>& y) {
        return !(x == y);
    }
    template<typename Iterator1, typename Iterator2>
    constexpr bool operator<(const reverse_iterator<Iterator1>& x,
                               const reverse_iterator<Iterator2>& y) {
        return y.base() < x.base();
    }
    template<typename Iterator1, typename Iterator2>
    constexpr bool operator<=(const reverse_iterator<Iterator1>& x,
                                const reverse_iterator<Iterator2>& y) {
        return !(y < x);
    }
    template<typename Iterator1, typename Iterator2>
    constexpr bool operator>(const reverse_iterator<Iterator1>& x,
                               const reverse_iterator<Iterator2>& y) {
        return y < x;
    }
    template<typename Iterator1, typename Iterator2>
    constexpr bool operator>=(const reverse_iterator<Iterator1>& x,
                                const reverse_iterator<Iterator2>& y) {
        return !(x < y);
    }

    template<typename Iterator>
    constexpr reverse_iterator<Iterator> make_reverse_iterator(Iterator i) {
        return reverse_iterator<Iterator>(i);
    }


    template<typename Container>
    class back_insert_iterator {
    protected:
        Container* container;
    public:
        using container_type = Container;
        constexpr explicit back_insert_iterator(Container& x) : container(&x) {}
        constexpr back_insert_iterator& operator=(const typename Container::value_type& value) {
            container->push_back(value);
            return *this;
        }
        constexpr back_insert_iterator& operator=(typename Container::value_type&& value) {
            container->push_back(std::move(value));
            return *this;
        }
        constexpr back_insert_iterator& operator*() { return *this; }
        constexpr back_insert_iterator& operator++() { return *this; }
        constexpr back_insert_iterator operator++(int) { return *this; }
    };

    template<typename Container>
    constexpr back_insert_iterator<Container> back_inserter(Container& x) {
        return back_insert_iterator<Container>(x);
    }

    template<typename Container>
    class front_insert_iterator {
    protected:
        Container* container;
    public:
        using container_type = Container;
        constexpr explicit front_insert_iterator(Container& x) : container(&x) {}
        constexpr front_insert_iterator& operator=(const typename Container::value_type& value) {
            container->push_front(value);
            return *this;
        }
        constexpr front_insert_iterator& operator=(typename Container::value_type&& value) {
            container->push_front(std::move(value));
            return *this;
        }
        constexpr front_insert_iterator& operator*() { return *this; }
        constexpr front_insert_iterator& operator++() { return *this; }
        constexpr front_insert_iterator operator++(int) { return *this; }
    };

    template<typename Container>
    constexpr front_insert_iterator<Container> front_inserter(Container& x) {
        return front_insert_iterator<Container>(x);
    }

    template<typename Container>
    class insert_iterator {
    protected:
        Container* container;
        typename Container::iterator iter;
    public:
        using container_type = Container;
        constexpr insert_iterator(Container& x, typename Container::iterator i)
            : container(&x), iter(i) {}
        constexpr insert_iterator& operator=(const typename Container::value_type& value) {
            iter = container->insert(iter, value);
            ++iter;
            return *this;
        }
        constexpr insert_iterator& operator=(typename Container::value_type&& value) {
            iter = container->insert(iter, std::move(value));
            ++iter;
            return *this;
        }
        constexpr insert_iterator& operator*() { return *this; }
        constexpr insert_iterator& operator++() { return *this; }
        constexpr insert_iterator operator++(int) { return *this; }
    };

    template<typename Container, typename Iterator>
    constexpr insert_iterator<Container> inserter(Container& x, Iterator i) {
        return insert_iterator<Container>(x, i);
    }

    template<typename Iterator>
    class move_iterator {
    protected:
        Iterator current;
    public:
        using iterator_type   = Iterator;
        using traits          = iterator_traits<Iterator>;
        using difference_type = typename traits::difference_type;
        using value_type      = typename traits::value_type;
        using pointer         = typename traits::pointer;
        using reference       = value_type&&;

        constexpr move_iterator() : current() {}
        explicit constexpr move_iterator(Iterator i) : current(i) {}
        template<typename U>
        constexpr move_iterator(const move_iterator<U>& other) : current(other.base()) {}

        constexpr Iterator base() const { return current; }
        constexpr reference operator*() const {
            return static_cast<value_type&&>(*current);
        }
        constexpr pointer operator->() const {
            return current;
        }
        constexpr move_iterator& operator++() {
            ++current;
            return *this;
        }
        constexpr move_iterator operator++(int) {
            move_iterator tmp(*this);
            ++current;
            return tmp;
        }
        constexpr move_iterator& operator--() {
            --current;
            return *this;
        }
        constexpr move_iterator operator--(int) {
            move_iterator tmp(*this);
            --current;
            return tmp;
        }
        constexpr move_iterator operator+(difference_type n) const {
            return move_iterator(current + n);
        }
        constexpr move_iterator& operator+=(difference_type n) {
            current += n;
            return *this;
        }
        constexpr move_iterator operator-(difference_type n) const {
            return move_iterator(current - n);
        }
        constexpr move_iterator& operator-=(difference_type n) {
            current -= n;
            return *this;
        }
        constexpr reference operator[](difference_type n) const {
            return static_cast<value_type&&>(current[n]);
        }
    };

    template<typename Iterator1, typename Iterator2>
    constexpr bool operator==(const move_iterator<Iterator1>& x, const move_iterator<Iterator2>& y) {
        return x.base() == y.base();
    }
    template<typename Iterator1, typename Iterator2>
    constexpr bool operator!=(const move_iterator<Iterator1>& x, const move_iterator<Iterator2>& y) {
        return !(x == y);
    }
    template<typename Iterator1, typename Iterator2>
    constexpr bool operator<(const move_iterator<Iterator1>& x, const move_iterator<Iterator2>& y) {
        return x.base() < y.base();
    }
    template<typename Iterator1, typename Iterator2>
    constexpr bool operator<=(const move_iterator<Iterator1>& x, const move_iterator<Iterator2>& y) {
        return !(y < x);
    }
    template<typename Iterator1, typename Iterator2>
    constexpr bool operator>(const move_iterator<Iterator1>& x, const move_iterator<Iterator2>& y) {
        return y < x;
    }
    template<typename Iterator1, typename Iterator2>
    constexpr bool operator>=(const move_iterator<Iterator1>& x, const move_iterator<Iterator2>& y) {
        return !(x < y);
    }

    template<typename Iterator>
    constexpr move_iterator<Iterator> make_move_iterator(Iterator i) {
        return move_iterator<Iterator>(i);
    }

    template<typename It>
    concept input_iterator = requires(It it) {
        { *it } -> std::convertible_to<typename iterator_traits<It>::value_type>;
    } && std::copyable<It>;

    template<typename It, typename T>
    concept output_iterator = requires(It it, T&& t) {
        { *it = std::forward<T>(t) } -> std::same_as<It&>;
    };

    template<typename It>
    concept forward_iterator = input_iterator<It> && std::default_initializable<It>;

    template<typename It>
    concept bidirectional_iterator = forward_iterator<It> && requires(It it) {
        { --it } -> std::same_as<It&>;
    };

    template<typename It>
    concept random_access_iterator = bidirectional_iterator<It> && requires(It it, typename iterator_traits<It>::difference_type n) {
        { it + n } -> std::same_as<It>;
        { it - n } -> std::same_as<It>;
    };

    template<typename It>
    concept contiguous_iterator = random_access_iterator<It> && requires(It it) {
        { addressof(*it) } -> std::convertible_to<typename iterator_traits<It>::pointer>;
    };

    template<typename InputIt>
    constexpr typename iterator_traits<InputIt>::difference_type distance(InputIt first, InputIt last) {
        typename iterator_traits<InputIt>::difference_type n = 0;
        for (; first != last; ++first) {
            ++n;
        }
        return n;
    }

    template<typename InputIt, typename Distance>
    constexpr void advance(InputIt& it, Distance n) {
        if constexpr (random_access_iterator<InputIt>) {
            it += n;
        } else {
            if (n >= 0) {
                while (n--) ++it;
            } else {
                while (n++) --it;
            }
        }
    }

} // namespace my_iterator

#endif
