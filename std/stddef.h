/*!
 * @file stddef.h
 * @brief Definitions of common types and macros.
 *
 * This header provides definitions of common types and macros used throughout
 * the standard library.
 *
 * @namespace std
 * @note This header is a part of the C++ standard library.
 */

#ifndef STDDEF_H
#define STDDEF_H

namespace std
{

    /*!
     * @brief Signed integer type of the result of subtracting two pointers.
     *
     * The type of the result of subtracting two pointers is implementation-defined.
     * This type is used to represent the result of subtracting two pointers.
     */
    typedef long int ptrdiff_t;

    /*!
     * @brief Unsigned integer type of the size of any object.
     *
     * The type of the size of any object is implementation-defined. This type is
     * used to represent the size of any object.
     */
    typedef unsigned long int size_t;

    /*!
     * @brief Signed integer type of the result of subtracting two pointers.
     * 
     * The type of the result of subtracting two pointers is implementation-defined.    
     * This type is used to represent the result of subtracting two pointers.
     */
    typedef long int ssize_t;
    /*!
     * @brief Signed integer type of the result of adding two pointers.
     *
     * The type of the result of adding two pointers is implementation-defined.
     * This type is used to represent the result of adding two pointers.
     */
    typedef long int intptr_t;

    /*!
     * @brief Unsigned integer type of the result of adding two pointers.
     *
     * The type of the result of adding two pointers is implementation-defined.
     * This type is used to represent the result of adding two pointers.
     */
    typedef unsigned long int uintptr_t;

    /*!
     * @brief Type of the result of the division of two integers.
     *
     * The type of the result of the division of two integers is implementation-defined.
     * This type is used to represent the result of the division of two integers.
     */
    typedef long int intmax_t;

    /*!
     * @brief Type of the result of the division of two integers.
     *
     * The type of the result of the division of two integers is implementation-defined.
     * This type is used to represent the result of the division of two integers.
     */
    typedef unsigned long int uintmax_t;
} // namespace std

/*!
 * @brief The macro for a null pointer constant.
 *
 * This macro is used to represent a null pointer constant.
 */
#ifndef NULL
#define NULL nullptr
#endif

using size_t = std::size_t;
using ssize_t = std::ssize_t;
using ptrdiff_t = std::ptrdiff_t;
using intptr_t = std::intptr_t;
using uintptr_t = std::uintptr_t;
using intmax_t = std::intmax_t;
using uintmax_t = std::uintmax_t;

#endif // STDDEF_H
