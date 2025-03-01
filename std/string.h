/*!
 * @file
 * @brief This header file contains the declaration of the string class.
 * @details
 * The string class provides a robust implementation of a UTF-16 string with comprehensive
 * Unicode support. It handles various string encodings (ASCII, UTF-8, UTF-16, UTF-32)
 * and provides automatic conversion between them. The class implements common string
 * operations and follows modern C++ container conventions. while it follows the c++ standard it breaks the rules by
 * being a utf-16 string
 *
 * @namespace std
 *
 * @section features Features
 * - UTF-16 internal representation
 * - Automatic encoding detection and conversion
 * - Support for ASCII, UTF-8, UTF-16 (LE/BE), and UTF-32 (LE/BE)
 * - Standard container interface (iterators, size, capacity)
 * - Memory management with automatic growth
 * - string manipulation operations (append, insert, erase)
 * - string search functionality
 * - Surrogate pair handling for Unicode supplementary planes
 *
 * @section usage Usage
 * The string class provides a familiar interface similar to std::string. It can be
 * constructed from various string types and supports common string operations.
 *
 * @section example Example
 * @code
 * // Create a string
 * string str("Hello, World!");
 *
 * // Append to string
 * str += " Welcome";
 *
 * // Search within string
 * size_type pos = str.find("World");
 *
 * // Insert content
 * str.insert(6, "there ");
 *
 * // Convert to UTF-8
 * throw_away_string utf8 = str.throw_away();
 * @endcode
 *
 * @section encoding Encoding Support
 * The class supports multiple input encodings:
 * - ASCII
 * - UTF-8 (with BOM detection)
 * - UTF-16 Little Endian
 * - UTF-16 Big Endian
 * - UTF-32 Little Endian
 * - UTF-32 Big Endian
 *
 * @section thread_safety Thread Safety
 * The string class is not thread-safe. Concurrent access must be externally synchronized.
 */

#ifndef string_H
#define string_H
#include <algorithm.h>
#include <cstring.h>
#include <new.h>
#include <iterator.h>
#include <stdexcept.h>
namespace std
{
/*!
 * @brief A temporary string class for UTF-8 conversion results. this is a move only class that is not ment to have a
 * size_type lifetime
 */
class [[nodiscard]] throw_away_string final
{
  public:
    /*!
     * @brief Constructor for the throw_away_string class.
     * @param str The input string to be converted to UTF-8.
     */
    explicit throw_away_string(const char *str)
    {
        len = strlen(str);
        ptr = new char[len + 1];
        memset(static_cast<void *>(const_cast<char *>(ptr)), '\0', len + 1);
        memcpy(static_cast<void *>(const_cast<char *>(ptr)), reinterpret_cast<const void *>(str), len);
    }
    /*!
     * @brief Destructor for the throw_away_string class.
     */
    throw_away_string()
    {
        if (ptr != nullptr)
            delete[] ptr;
        ptr = nullptr;
    }
    /*!
     * @brief Copy constructor for the throw_away_string class. is deleted to prevent copying.
     */
    throw_away_string(const throw_away_string &other) = delete;
    /*!
     * @brief Copy assignment operator for the throw_away_string class. is deleted to prevent copying.
     */
    throw_away_string &operator=(const throw_away_string &other) = delete;
    /*!
     * @brief Move constructor for the throw_away_string class. this the only way to handle the string
     */
    throw_away_string(throw_away_string &&other) noexcept
    {
        ptr = other.ptr;
        len = other.len;
        other.ptr = nullptr;
        other.len = 0;
    }
    /*!
     * @brief Move assignment operator for the throw_away_string class. this the only way to handle the string
     */
    throw_away_string &operator=(throw_away_string &&other) noexcept
    {
        ptr = other.ptr;
        len = other.len;
        other.ptr = nullptr;
        other.len = 0;
        return *this;
    }
    /*!
     * @brief Returns a pointer to the underlying character array.
     * @return A pointer to the character array.
     */
    const char *c_str() const
    {
        return ptr;
    }

    /*!
     * @brief Returns the length of the string.
     * @return The length of the string.
     */
    std::size_t size() const
    {
        return len;
    }

