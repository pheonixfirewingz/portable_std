#ifndef CSTRING_H
#define CSTRING_H
#include <algorithm.h>
#include <stddef.h>

/*!
 * @brief Get the length of a C-style string.
 * @param str The string to measure.
 * @return The length of the string.
 */
inline std::size_t strlen(const char *str)
{
    // Start at 0 and count up until the null terminator.
    std::size_t len = 0;
    while (str[len] != 0)
    {
        ++len;
    }
    return len;
}

/*!
 * @brief Compare two C-style strings lexicographically.
 * @param str1 The first string to compare.
 * @param str2 The second string to compare.
 * @return An integer less than, equal to, or greater than zero if str1 is found, respectively, to be less than,
 * to match, or be greater than str2.
 */
inline int strcmp(const char *str1, const char *str2)
{
    // Get the lengths of both strings
    auto len = strlen(str1);
    auto len2 = strlen(str2);

    // Compare lengths
    if (len < len2)
    {
        return -1; // str1 is shorter than str2
    }
    if (len > len2)
    {
        return 1; // str1 is longer than str2
    }

    // Compare strings lexicographically
    return std::memcmp(str1, str2, len);
}
#endif