  private:
    const char *ptr = nullptr; //!< Pointer to the underlying character array.
    std::size_t len = 0;       //!< Length of the string.
};

class [[nodiscard]] string final
{
  public:
    using data_type = short;
    using size_type = std::size_t;
    using ssize_type = std::ssize_t;
    using udata_type = unsigned short;
    using iterator = data_type *;
    using const_iterator = const data_type *;
    using const_type = const data_type *;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;
    static constexpr size_type npos = static_cast<size_type>(-1);

    string(const_type str, ssize_type len_in = -1)
    {
        if (len_in == -1)
        {
            while (str[len] != 0)
            {
                ++len;
            }
        }
        else
        {
            len = static_cast<size_type>(len_in);
        }
        ensure_capacity(len);
        memcpy(ptr, str, len * sizeof(data_type));
    }

    string()
        : len(0)
    {
        ensure_capacity(1);
        ptr[0] = 0;
    }

    string(const char *str, ssize_type len_in = -1)
    {
        Encoding encoding = detect_encoding(str, len_in);
        convert_to_utf16(str, len_in, encoding);
    }

    string(const string &other)
        : len(other.len)
        , _str_capacity(other._str_capacity)
    {
        if (len > 0)
        {
            ptr = new data_type[_str_capacity];
            memcpy(ptr, other.ptr, len * sizeof(data_type));
        }
    }

    string(string &&other) noexcept
        : ptr(other.ptr)
        , len(other.len)
        , _str_capacity(other._str_capacity)
    {
        other.ptr = nullptr;
        other.len = 0;
        other._str_capacity = 0;
    }

    ~string()
    {
        delete[] ptr;
    }

    string &operator=(const string &other)
    {
        if (this != &other)
        {
            delete[] ptr;
            len = other.len;
            _str_capacity = other._str_capacity;
            ptr = new data_type[_str_capacity];
            memcpy(ptr, other.ptr, len * sizeof(data_type));
        }
        return *this;
    }

    string &operator=(string &&other) noexcept
    {
        if (this != &other)
        {
            delete[] ptr;
            ptr = other.ptr;
            len = other.len;
            _str_capacity = other._str_capacity;
            other.ptr = nullptr;
            other.len = 0;
            other._str_capacity = 0;
        }
        return *this;
    }

    string &operator=(const_type str)
    {
        len = strlen(reinterpret_cast<const char *>(str));
        ensure_capacity(len);
        memcpy(ptr, str, len * sizeof(data_type));
        return *this;
    }

    string operator+(const char *str) const
    {
        string result = *this;
        result.append(str);
        return result;
    }

    string operator+(const_type str) const
    {
        string result = *this;
        result.append(str);
        return result;
    }

    string operator+(const string &str) const
    {
        string result = *this;
        result.append(str);
        return result;
    }

    iterator begin() noexcept
    {
        return ptr;
    }
    const_iterator begin() const noexcept
    {
        return ptr;
    }
    const_iterator cbegin() const noexcept
    {
        return ptr;
    }
    iterator end() noexcept
    {
        return ptr + len;
    }
    const_iterator end() const noexcept
    {
        return ptr + len;
    }
    const_iterator cend() const noexcept
    {
        return ptr + len;
    }

    reverse_iterator rbegin() noexcept
    {
        return reverse_iterator(end());
    }
    const_reverse_iterator rbegin() const noexcept
    {
        return const_reverse_iterator(end());
    }
    const_reverse_iterator crbegin() const noexcept
    {
        return const_reverse_iterator(end());
    }
    reverse_iterator rend() noexcept
    {
        return reverse_iterator(begin());
    }
    const_reverse_iterator rend() const noexcept
    {
        return const_reverse_iterator(begin());
    }
    const_reverse_iterator crend() const noexcept
    {
        return const_reverse_iterator(begin());
    }

    size_type size() const noexcept
    {
        return len;
    }

    size_type length() const noexcept
    {
        return len;
    }

    constexpr bool empty() const noexcept
    {
        return len == 0;
    }

    size_type capacity() const noexcept
    {
        return _str_capacity;
    }

    void reserve(size_type new__str_capacity)
    {
        ensure_capacity(new__str_capacity);
    }

    constexpr size_type max_size() const noexcept
    {
        return static_cast<size_type>(-1) / sizeof(data_type);
    }

    data_type at(size_type index) const
    {
        if (index >= len)
        {
            throw std::out_of_range();
        }
        return ptr[index];
    }

    data_type &at(size_type index)
    {
        if (index >= len)
        {
            throw std::out_of_range();
        }
        return ptr[index];
    }

    data_type operator[](size_type index) const
    {
        return at(index);
    }

    data_type &operator[](size_type index)
    {
        return at(index);
    }

    data_type *data()
    {
        return ptr;
    }

    const_type data() const
    {
        return ptr;
    }

    data_type front() const
    {
        return ptr[0];
    }

    data_type back() const
    {
        return ptr[len - 1];
    }

    data_type *raw_data()
    {
        return ptr;
    }
    const_type raw_data() const
    {
        return ptr;
    }

    void push_back(data_type ch)
    {
        ensure_capacity(len + 1);
        ptr[len++] = ch;
    }

    void pop_back()
    {
        if (len > 0)
        {
            --len;
        }
    }

    void shrink_to_fit()
    {
        _str_capacity = len;
        data_type *new_ptr = new data_type[_str_capacity];
        memcpy(new_ptr, ptr, len * sizeof(data_type));
        delete[] ptr;
        ptr = new_ptr;
    }

    void resize(size_type new_size)
    {
        ensure_capacity(new_size);
        len = new_size;
    }

    string &append(const char *str)
    {
        auto encoding = detect_encoding(str, -1);
        convert_to_utf16(str, -1, encoding);
        return *this;
    }

    string &operator+=(const char *str)
    {
        return append(str);
    }

    string &append(const string &other)
    {
        ensure_capacity(len + other.len);
        memcpy(ptr + len, other.ptr, other.len * sizeof(data_type));
        len += other.len;
        return *this;
    }

    string &operator+=(const string &other)
    {
        return append(other);
    }

    string &append(data_type ch)
    {
        ensure_capacity(len + 1);
        ptr[len++] = ch;
        return *this;
    }

    string &operator+=(data_type ch)
    {
        return append(ch);
    }

    string substr(size_type start, size_type end) const
    {
        string substr;
        substr.reserve(end - start);
        for (size_type i = start; i < end; ++i)
        {
            substr.push_back(ptr[i]);
        }
        return substr;
    }

    string &insert(size_type pos, const string &str)
    {
        if (pos > len)
        {
            throw std::out_of_range();
        }
        size_type new_len = len + str.len;
        ensure_capacity(new_len);

        // Move existing characters
        for (size_type i = len; i > pos; --i)
        {
            ptr[i + str.len - 1] = ptr[i - 1];
        }

        // Copy new string
        for (size_type i = 0; i < str.len; ++i)
        {
            ptr[pos + i] = str.ptr[i];
        }

        len = new_len;
        return *this;
    }

    string &insert(size_type pos, const_type s, size_type n)
    {
        if (pos > len)
        {
            throw std::out_of_range();
        }
        ensure_capacity(len + n);

        // Move existing characters
        for (size_type i = len; i > pos; --i)
        {
            ptr[i + n - 1] = ptr[i - 1];
        }

        // Copy new characters
        for (size_type i = 0; i < n; ++i)
        {
            ptr[pos + i] = s[i];
        }

        len += n;
        return *this;
    }

    string &insert(size_type pos, size_type n, data_type c)
    {
        if (pos > len)
        {
            throw std::out_of_range();
        }
        ensure_capacity(len + n);

        // Move existing characters
        for (size_type i = len; i > pos; --i)
        {
            ptr[i + n - 1] = ptr[i - 1];
        }

        // Fill with character
        for (size_type i = 0; i < n; ++i)
        {
            ptr[pos + i] = c;
        }

        len += n;
        return *this;
    }

    string &erase(size_type pos, size_type n)
    {
        if (pos > len)
        {
            throw std::out_of_range();
        }

        if (n == npos || pos + n > len)
        {
            n = len - pos;
        }

        for (size_type i = pos; i + n < len; ++i)
        {
            ptr[i] = ptr[i + n];
        }

        len -= n;
        return *this;
    }

    constexpr size_type find(const string &str, size_type pos) const noexcept
    {
        if (pos >= len)
            return npos;
        if (str.empty())
            return pos;

        for (size_type i = pos; i <= len - str.len; ++i)
        {
            bool found = true;
            for (size_type j = 0; j < str.len; ++j)
            {
                if (ptr[i + j] != str.ptr[j])
                {
                    found = false;
                    break;
                }
            }
            if (found)
                return i;
        }
        return npos;
    }

    constexpr size_type rfind(const string &str, size_type pos) const noexcept
    {
        if (str.empty())
            return min(pos, len);
        if (len < str.len)
            return npos;

        pos = min(pos, len - str.len);

        for (size_type i = pos + 1; i-- > 0;)
        {
            bool found = true;
            for (size_type j = 0; j < str.len; ++j)
            {
                if (ptr[i + j] != str.ptr[j])
                {
                    found = false;
                    break;
                }
            }
            if (found)
                return i;
        }
        return npos;
    }

    constexpr size_type find_first_of(const string &str, size_type pos) const noexcept
    {
        for (size_type i = pos; i < len; ++i)
        {
            for (size_type j = 0; j < str.len; ++j)
            {
                if (ptr[i] == str.ptr[j])
                {
                    return i;
                }
            }
        }
        return npos;
    }

    constexpr size_type find_last_of(const string &str, size_type pos) const noexcept
    {
        if (empty())
            return npos;
        pos = min(pos, len - 1);

        for (size_type i = pos + 1; i-- > 0;)
        {
            for (size_type j = 0; j < str.len; ++j)
            {
                if (ptr[i] == str.ptr[j])
                {
                    return i;
                }
            }
        }
        return npos;
    }

    constexpr bool operator<(const string &rhs) const noexcept
    {
        size_type min_len = min(len, rhs.len);
        for (size_type i = 0; i < min_len; ++i)
        {
            if (ptr[i] < rhs.ptr[i])
                return true;
            if (ptr[i] > rhs.ptr[i])
                return false;
        }
        return len < rhs.len;
    }

    constexpr bool operator<=(const string &rhs) const noexcept
    {
        return !(rhs < *this);
    }

    constexpr bool operator>(const string &rhs) const noexcept
    {
        return rhs < *this;
    }

    constexpr bool operator>=(const string &rhs) const noexcept
    {
        return !(*this < rhs);
    }

    constexpr bool operator==(const string &other) const
    {
        if (len != other.len)
        {
            return false;
        }
        return memcmp(ptr, other.ptr, len * sizeof(data_type)) == 0;
    }

    constexpr bool operator!=(const string &other) const
    {
        return !(*this == other);
    }

    const throw_away_string throw_away() const
    {
        if (!ptr)
            return throw_away_string("\0");
        char *utf8_result = new char[len + 1];
        memset(utf8_result, '\0', len + 1);

        size_type i = 0;
        ssize_type j = 0;

        while (i < len)
        {
            udata_type code_unit = static_cast<udata_type>(ptr[i++]);

            if (code_unit < 0x80)
            {
                // 1-byte UTF-8
                utf8_result[j++] = static_cast<char>(code_unit);
            }
            else if (code_unit < 0x800)
            {
                // 2-byte UTF-8
                utf8_result[j++] = static_cast<char>(0xC0 | (code_unit >> 6));
                utf8_result[j++] = static_cast<char>(0x80 | (code_unit & 0x3F));
            }
            else if (code_unit >= 0xD800 && code_unit <= 0xDBFF)
            {
                // Surrogate pair: high surrogate
                udata_type high_surrogate = code_unit;
                udata_type low_surrogate = static_cast<udata_type>(ptr[i]);

                if (low_surrogate >= 0xDC00 && low_surrogate <= 0xDFFF)
                {
                    // Valid surrogate pair
                    i++; // Consume the low surrogate
                    unsigned int code_point =
                        static_cast<unsigned int>(0x10000) +
                        ((static_cast<unsigned int>(high_surrogate) - static_cast<unsigned int>(0xD800))
                         << static_cast<unsigned int>(10)) +
                        (static_cast<unsigned int>(low_surrogate) - static_cast<unsigned int>(0xDC00));

                    // Encode the code point as 4-byte UTF-8
                    utf8_result[j++] = static_cast<char>(0xF0 | (code_point >> 18));
                    utf8_result[j++] = static_cast<char>(0x80 | ((code_point >> 12) & 0x3F));
                    utf8_result[j++] = static_cast<char>(0x80 | ((code_point >> 6) & 0x3F));
                    utf8_result[j++] = static_cast<char>(0x80 | (code_point & 0x3F));
                }
                else
                {
                    // Invalid surrogate pair; skip the invalid sequence
                    continue;
                }
            }
            else if (code_unit >= 0xDC00 && code_unit <= 0xDFFF)
            {
                // Unpaired low surrogate; skip it
                continue;
            }
            else
            {
                // 3-byte UTF-8
                utf8_result[j++] = static_cast<char>(0xE0 | (code_unit >> 12));
                utf8_result[j++] = static_cast<char>(0x80 | ((code_unit >> 6) & 0x3F));
                utf8_result[j++] = static_cast<char>(0x80 | (code_unit & 0x3F));
            }
        }
        throw_away_string ret(utf8_result);
        delete[] utf8_result;
        return ret;
    }

    bool start_with(const string &other) const
    {
        if (len < other.len)
        {
            return false;
        }
        return memcmp(ptr, other.ptr, other.len * sizeof(data_type)) == 0;
    }

    bool end_with(const string &other) const
    {
        if (len < other.len)
        {
            return false;
        }
        return memcmp(ptr + len - other.len, other.ptr, other.len * sizeof(data_type)) == 0;
    }

    void clear()
    {
        delete[] ptr;
        ptr = nullptr;
        len = 0;
        _str_capacity = 0;
    }

    void resize(size_type n, data_type ch)
    {
        if (n > len)
        {
            reserve(n);
            while (len < n)
            {
                push_back(ch);
            }
            return;
        }
        resize(n);
    }

    friend string operator+(const char *lhs, const string &rhs)
    {
        return string(lhs) + rhs;
    }

    friend string operator+(const_type lhs, const string &rhs)
    {
        return string(lhs) + rhs;
    }

  private:
    data_type *ptr = nullptr;
    size_type len = 0;
    size_type _str_capacity = 0;

    enum class Encoding
    {
        ASCII,
        UTF8,
        UTF16_LE,
        UTF16_BE,
        UTF32_LE,
        UTF32_BE
    };

    ssize_type get_utf16_length(const char *str, Encoding encoding)
    {
        if (!str)
            return 0;

        // Skip BOM
        if (encoding == Encoding::UTF8 && static_cast<unsigned char>(str[0]) == 0xEF &&
            static_cast<unsigned char>(str[1]) == 0xBB && static_cast<unsigned char>(str[2]) == 0xBF)
        {
            str += 3;
        }
        else if ((encoding == Encoding::UTF16_BE || encoding == Encoding::UTF16_LE) &&
                 static_cast<unsigned char>(str[0]) == 0xFE && static_cast<unsigned char>(str[1]) == 0xFF)
        {
            str += 2;
        }
        else if ((encoding == Encoding::UTF32_BE || encoding == Encoding::UTF32_LE) &&
                 ((static_cast<unsigned char>(str[0]) == 0x00 && static_cast<unsigned char>(str[1]) == 0x00 &&
                   static_cast<unsigned char>(str[2]) == 0xFE && static_cast<unsigned char>(str[3]) == 0xFF) ||
                  (static_cast<unsigned char>(str[0]) == 0xFF && static_cast<unsigned char>(str[1]) == 0xFE &&
                   static_cast<unsigned char>(str[2]) == 0x00 && static_cast<unsigned char>(str[3]) == 0x00)))
        {
            str += 4;
        }

        ssize_type utf16_len = 0;

        switch (encoding)
        {
        case Encoding::ASCII:
            utf16_len = static_cast<ssize_type>(strlen(str));
            break;

        case Encoding::UTF8: {
            for (ssize_type i = 0; str[i];)
            {
                unsigned char byte = static_cast<unsigned char>(str[i]);
                if (byte < 0x80)
                {
                    utf16_len++;
                    i++;
                }
                else if ((byte & 0xE0) == 0xC0)
                {
                    utf16_len++;
                    i += 2;
                }
                else if ((byte & 0xF0) == 0xE0)
                {
                    utf16_len++;
                    i += 3;
                }
                else if ((byte & 0xF8) == 0xF0)
                {
                    utf16_len += 2; // Emoji or supplementary character (surrogate pair)
                    i += 4;
                }
                else
                {
                    // Handle invalid byte sequences gracefully
                    i++;
                }
            }
            break;
        }

        case Encoding::UTF16_LE:
        case Encoding::UTF16_BE: {
            const udata_type *ptr_utf16 = reinterpret_cast<const udata_type *>(str);
            while (*ptr_utf16)
            {
                if (*ptr_utf16 >= 0xD800 && *ptr_utf16 <= 0xDBFF)
                {
                    utf16_len += 2; // High surrogate followed by low surrogate
                    ptr_utf16 += 2;
                }
                else
                {
                    utf16_len++;
                    ptr_utf16++;
                }
            }
            break;
        }

        case Encoding::UTF32_LE:
        case Encoding::UTF32_BE: {
            const unsigned int *utf32_str = reinterpret_cast<const unsigned int *>(str);
            while (*utf32_str)
            {
                utf16_len += (*utf32_str > 0xFFFF) ? 2 : 1; // Surrogate pairs for supplementary planes
                utf32_str++;
            }
            break;
        }
        }

        return utf16_len;
    }

    Encoding detect_encoding(const char *str, ssize_type len_in)
    {
        // Determine actual length if not provided
        if (len_in == -1)
        {
            len_in = static_cast<ssize_type>(strlen(str));
        }

        if (len_in < 2)
            return Encoding::ASCII;

        const unsigned char *bytes = reinterpret_cast<const unsigned char *>(str);

        // UTF-8 BOM check
        if (len_in >= 3 && bytes[0] == 0xEF && bytes[1] == 0xBB && bytes[2] == 0xBF)
            return Encoding::UTF8;

        // UTF-16 BOM check
        if (len_in >= 2)
        {
            if (bytes[0] == 0xFE && bytes[1] == 0xFF)
                return Encoding::UTF16_BE;
            if (bytes[0] == 0xFF && bytes[1] == 0xFE)
                return Encoding::UTF16_LE;
        }

        // UTF-32 BOM check
        if (len_in >= 4)
        {
            if (bytes[0] == 0x00 && bytes[1] == 0x00 && bytes[2] == 0xFE && bytes[3] == 0xFF)
                return Encoding::UTF32_BE;
            if (bytes[0] == 0xFF && bytes[1] == 0xFE && bytes[2] == 0x00 && bytes[3] == 0x00)
                return Encoding::UTF32_LE;
        }

        return detect_utf8_heuristic(str, len_in);
    }

    Encoding detect_utf8_heuristic(const char *str, ssize_type len_in)
    {
        size_type utf8_sequence_count = 0;
        size_type expected_continuation_bytes = 0;

        for (ssize_type i = 0; i < len_in; ++i)
        {
            unsigned char byte = static_cast<unsigned char>(str[i]);

            if (expected_continuation_bytes == 0)
            {
                if (byte <= 0x7F)
                    continue;

                if ((byte & 0xE0) == 0xC0)
                    expected_continuation_bytes = 1;
                else if ((byte & 0xF0) == 0xE0)
                    expected_continuation_bytes = 2;
                else if ((byte & 0xF8) == 0xF0)
                    expected_continuation_bytes = 3;
                else
                    return Encoding::ASCII;

                utf8_sequence_count++;
            }
            else
            {
                if ((byte & 0xC0) != 0x80)
                    return Encoding::ASCII;
                expected_continuation_bytes--;
            }
        }

        return (utf8_sequence_count > 0) ? Encoding::UTF8 : Encoding::ASCII;
    }

    void convert_to_utf16(const char *str, ssize_type len_in, Encoding encoding)
    {
        size_type true_len = static_cast<size_type>(len_in);
        if (len_in == -1)
        {
            true_len = strlen(str);
        }

        ensure_capacity(true_len);
        len = 0;

        switch (encoding)
        {
        case Encoding::ASCII:
            for (ssize_type i = 0; i < len_in; ++i)
            {
                ptr[len++] = static_cast<data_type>(str[i]);
            }
            break;

        case Encoding::UTF8:
            convert_utf8_to_utf16(str, true_len);
            break;

        case Encoding::UTF16_LE:
            convert_utf16le_to_utf16(str, true_len);
            break;

        case Encoding::UTF16_BE:
            convert_utf16be_to_utf16(str, true_len);
            break;

        case Encoding::UTF32_LE:
            convert_utf32le_to_utf16(str, true_len);
            break;

        case Encoding::UTF32_BE:
            convert_utf32be_to_utf16(str, true_len);
            break;
        }
    }

    void convert_utf8_to_utf16(const char *str, size_type len_in)
    {
        size_type utf16_index = 0;
        for (size_type i = 0; i < len_in;)
        {
            unsigned char first_byte = static_cast<unsigned char>(str[i]);

            if (first_byte < 0x80)
            {
                ensure_capacity(utf16_index + 1);
                ptr[utf16_index++] = first_byte;
                i++;
            }
            else if ((first_byte & 0xE0) == 0xC0)
            {
                if (i + 1 >= len_in)
                    throw std::runtime_error("Incomplete UTF-8 sequence");

                unsigned char second_byte = static_cast<unsigned char>(str[i + 1]);
                if ((second_byte & 0xC0) != 0x80)
                    throw std::runtime_error("Invalid UTF-8 sequence");

                unsigned int code_point = (static_cast<unsigned int>(first_byte) & 0x1F) << 6 |
                                          (static_cast<unsigned int>(second_byte) & 0x3F);
                ensure_capacity(utf16_index + 1);
                ptr[utf16_index++] = static_cast<data_type>(code_point);
                i += 2;
            }

            else if ((first_byte & 0xF0) == 0xE0)
            {
                if (i + 2 >= len_in)
                    throw std::runtime_error("Incomplete UTF-8 sequence");

                unsigned char second_byte = static_cast<unsigned char>(str[i + 1]);
                unsigned char third_byte = static_cast<unsigned char>(str[i + 2]);

                if ((second_byte & 0xC0) != 0x80 || (third_byte & 0xC0) != 0x80)
                    throw std::runtime_error("Invalid UTF-8 sequence");

                unsigned int code_point = ((static_cast<unsigned int>(first_byte) & 0x0F) << 12) |
                                          ((static_cast<unsigned int>(second_byte) & 0x3F) << 6) |
                                          (static_cast<unsigned int>(third_byte) & 0x3F);
                ensure_capacity(utf16_index + 1);
                ptr[utf16_index++] = static_cast<data_type>(code_point);
                i += 3;
            }

            else if ((first_byte & 0xF8) == 0xF0)
            {
                if (i + 3 >= len_in)
                    throw std::runtime_error("Incomplete UTF-8 sequence");

                unsigned char second_byte = static_cast<unsigned char>(str[i + 1]);
                unsigned char third_byte = static_cast<unsigned char>(str[i + 2]);
                unsigned char fourth_byte = static_cast<unsigned char>(str[i + 3]);

                if ((second_byte & 0xC0) != 0x80 || (third_byte & 0xC0) != 0x80 || (fourth_byte & 0xC0) != 0x80)
                    throw std::runtime_error("Invalid UTF-8 sequence");

                unsigned int code_point = ((static_cast<unsigned int>(first_byte) & 0x07) << 18) | ((static_cast<unsigned int>(second_byte) & 0x3F) << 12) |
                                          ((static_cast<unsigned int>(third_byte) & 0x3F) << 6) | (static_cast<unsigned int>(fourth_byte) & 0x3F);

                code_point -= 0x10000;
                ensure_capacity(utf16_index + 2);
                ptr[utf16_index++] = static_cast<data_type>(0xD800 | ((code_point >> 10) & 0x3FF));
                ptr[utf16_index++] = static_cast<data_type>(0xDC00 | (code_point & 0x3FF));

                i += 4;
            }
            else
            {
                throw std::runtime_error("Invalid UTF-8 start byte");
            }
        }

        len = utf16_index;
    }

    void convert_utf16le_to_utf16(const char *str, size_type len_in)
    {
        ensure_capacity(len_in / 2);
        len = 0;

        for (size_type i = 0; i < len_in; i += 2)
        {
            udata_type code_unit = (static_cast<unsigned char>(str[i + 1]) << 8) | static_cast<unsigned char>(str[i]);

            ensure_capacity(len + 1);
            ptr[len++] = static_cast<data_type>(code_unit);

            if (code_unit >= 0xD800 && code_unit <= 0xDBFF)
            {
                if (i + 2 >= len_in)
                    throw std::runtime_error("Incomplete surrogate pair");

                udata_type low_surrogate =
                    (static_cast<unsigned char>(str[i + 3]) << 8) | static_cast<unsigned char>(str[i + 2]);

                if (low_surrogate < 0xDC00 || low_surrogate > 0xDFFF)
                    throw std::runtime_error("Invalid low surrogate");

                ensure_capacity(len + 1);
                ptr[len++] = static_cast<data_type>(low_surrogate);
                i += 2;
            }
        }
    }

    void convert_utf16be_to_utf16(const char *str, size_type len_in)
    {
        ensure_capacity(len_in / 2);
        len = 0;

        for (size_type i = 0; i < len_in; i += 2)
        {
            udata_type code_unit = (static_cast<unsigned char>(str[i]) << 8) | static_cast<unsigned char>(str[i + 1]);

            ensure_capacity(len + 1);
            ptr[len++] = static_cast<data_type>(code_unit);

            if (code_unit >= 0xD800 && code_unit <= 0xDBFF)
            {
                if (i + 2 >= len_in)
                    throw std::runtime_error("Incomplete surrogate pair");

                udata_type low_surrogate =
                    (static_cast<unsigned char>(str[i + 2]) << 8) | static_cast<unsigned char>(str[i + 3]);

                if (low_surrogate < 0xDC00 || low_surrogate > 0xDFFF)
                    throw std::runtime_error("Invalid low surrogate");

                ensure_capacity(len + 1);
                ptr[len++] = static_cast<data_type>(low_surrogate);
                i += 2;
            }
        }
    }

    void convert_utf32le_to_utf16(const char *str, size_type len_in)
    {
        ensure_capacity(len_in / 4 * 2);
        len = 0;

        for (size_type i = 0; i < len_in; i += 4)
        {
            unsigned int code_point =
                static_cast<unsigned char>(str[i]) | (static_cast<unsigned char>(str[i + 1]) << 8) |
                (static_cast<unsigned char>(str[i + 2]) << 16) | (static_cast<unsigned char>(str[i + 3]) << 24);

            if (code_point <= 0xFFFF)
            {
                ensure_capacity(len + 1);
                ptr[len++] = static_cast<data_type>(code_point);
                continue;
            }
            else if (code_point <= 0x10FFFF)
            {
                code_point -= 0x10000;
                ensure_capacity(len + 2);
                ptr[len++] = static_cast<data_type>(0xD800 | ((code_point >> 10) & 0x3FF));
                ptr[len++] = static_cast<data_type>(0xDC00 | (code_point & 0x3FF));
                continue;
            }
            throw std::runtime_error("Invalid Unicode code point");
        }
    }

    void convert_utf32be_to_utf16(const char *str, size_type len_in)
    {
        ensure_capacity(len_in / 4 * 2);
        len = 0;

        for (size_type i = 0; i < len_in; i += 4)
        {
            unsigned int code_point =
                (static_cast<unsigned char>(str[i]) << 24) | (static_cast<unsigned char>(str[i + 1]) << 16) |
                (static_cast<unsigned char>(str[i + 2]) << 8) | static_cast<unsigned char>(str[i + 3]);

            if (code_point <= 0xFFFF)
            {
                ensure_capacity(len + 1);
                ptr[len++] = static_cast<data_type>(code_point);
            }
            else if (code_point <= 0x10FFFF)
            {
                code_point -= 0x10000;
                ensure_capacity(len + 2);
                ptr[len++] = static_cast<data_type>(0xD800 | ((code_point >> 10) & 0x3FF));
                ptr[len++] = static_cast<data_type>(0xDC00 | (code_point & 0x3FF));
            }
            else
            {
                throw std::runtime_error("Invalid Unicode code point");
            }
        }
    }

    void ensure_capacity(size_type new__str_capacity)
    {
        if (new__str_capacity > _str_capacity)
        {
            size_type new_cap = (_str_capacity == 0) ? 8 : _str_capacity * 2;
            if (new_cap < new__str_capacity)
            {
                new_cap = new__str_capacity;
            }
            data_type *new_ptr = new data_type[new_cap];
            if (ptr != nullptr && len > 0)
            {
                memcpy(new_ptr, ptr, len * sizeof(data_type));
            }
            delete[] ptr;
            ptr = new_ptr;
            _str_capacity = new_cap;
        }
    }
};
} // namespace std
#endif
