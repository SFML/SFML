//
// detail/config.hpp
// ~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef NET_TS_DETAIL_CONFIG_HPP
#define NET_TS_DETAIL_CONFIG_HPP

#define NET_TS_STANDALONE 1

#if defined(NET_TS_STANDALONE)
# define NET_TS_DISABLE_BOOST_ARRAY 1
# define NET_TS_DISABLE_BOOST_ASSERT 1
# define NET_TS_DISABLE_BOOST_BIND 1
# define NET_TS_DISABLE_BOOST_CHRONO 1
# define NET_TS_DISABLE_BOOST_DATE_TIME 1
# define NET_TS_DISABLE_BOOST_LIMITS 1
# define NET_TS_DISABLE_BOOST_REGEX 1
# define NET_TS_DISABLE_BOOST_STATIC_CONSTANT 1
# define NET_TS_DISABLE_BOOST_THROW_EXCEPTION 1
# define NET_TS_DISABLE_BOOST_WORKAROUND 1
#else // defined(NET_TS_STANDALONE)
# include <boost/config.hpp>
# include <boost/version.hpp>
# define NET_TS_HAS_BOOST_CONFIG 1
#endif // defined(NET_TS_STANDALONE)

// Default to a header-only implementation. The user must specifically request
// separate compilation by defining either NET_TS_SEPARATE_COMPILATION or
// NET_TS_DYN_LINK (as a DLL/shared library implies separate compilation).
#if !defined(NET_TS_HEADER_ONLY)
# if !defined(NET_TS_SEPARATE_COMPILATION)
#  if !defined(NET_TS_DYN_LINK)
#   define NET_TS_HEADER_ONLY 1
#  endif // !defined(NET_TS_DYN_LINK)
# endif // !defined(NET_TS_SEPARATE_COMPILATION)
#endif // !defined(NET_TS_HEADER_ONLY)

#if defined(NET_TS_HEADER_ONLY)
# define NET_TS_DECL inline
#else // defined(NET_TS_HEADER_ONLY)
# if defined(_MSC_VER) || defined(__BORLANDC__) || defined(__CODEGEARC__)
// We need to import/export our code only if the user has specifically asked
// for it by defining NET_TS_DYN_LINK.
#  if defined(NET_TS_DYN_LINK)
// Export if this is our own source, otherwise import.
#   if defined(NET_TS_SOURCE)
#    define NET_TS_DECL __declspec(dllexport)
#   else // defined(NET_TS_SOURCE)
#    define NET_TS_DECL __declspec(dllimport)
#   endif // defined(NET_TS_SOURCE)
#  endif // defined(NET_TS_DYN_LINK)
# endif // defined(_MSC_VER) || defined(__BORLANDC__) || defined(__CODEGEARC__)
#endif // defined(NET_TS_HEADER_ONLY)

// If NET_TS_DECL isn't defined yet define it now.
#if !defined(NET_TS_DECL)
# define NET_TS_DECL
#endif // !defined(NET_TS_DECL)

// Microsoft Visual C++ detection.
#if !defined(NET_TS_MSVC)
# if defined(NET_TS_HAS_BOOST_CONFIG) && defined(BOOST_MSVC)
#  define NET_TS_MSVC BOOST_MSVC
# elif defined(_MSC_VER) && (defined(__INTELLISENSE__) \
      || (!defined(__MWERKS__) && !defined(__EDG_VERSION__)))
#  define NET_TS_MSVC _MSC_VER
# endif // defined(NET_TS_HAS_BOOST_CONFIG) && defined(BOOST_MSVC)
#endif // defined(NET_TS_MSVC)

// Clang / libc++ detection.
#if defined(__clang__)
# if (__cplusplus >= 201103)
#  if __has_include(<__config>)
#   include <__config>
#   if defined(_LIBCPP_VERSION)
#    define NET_TS_HAS_CLANG_LIBCXX 1
#   endif // defined(_LIBCPP_VERSION)
#  endif // __has_include(<__config>)
# endif // (__cplusplus >= 201103)
#endif // defined(__clang__)

// Android platform detection.
#if defined(__ANDROID__)
# include <android/api-level.h>
#endif // defined(__ANDROID__)

// Support move construction and assignment on compilers known to allow it.
#if !defined(NET_TS_HAS_MOVE)
# if !defined(NET_TS_DISABLE_MOVE)
#  if defined(__clang__)
#   if __has_feature(__cxx_rvalue_references__)
#    define NET_TS_HAS_MOVE 1
#   endif // __has_feature(__cxx_rvalue_references__)
#  endif // defined(__clang__)
#  if defined(__GNUC__)
#   if ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 5)) || (__GNUC__ > 4)
#    if defined(__GXX_EXPERIMENTAL_CXX0X__)
#     define NET_TS_HAS_MOVE 1
#    endif // defined(__GXX_EXPERIMENTAL_CXX0X__)
#   endif // ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 5)) || (__GNUC__ > 4)
#  endif // defined(__GNUC__)
#  if defined(NET_TS_MSVC)
#   if (_MSC_VER >= 1700)
#    define NET_TS_HAS_MOVE 1
#   endif // (_MSC_VER >= 1700)
#  endif // defined(NET_TS_MSVC)
# endif // !defined(NET_TS_DISABLE_MOVE)
#endif // !defined(NET_TS_HAS_MOVE)

// If NET_TS_MOVE_CAST isn't defined, and move support is available, define
// NET_TS_MOVE_ARG and NET_TS_MOVE_CAST to take advantage of rvalue
// references and perfect forwarding.
#if defined(NET_TS_HAS_MOVE) && !defined(NET_TS_MOVE_CAST)
# define NET_TS_MOVE_ARG(type) type&&
# define NET_TS_MOVE_ARG2(type1, type2) type1, type2&&
# define NET_TS_MOVE_CAST(type) static_cast<type&&>
# define NET_TS_MOVE_CAST2(type1, type2) static_cast<type1, type2&&>
#endif // defined(NET_TS_HAS_MOVE) && !defined(NET_TS_MOVE_CAST)

// If NET_TS_MOVE_CAST still isn't defined, default to a C++03-compatible
// implementation. Note that older g++ and MSVC versions don't like it when you
// pass a non-member function through a const reference, so for most compilers
// we'll play it safe and stick with the old approach of passing the handler by
// value.
#if !defined(NET_TS_MOVE_CAST)
# if defined(__GNUC__)
#  if ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 1)) || (__GNUC__ > 4)
#   define NET_TS_MOVE_ARG(type) const type&
#  else // ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 1)) || (__GNUC__ > 4)
#   define NET_TS_MOVE_ARG(type) type
#  endif // ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 1)) || (__GNUC__ > 4)
# elif defined(NET_TS_MSVC)
#  if (_MSC_VER >= 1400)
#   define NET_TS_MOVE_ARG(type) const type&
#  else // (_MSC_VER >= 1400)
#   define NET_TS_MOVE_ARG(type) type
#  endif // (_MSC_VER >= 1400)
# else
#  define NET_TS_MOVE_ARG(type) type
# endif
# define NET_TS_MOVE_CAST(type) static_cast<const type&>
# define NET_TS_MOVE_CAST2(type1, type2) static_cast<const type1, type2&>
#endif // !defined(NET_TS_MOVE_CAST)

// Support variadic templates on compilers known to allow it.
#if !defined(NET_TS_HAS_VARIADIC_TEMPLATES)
# if !defined(NET_TS_DISABLE_VARIADIC_TEMPLATES)
#  if defined(__clang__)
#   if __has_feature(__cxx_variadic_templates__)
#    define NET_TS_HAS_VARIADIC_TEMPLATES 1
#   endif // __has_feature(__cxx_variadic_templates__)
#  endif // defined(__clang__)
#  if defined(__GNUC__)
#   if ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 3)) || (__GNUC__ > 4)
#    if defined(__GXX_EXPERIMENTAL_CXX0X__)
#     define NET_TS_HAS_VARIADIC_TEMPLATES 1
#    endif // defined(__GXX_EXPERIMENTAL_CXX0X__)
#   endif // ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 3)) || (__GNUC__ > 4)
#  endif // defined(__GNUC__)
#  if defined(NET_TS_MSVC)
#   if (_MSC_VER >= 1900)
#    define NET_TS_HAS_VARIADIC_TEMPLATES 1
#   endif // (_MSC_VER >= 1900)
#  endif // defined(NET_TS_MSVC)
# endif // !defined(NET_TS_DISABLE_VARIADIC_TEMPLATES)
#endif // !defined(NET_TS_HAS_VARIADIC_TEMPLATES)

// Support deleted functions on compilers known to allow it.
#if !defined(NET_TS_DELETED)
# if defined(__GNUC__)
#  if ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 7)) || (__GNUC__ > 4)
#   if defined(__GXX_EXPERIMENTAL_CXX0X__)
#    define NET_TS_DELETED = delete
#   endif // defined(__GXX_EXPERIMENTAL_CXX0X__)
#  endif // ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 7)) || (__GNUC__ > 4)
# endif // defined(__GNUC__)
# if defined(__clang__)
#  if __has_feature(__cxx_deleted_functions__)
#   define NET_TS_DELETED = delete
#  endif // __has_feature(__cxx_deleted_functions__)
# endif // defined(__clang__)
# if defined(NET_TS_MSVC)
#  if (_MSC_VER >= 1900)
#   define NET_TS_DELETED = delete
#  endif // (_MSC_VER >= 1900)
# endif // defined(NET_TS_MSVC)
# if !defined(NET_TS_DELETED)
#  define NET_TS_DELETED
# endif // !defined(NET_TS_DELETED)
#endif // !defined(NET_TS_DELETED)

// Support constexpr on compilers known to allow it.
#if !defined(NET_TS_HAS_CONSTEXPR)
# if !defined(NET_TS_DISABLE_CONSTEXPR)
#  if defined(__clang__)
#   if __has_feature(__cxx_constexpr__)
#    define NET_TS_HAS_CONSTEXPR 1
#   endif // __has_feature(__cxx_constexr__)
#  endif // defined(__clang__)
#  if defined(__GNUC__)
#   if ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 6)) || (__GNUC__ > 4)
#    if defined(__GXX_EXPERIMENTAL_CXX0X__)
#     define NET_TS_HAS_CONSTEXPR 1
#    endif // defined(__GXX_EXPERIMENTAL_CXX0X__)
#   endif // ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 6)) || (__GNUC__ > 4)
#  endif // defined(__GNUC__)
#  if defined(NET_TS_MSVC)
#   if (_MSC_VER >= 1900)
#    define NET_TS_HAS_CONSTEXPR 1
#   endif // (_MSC_VER >= 1900)
#  endif // defined(NET_TS_MSVC)
# endif // !defined(NET_TS_DISABLE_CONSTEXPR)
#endif // !defined(NET_TS_HAS_CONSTEXPR)
#if !defined(NET_TS_CONSTEXPR)
# if defined(NET_TS_HAS_CONSTEXPR)
#  define NET_TS_CONSTEXPR constexpr
# else // defined(NET_TS_HAS_CONSTEXPR)
#  define NET_TS_CONSTEXPR
# endif // defined(NET_TS_HAS_CONSTEXPR)
#endif // !defined(NET_TS_CONSTEXPR)

// Support noexcept on compilers known to allow it.
#if !defined(NET_TS_NOEXCEPT)
# if !defined(NET_TS_DISABLE_NOEXCEPT)
#  if (BOOST_VERSION >= 105300)
#   define NET_TS_NOEXCEPT BOOST_NOEXCEPT
#   define NET_TS_NOEXCEPT_OR_NOTHROW BOOST_NOEXCEPT_OR_NOTHROW
#  elif defined(__clang__)
#   if __has_feature(__cxx_noexcept__)
#    define NET_TS_NOEXCEPT noexcept(true)
#    define NET_TS_NOEXCEPT_OR_NOTHROW noexcept(true)
#   endif // __has_feature(__cxx_noexcept__)
#  elif defined(__GNUC__)
#   if ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 7)) || (__GNUC__ > 4)
#    if defined(__GXX_EXPERIMENTAL_CXX0X__)
#      define NET_TS_NOEXCEPT noexcept(true)
#      define NET_TS_NOEXCEPT_OR_NOTHROW noexcept(true)
#    endif // defined(__GXX_EXPERIMENTAL_CXX0X__)
#   endif // ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 7)) || (__GNUC__ > 4)
#  elif defined(NET_TS_MSVC)
#   if (_MSC_VER >= 1900)
#    define NET_TS_NOEXCEPT noexcept(true)
#    define NET_TS_NOEXCEPT_OR_NOTHROW noexcept(true)
#   endif // (_MSC_VER >= 1900)
#  endif // defined(NET_TS_MSVC)
# endif // !defined(NET_TS_DISABLE_NOEXCEPT)
# if !defined(NET_TS_NOEXCEPT)
#  define NET_TS_NOEXCEPT
# endif // !defined(NET_TS_NOEXCEPT)
# if !defined(NET_TS_NOEXCEPT_OR_NOTHROW)
#  define NET_TS_NOEXCEPT_OR_NOTHROW throw()
# endif // !defined(NET_TS_NOEXCEPT_OR_NOTHROW)
#endif // !defined(NET_TS_NOEXCEPT)

// Support automatic type deduction on compilers known to support it.
#if !defined(NET_TS_HAS_DECLTYPE)
# if !defined(NET_TS_DISABLE_DECLTYPE)
#  if defined(__clang__)
#   if __has_feature(__cxx_decltype__)
#    define NET_TS_HAS_DECLTYPE 1
#   endif // __has_feature(__cxx_decltype__)
#  endif // defined(__clang__)
#  if defined(__GNUC__)
#   if ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 6)) || (__GNUC__ > 4)
#    if defined(__GXX_EXPERIMENTAL_CXX0X__)
#     define NET_TS_HAS_DECLTYPE 1
#    endif // defined(__GXX_EXPERIMENTAL_CXX0X__)
#   endif // ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 6)) || (__GNUC__ > 4)
#  endif // defined(__GNUC__)
#  if defined(NET_TS_MSVC)
#   if (_MSC_VER >= 1700)
#    define NET_TS_HAS_DECLTYPE 1
#   endif // (_MSC_VER >= 1700)
#  endif // defined(NET_TS_MSVC)
# endif // !defined(NET_TS_DISABLE_DECLTYPE)
#endif // !defined(NET_TS_HAS_DECLTYPE)

// Support alias templates on compilers known to allow it.
#if !defined(NET_TS_HAS_ALIAS_TEMPLATES)
# if !defined(NET_TS_DISABLE_ALIAS_TEMPLATES)
#  if defined(__clang__)
#   if __has_feature(__cxx_alias_templates__)
#    define NET_TS_HAS_ALIAS_TEMPLATES 1
#   endif // __has_feature(__cxx_alias_templates__)
#  endif // defined(__clang__)
#  if defined(__GNUC__)
#   if ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 7)) || (__GNUC__ > 4)
#    if defined(__GXX_EXPERIMENTAL_CXX0X__)
#     define NET_TS_HAS_ALIAS_TEMPLATES 1
#    endif // defined(__GXX_EXPERIMENTAL_CXX0X__)
#   endif // ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 7)) || (__GNUC__ > 4)
#  endif // defined(__GNUC__)
#  if defined(NET_TS_MSVC)
#   if (_MSC_VER >= 1900)
#    define NET_TS_HAS_ALIAS_TEMPLATES 1
#   endif // (_MSC_VER >= 1900)
#  endif // defined(NET_TS_MSVC)
# endif // !defined(NET_TS_DISABLE_ALIAS_TEMPLATES)
#endif // !defined(NET_TS_HAS_ALIAS_TEMPLATES)

// Standard library support for system errors.
#if !defined(NET_TS_HAS_STD_SYSTEM_ERROR)
# if !defined(NET_TS_DISABLE_STD_SYSTEM_ERROR)
#  if defined(__clang__)
#   if defined(NET_TS_HAS_CLANG_LIBCXX)
#    define NET_TS_HAS_STD_SYSTEM_ERROR 1
#   elif (__cplusplus >= 201103)
#    if __has_include(<system_error>)
#     define NET_TS_HAS_STD_SYSTEM_ERROR 1
#    endif // __has_include(<system_error>)
#   endif // (__cplusplus >= 201103)
#  endif // defined(__clang__)
#  if defined(__GNUC__)
#   if ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 6)) || (__GNUC__ > 4)
#    if defined(__GXX_EXPERIMENTAL_CXX0X__)
#     define NET_TS_HAS_STD_SYSTEM_ERROR 1
#    endif // defined(__GXX_EXPERIMENTAL_CXX0X__)
#   endif // ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 6)) || (__GNUC__ > 4)
#  endif // defined(__GNUC__)
#  if defined(NET_TS_MSVC)
#   if (_MSC_VER >= 1700)
#    define NET_TS_HAS_STD_SYSTEM_ERROR 1
#   endif // (_MSC_VER >= 1700)
#  endif // defined(NET_TS_MSVC)
# endif // !defined(NET_TS_DISABLE_STD_SYSTEM_ERROR)
#endif // !defined(NET_TS_HAS_STD_SYSTEM_ERROR)

// Compliant C++11 compilers put noexcept specifiers on error_category members.
#if !defined(NET_TS_ERROR_CATEGORY_NOEXCEPT)
# if (BOOST_VERSION >= 105300)
#  define NET_TS_ERROR_CATEGORY_NOEXCEPT BOOST_NOEXCEPT
# elif defined(__clang__)
#  if __has_feature(__cxx_noexcept__)
#   define NET_TS_ERROR_CATEGORY_NOEXCEPT noexcept(true)
#  endif // __has_feature(__cxx_noexcept__)
# elif defined(__GNUC__)
#  if ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 7)) || (__GNUC__ > 4)
#   if defined(__GXX_EXPERIMENTAL_CXX0X__)
#     define NET_TS_ERROR_CATEGORY_NOEXCEPT noexcept(true)
#   endif // defined(__GXX_EXPERIMENTAL_CXX0X__)
#  endif // ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 7)) || (__GNUC__ > 4)
# elif defined(NET_TS_MSVC)
#  if (_MSC_VER >= 1900)
#   define NET_TS_ERROR_CATEGORY_NOEXCEPT noexcept(true)
#  endif // (_MSC_VER >= 1900)
# endif // defined(NET_TS_MSVC)
# if !defined(NET_TS_ERROR_CATEGORY_NOEXCEPT)
#  define NET_TS_ERROR_CATEGORY_NOEXCEPT
# endif // !defined(NET_TS_ERROR_CATEGORY_NOEXCEPT)
#endif // !defined(NET_TS_ERROR_CATEGORY_NOEXCEPT)

// Standard library support for arrays.
#if !defined(NET_TS_HAS_STD_ARRAY)
# if !defined(NET_TS_DISABLE_STD_ARRAY)
#  if defined(__clang__)
#   if defined(NET_TS_HAS_CLANG_LIBCXX)
#    define NET_TS_HAS_STD_ARRAY 1
#   elif (__cplusplus >= 201103)
#    if __has_include(<array>)
#     define NET_TS_HAS_STD_ARRAY 1
#    endif // __has_include(<array>)
#   endif // (__cplusplus >= 201103)
#  endif // defined(__clang__)
#  if defined(__GNUC__)
#   if ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 3)) || (__GNUC__ > 4)
#    if defined(__GXX_EXPERIMENTAL_CXX0X__)
#     define NET_TS_HAS_STD_ARRAY 1
#    endif // defined(__GXX_EXPERIMENTAL_CXX0X__)
#   endif // ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 3)) || (__GNUC__ > 4)
#  endif // defined(__GNUC__)
#  if defined(NET_TS_MSVC)
#   if (_MSC_VER >= 1600)
#    define NET_TS_HAS_STD_ARRAY 1
#   endif // (_MSC_VER >= 1600)
#  endif // defined(NET_TS_MSVC)
# endif // !defined(NET_TS_DISABLE_STD_ARRAY)
#endif // !defined(NET_TS_HAS_STD_ARRAY)

// Standard library support for shared_ptr and weak_ptr.
#if !defined(NET_TS_HAS_STD_SHARED_PTR)
# if !defined(NET_TS_DISABLE_STD_SHARED_PTR)
#  if defined(__clang__)
#   if defined(NET_TS_HAS_CLANG_LIBCXX)
#    define NET_TS_HAS_STD_SHARED_PTR 1
#   elif (__cplusplus >= 201103)
#    define NET_TS_HAS_STD_SHARED_PTR 1
#   endif // (__cplusplus >= 201103)
#  endif // defined(__clang__)
#  if defined(__GNUC__)
#   if ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 3)) || (__GNUC__ > 4)
#    if defined(__GXX_EXPERIMENTAL_CXX0X__)
#     define NET_TS_HAS_STD_SHARED_PTR 1
#    endif // defined(__GXX_EXPERIMENTAL_CXX0X__)
#   endif // ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 3)) || (__GNUC__ > 4)
#  endif // defined(__GNUC__)
#  if defined(NET_TS_MSVC)
#   if (_MSC_VER >= 1600)
#    define NET_TS_HAS_STD_SHARED_PTR 1
#   endif // (_MSC_VER >= 1600)
#  endif // defined(NET_TS_MSVC)
# endif // !defined(NET_TS_DISABLE_STD_SHARED_PTR)
#endif // !defined(NET_TS_HAS_STD_SHARED_PTR)

// Standard library support for allocator_arg_t.
#if !defined(NET_TS_HAS_STD_ALLOCATOR_ARG)
# if !defined(NET_TS_DISABLE_STD_ALLOCATOR_ARG)
#  if defined(__clang__)
#   if defined(NET_TS_HAS_CLANG_LIBCXX)
#    define NET_TS_HAS_STD_ALLOCATOR_ARG 1
#   elif (__cplusplus >= 201103)
#    define NET_TS_HAS_STD_ALLOCATOR_ARG 1
#   endif // (__cplusplus >= 201103)
#  endif // defined(__clang__)
#  if defined(__GNUC__)
#   if ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 6)) || (__GNUC__ > 4)
#    if defined(__GXX_EXPERIMENTAL_CXX0X__)
#     define NET_TS_HAS_STD_ALLOCATOR_ARG 1
#    endif // defined(__GXX_EXPERIMENTAL_CXX0X__)
#   endif // ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 6)) || (__GNUC__ > 4)
#  endif // defined(__GNUC__)
#  if defined(NET_TS_MSVC)
#   if (_MSC_VER >= 1600)
#    define NET_TS_HAS_STD_ALLOCATOR_ARG 1
#   endif // (_MSC_VER >= 1600)
#  endif // defined(NET_TS_MSVC)
# endif // !defined(NET_TS_DISABLE_STD_ALLOCATOR_ARG)
#endif // !defined(NET_TS_HAS_STD_ALLOCATOR_ARG)

// Standard library support for atomic operations.
#if !defined(NET_TS_HAS_STD_ATOMIC)
# if !defined(NET_TS_DISABLE_STD_ATOMIC)
#  if defined(__clang__)
#   if defined(NET_TS_HAS_CLANG_LIBCXX)
#    define NET_TS_HAS_STD_ATOMIC 1
#   elif (__cplusplus >= 201103)
#    if __has_include(<atomic>)
#     define NET_TS_HAS_STD_ATOMIC 1
#    endif // __has_include(<atomic>)
#   endif // (__cplusplus >= 201103)
#  endif // defined(__clang__)
#  if defined(__GNUC__)
#   if ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 7)) || (__GNUC__ > 4)
#    if defined(__GXX_EXPERIMENTAL_CXX0X__)
#     define NET_TS_HAS_STD_ATOMIC 1
#    endif // defined(__GXX_EXPERIMENTAL_CXX0X__)
#   endif // ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 7)) || (__GNUC__ > 4)
#  endif // defined(__GNUC__)
#  if defined(NET_TS_MSVC)
#   if (_MSC_VER >= 1700)
#    define NET_TS_HAS_STD_ATOMIC 1
#   endif // (_MSC_VER >= 1700)
#  endif // defined(NET_TS_MSVC)
# endif // !defined(NET_TS_DISABLE_STD_ATOMIC)
#endif // !defined(NET_TS_HAS_STD_ATOMIC)

// Standard library support for chrono. Some standard libraries (such as the
// libstdc++ shipped with gcc 4.6) provide monotonic_clock as per early C++0x
// drafts, rather than the eventually standardised name of steady_clock.
#if !defined(NET_TS_HAS_STD_CHRONO)
# if !defined(NET_TS_DISABLE_STD_CHRONO)
#  if defined(__clang__)
#   if defined(NET_TS_HAS_CLANG_LIBCXX)
#    define NET_TS_HAS_STD_CHRONO 1
#   elif (__cplusplus >= 201103)
#    if __has_include(<chrono>)
#     define NET_TS_HAS_STD_CHRONO 1
#    endif // __has_include(<chrono>)
#   endif // (__cplusplus >= 201103)
#  endif // defined(__clang__)
#  if defined(__GNUC__)
#   if ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 6)) || (__GNUC__ > 4)
#    if defined(__GXX_EXPERIMENTAL_CXX0X__)
#     define NET_TS_HAS_STD_CHRONO 1
#     if ((__GNUC__ == 4) && (__GNUC_MINOR__ == 6))
#      define NET_TS_HAS_STD_CHRONO_MONOTONIC_CLOCK 1
#     endif // ((__GNUC__ == 4) && (__GNUC_MINOR__ == 6))
#    endif // defined(__GXX_EXPERIMENTAL_CXX0X__)
#   endif // ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 6)) || (__GNUC__ > 4)
#  endif // defined(__GNUC__)
#  if defined(NET_TS_MSVC)
#   if (_MSC_VER >= 1700)
#    define NET_TS_HAS_STD_CHRONO 1
#   endif // (_MSC_VER >= 1700)
#  endif // defined(NET_TS_MSVC)
# endif // !defined(NET_TS_DISABLE_STD_CHRONO)
#endif // !defined(NET_TS_HAS_STD_CHRONO)

// Boost support for chrono.
#if !defined(NET_TS_HAS_BOOST_CHRONO)
# if !defined(NET_TS_DISABLE_BOOST_CHRONO)
#  if (BOOST_VERSION >= 104700)
#   define NET_TS_HAS_BOOST_CHRONO 1
#  endif // (BOOST_VERSION >= 104700)
# endif // !defined(NET_TS_DISABLE_BOOST_CHRONO)
#endif // !defined(NET_TS_HAS_BOOST_CHRONO)

// Some form of chrono library is available.
#if !defined(NET_TS_HAS_CHRONO)
# if defined(NET_TS_HAS_STD_CHRONO) \
    || defined(NET_TS_HAS_BOOST_CHRONO)
#  define NET_TS_HAS_CHRONO 1
# endif // defined(NET_TS_HAS_STD_CHRONO)
        // || defined(NET_TS_HAS_BOOST_CHRONO)
#endif // !defined(NET_TS_HAS_CHRONO)

// Boost support for the DateTime library.
#if !defined(NET_TS_HAS_BOOST_DATE_TIME)
# if !defined(NET_TS_DISABLE_BOOST_DATE_TIME)
#  define NET_TS_HAS_BOOST_DATE_TIME 1
# endif // !defined(NET_TS_DISABLE_BOOST_DATE_TIME)
#endif // !defined(NET_TS_HAS_BOOST_DATE_TIME)

// Standard library support for addressof.
#if !defined(NET_TS_HAS_STD_ADDRESSOF)
# if !defined(NET_TS_DISABLE_STD_ADDRESSOF)
#  if defined(__clang__)
#   if defined(NET_TS_HAS_CLANG_LIBCXX)
#    define NET_TS_HAS_STD_ADDRESSOF 1
#   elif (__cplusplus >= 201103)
#    define NET_TS_HAS_STD_ADDRESSOF 1
#   endif // (__cplusplus >= 201103)
#  endif // defined(__clang__)
#  if defined(__GNUC__)
#   if ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 6)) || (__GNUC__ > 4)
#    if defined(__GXX_EXPERIMENTAL_CXX0X__)
#     define NET_TS_HAS_STD_ADDRESSOF 1
#    endif // defined(__GXX_EXPERIMENTAL_CXX0X__)
#   endif // ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 6)) || (__GNUC__ > 4)
#  endif // defined(__GNUC__)
#  if defined(NET_TS_MSVC)
#   if (_MSC_VER >= 1700)
#    define NET_TS_HAS_STD_ADDRESSOF 1
#   endif // (_MSC_VER >= 1700)
#  endif // defined(NET_TS_MSVC)
# endif // !defined(NET_TS_DISABLE_STD_ADDRESSOF)
#endif // !defined(NET_TS_HAS_STD_ADDRESSOF)

// Standard library support for the function class.
#if !defined(NET_TS_HAS_STD_FUNCTION)
# if !defined(NET_TS_DISABLE_STD_FUNCTION)
#  if defined(__clang__)
#   if defined(NET_TS_HAS_CLANG_LIBCXX)
#    define NET_TS_HAS_STD_FUNCTION 1
#   elif (__cplusplus >= 201103)
#    define NET_TS_HAS_STD_FUNCTION 1
#   endif // (__cplusplus >= 201103)
#  endif // defined(__clang__)
#  if defined(__GNUC__)
#   if ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 5)) || (__GNUC__ > 4)
#    if defined(__GXX_EXPERIMENTAL_CXX0X__)
#     define NET_TS_HAS_STD_FUNCTION 1
#    endif // defined(__GXX_EXPERIMENTAL_CXX0X__)
#   endif // ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 5)) || (__GNUC__ > 4)
#  endif // defined(__GNUC__)
#  if defined(NET_TS_MSVC)
#   if (_MSC_VER >= 1700)
#    define NET_TS_HAS_STD_FUNCTION 1
#   endif // (_MSC_VER >= 1700)
#  endif // defined(NET_TS_MSVC)
# endif // !defined(NET_TS_DISABLE_STD_FUNCTION)
#endif // !defined(NET_TS_HAS_STD_FUNCTION)

// Standard library support for type traits.
#if !defined(NET_TS_HAS_STD_TYPE_TRAITS)
# if !defined(NET_TS_DISABLE_STD_TYPE_TRAITS)
#  if defined(__clang__)
#   if defined(NET_TS_HAS_CLANG_LIBCXX)
#    define NET_TS_HAS_STD_TYPE_TRAITS 1
#   elif (__cplusplus >= 201103)
#    if __has_include(<type_traits>)
#     define NET_TS_HAS_STD_TYPE_TRAITS 1
#    endif // __has_include(<type_traits>)
#   endif // (__cplusplus >= 201103)
#  endif // defined(__clang__)
#  if defined(__GNUC__)
#   if ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 5)) || (__GNUC__ > 4)
#    if defined(__GXX_EXPERIMENTAL_CXX0X__)
#     define NET_TS_HAS_STD_TYPE_TRAITS 1
#    endif // defined(__GXX_EXPERIMENTAL_CXX0X__)
#   endif // ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 5)) || (__GNUC__ > 4)
#  endif // defined(__GNUC__)
#  if defined(NET_TS_MSVC)
#   if (_MSC_VER >= 1700)
#    define NET_TS_HAS_STD_TYPE_TRAITS 1
#   endif // (_MSC_VER >= 1700)
#  endif // defined(NET_TS_MSVC)
# endif // !defined(NET_TS_DISABLE_STD_TYPE_TRAITS)
#endif // !defined(NET_TS_HAS_STD_TYPE_TRAITS)

// Standard library support for the nullptr_t type.
#if !defined(NET_TS_HAS_NULLPTR)
# if !defined(NET_TS_DISABLE_NULLPTR)
#  if defined(__clang__)
#   if __has_feature(__cxx_nullptr__)
#    define NET_TS_HAS_NULLPTR 1
#   endif // __has_feature(__cxx_rvalue_references__)
#  elif defined(__GNUC__)
#   if ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 6)) || (__GNUC__ > 4)
#    if defined(__GXX_EXPERIMENTAL_CXX0X__)
#     define NET_TS_HAS_NULLPTR 1
#    endif // defined(__GXX_EXPERIMENTAL_CXX0X__)
#   endif // ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 6)) || (__GNUC__ > 4)
#  endif // defined(__GNUC__)
#  if defined(NET_TS_MSVC)
#   if (_MSC_VER >= 1700)
#    define NET_TS_HAS_NULLPTR 1
#   endif // (_MSC_VER >= 1700)
#  endif // defined(NET_TS_MSVC)
# endif // !defined(NET_TS_DISABLE_NULLPTR)
#endif // !defined(NET_TS_HAS_NULLPTR)

// Standard library support for the C++11 allocator additions.
#if !defined(NET_TS_HAS_CXX11_ALLOCATORS)
# if !defined(NET_TS_DISABLE_CXX11_ALLOCATORS)
#  if defined(__clang__)
#   if defined(NET_TS_HAS_CLANG_LIBCXX)
#    define NET_TS_HAS_CXX11_ALLOCATORS 1
#   elif (__cplusplus >= 201103)
#    define NET_TS_HAS_CXX11_ALLOCATORS 1
#   endif // (__cplusplus >= 201103)
#  elif defined(__GNUC__)
#   if ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 7)) || (__GNUC__ > 4)
#    if defined(__GXX_EXPERIMENTAL_CXX0X__)
#     define NET_TS_HAS_CXX11_ALLOCATORS 1
#    endif // defined(__GXX_EXPERIMENTAL_CXX0X__)
#   endif // ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 7)) || (__GNUC__ > 4)
#  endif // defined(__GNUC__)
#  if defined(NET_TS_MSVC)
#   if (_MSC_VER >= 1800)
#    define NET_TS_HAS_CXX11_ALLOCATORS 1
#   endif // (_MSC_VER >= 1800)
#  endif // defined(NET_TS_MSVC)
# endif // !defined(NET_TS_DISABLE_CXX11_ALLOCATORS)
#endif // !defined(NET_TS_HAS_CXX11_ALLOCATORS)

// Standard library support for the cstdint header.
#if !defined(NET_TS_HAS_CSTDINT)
# if !defined(NET_TS_DISABLE_CSTDINT)
#  if defined(__clang__)
#   if defined(NET_TS_HAS_CLANG_LIBCXX)
#    define NET_TS_HAS_CSTDINT 1
#   elif (__cplusplus >= 201103)
#    define NET_TS_HAS_CSTDINT 1
#   endif // (__cplusplus >= 201103)
#  endif // defined(__clang__)
#  if defined(__GNUC__)
#   if ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 5)) || (__GNUC__ > 4)
#    if defined(__GXX_EXPERIMENTAL_CXX0X__)
#     define NET_TS_HAS_CSTDINT 1
#    endif // defined(__GXX_EXPERIMENTAL_CXX0X__)
#   endif // ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 5)) || (__GNUC__ > 4)
#  endif // defined(__GNUC__)
#  if defined(NET_TS_MSVC)
#   if (_MSC_VER >= 1700)
#    define NET_TS_HAS_CSTDINT 1
#   endif // (_MSC_VER >= 1700)
#  endif // defined(NET_TS_MSVC)
# endif // !defined(NET_TS_DISABLE_CSTDINT)
#endif // !defined(NET_TS_HAS_CSTDINT)

// Standard library support for the thread class.
#if !defined(NET_TS_HAS_STD_THREAD)
# if !defined(NET_TS_DISABLE_STD_THREAD)
#  if defined(__clang__)
#   if defined(NET_TS_HAS_CLANG_LIBCXX)
#    define NET_TS_HAS_STD_THREAD 1
#   elif (__cplusplus >= 201103)
#    if __has_include(<thread>)
#     define NET_TS_HAS_STD_THREAD 1
#    endif // __has_include(<thread>)
#   endif // (__cplusplus >= 201103)
#  endif // defined(__clang__)
#  if defined(__GNUC__)
#   if ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 7)) || (__GNUC__ > 4)
#    if defined(__GXX_EXPERIMENTAL_CXX0X__)
#     define NET_TS_HAS_STD_THREAD 1
#    endif // defined(__GXX_EXPERIMENTAL_CXX0X__)
#   endif // ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 7)) || (__GNUC__ > 4)
#  endif // defined(__GNUC__)
#  if defined(NET_TS_MSVC)
#   if (_MSC_VER >= 1700)
#    define NET_TS_HAS_STD_THREAD 1
#   endif // (_MSC_VER >= 1700)
#  endif // defined(NET_TS_MSVC)
# endif // !defined(NET_TS_DISABLE_STD_THREAD)
#endif // !defined(NET_TS_HAS_STD_THREAD)

// Standard library support for the mutex and condition variable classes.
#if !defined(NET_TS_HAS_STD_MUTEX_AND_CONDVAR)
# if !defined(NET_TS_DISABLE_STD_MUTEX_AND_CONDVAR)
#  if defined(__clang__)
#   if defined(NET_TS_HAS_CLANG_LIBCXX)
#    define NET_TS_HAS_STD_MUTEX_AND_CONDVAR 1
#   elif (__cplusplus >= 201103)
#    if __has_include(<mutex>)
#     define NET_TS_HAS_STD_MUTEX_AND_CONDVAR 1
#    endif // __has_include(<mutex>)
#   endif // (__cplusplus >= 201103)
#  endif // defined(__clang__)
#  if defined(__GNUC__)
#   if ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 7)) || (__GNUC__ > 4)
#    if defined(__GXX_EXPERIMENTAL_CXX0X__)
#     define NET_TS_HAS_STD_MUTEX_AND_CONDVAR 1
#    endif // defined(__GXX_EXPERIMENTAL_CXX0X__)
#   endif // ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 7)) || (__GNUC__ > 4)
#  endif // defined(__GNUC__)
#  if defined(NET_TS_MSVC)
#   if (_MSC_VER >= 1700)
#    define NET_TS_HAS_STD_MUTEX_AND_CONDVAR 1
#   endif // (_MSC_VER >= 1700)
#  endif // defined(NET_TS_MSVC)
# endif // !defined(NET_TS_DISABLE_STD_MUTEX_AND_CONDVAR)
#endif // !defined(NET_TS_HAS_STD_MUTEX_AND_CONDVAR)

// Standard library support for the call_once function.
#if !defined(NET_TS_HAS_STD_CALL_ONCE)
# if !defined(NET_TS_DISABLE_STD_CALL_ONCE)
#  if defined(__clang__)
#   if defined(NET_TS_HAS_CLANG_LIBCXX)
#    define NET_TS_HAS_STD_CALL_ONCE 1
#   elif (__cplusplus >= 201103)
#    if __has_include(<mutex>)
#     define NET_TS_HAS_STD_CALL_ONCE 1
#    endif // __has_include(<mutex>)
#   endif // (__cplusplus >= 201103)
#  endif // defined(__clang__)
#  if defined(__GNUC__)
#   if ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 7)) || (__GNUC__ > 4)
#    if defined(__GXX_EXPERIMENTAL_CXX0X__)
#     define NET_TS_HAS_STD_CALL_ONCE 1
#    endif // defined(__GXX_EXPERIMENTAL_CXX0X__)
#   endif // ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 7)) || (__GNUC__ > 4)
#  endif // defined(__GNUC__)
#  if defined(NET_TS_MSVC)
#   if (_MSC_VER >= 1700)
#    define NET_TS_HAS_STD_CALL_ONCE 1
#   endif // (_MSC_VER >= 1700)
#  endif // defined(NET_TS_MSVC)
# endif // !defined(NET_TS_DISABLE_STD_CALL_ONCE)
#endif // !defined(NET_TS_HAS_STD_CALL_ONCE)

// Standard library support for futures.
#if !defined(NET_TS_HAS_STD_FUTURE)
# if !defined(NET_TS_DISABLE_STD_FUTURE)
#  if defined(__clang__)
#   if defined(NET_TS_HAS_CLANG_LIBCXX)
#    define NET_TS_HAS_STD_FUTURE 1
#   elif (__cplusplus >= 201103)
#    if __has_include(<future>)
#     define NET_TS_HAS_STD_FUTURE 1
#    endif // __has_include(<mutex>)
#   endif // (__cplusplus >= 201103)
#  endif // defined(__clang__)
#  if defined(__GNUC__)
#   if ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 7)) || (__GNUC__ > 4)
#    if defined(__GXX_EXPERIMENTAL_CXX0X__)
#     define NET_TS_HAS_STD_FUTURE 1
#    endif // defined(__GXX_EXPERIMENTAL_CXX0X__)
#   endif // ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 7)) || (__GNUC__ > 4)
#  endif // defined(__GNUC__)
#  if defined(NET_TS_MSVC)
#   if (_MSC_VER >= 1700)
#    define NET_TS_HAS_STD_FUTURE 1
#   endif // (_MSC_VER >= 1700)
#  endif // defined(NET_TS_MSVC)
# endif // !defined(NET_TS_DISABLE_STD_FUTURE)
#endif // !defined(NET_TS_HAS_STD_FUTURE)

// Standard library support for experimental::string_view.
#if !defined(NET_TS_HAS_STD_STRING_VIEW)
# if !defined(NET_TS_DISABLE_STD_STRING_VIEW)
#  if defined(__clang__)
#   if (__cplusplus >= 201402)
#    if __has_include(<experimental/string_view>)
#     define NET_TS_HAS_STD_STRING_VIEW 1
#     define NET_TS_HAS_STD_EXPERIMENTAL_STRING_VIEW 1
#    endif // __has_include(<experimental/string_view>)
#   endif // (__cplusplus >= 201402)
#  endif // defined(__clang__)
#  if defined(__GNUC__)
#   if ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 9)) || (__GNUC__ > 4)
#    if (__cplusplus >= 201402)
#     define NET_TS_HAS_STD_STRING_VIEW 1
#     define NET_TS_HAS_STD_EXPERIMENTAL_STRING_VIEW 1
#    endif // (__cplusplus >= 201402)
#   endif // ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 7)) || (__GNUC__ > 4)
#  endif // defined(__GNUC__)
#  if defined(NET_TS_MSVC)
#   if (_MSC_VER >= 1910 && _HAS_CXX17)
#    define NET_TS_HAS_STD_STRING_VIEW
#   endif // (_MSC_VER >= 1910)
#  endif // defined(NET_TS_MSVC)
# endif // !defined(NET_TS_DISABLE_STD_STRING_VIEW)
#endif // !defined(NET_TS_HAS_STD_STRING_VIEW)

// Standard library support for iostream move construction and assignment.
#if !defined(NET_TS_HAS_STD_IOSTREAM_MOVE)
# if !defined(NET_TS_DISABLE_STD_IOSTREAM_MOVE)
#  if defined(__GNUC__)
#   if (__GNUC__ > 4)
#    if defined(__GXX_EXPERIMENTAL_CXX0X__)
#     define NET_TS_HAS_STD_IOSTREAM_MOVE 1
#    endif // defined(__GXX_EXPERIMENTAL_CXX0X__)
#   endif // (__GNUC__ > 4)
#  endif // defined(__GNUC__)
#  if defined(NET_TS_MSVC)
#   if (_MSC_VER >= 1700)
#    define NET_TS_HAS_STD_IOSTREAM_MOVE 1
#   endif // (_MSC_VER >= 1700)
#  endif // defined(NET_TS_MSVC)
# endif // !defined(NET_TS_DISABLE_STD_IOSTREAM_MOVE)
#endif // !defined(NET_TS_HAS_STD_IOSTREAM_MOVE)

// Windows App target. Windows but with a limited API.
#if !defined(NET_TS_WINDOWS_APP)
# if defined(_WIN32_WINNT) && (_WIN32_WINNT >= 0x0603)
#  include <winapifamily.h>
#  if WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_APP) \
   && !WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP)
#   define NET_TS_WINDOWS_APP 1
#  endif // WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_APP)
         // && !WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP)
# endif // defined(_WIN32_WINNT) && (_WIN32_WINNT >= 0x0603)
#endif // !defined(NET_TS_WINDOWS_APP)

// Legacy WinRT target. Windows App is preferred.
#if !defined(NET_TS_WINDOWS_RUNTIME)
# if !defined(NET_TS_WINDOWS_APP)
#  if defined(__cplusplus_winrt)
#   include <winapifamily.h>
#   if WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_APP) \
    && !WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP)
#    define NET_TS_WINDOWS_RUNTIME 1
#   endif // WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_APP)
          // && !WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP)
#  endif // defined(__cplusplus_winrt)
# endif // !defined(NET_TS_WINDOWS_APP)
#endif // !defined(NET_TS_WINDOWS_RUNTIME)

// Windows target. Excludes WinRT but includes Windows App targets.
#if !defined(NET_TS_WINDOWS)
# if !defined(NET_TS_WINDOWS_RUNTIME)
#  if defined(NET_TS_HAS_BOOST_CONFIG) && defined(BOOST_WINDOWS)
#   define NET_TS_WINDOWS 1
#  elif defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#   define NET_TS_WINDOWS 1
#  elif defined(NET_TS_WINDOWS_APP)
#   define NET_TS_WINDOWS 1
#  endif // defined(NET_TS_HAS_BOOST_CONFIG) && defined(BOOST_WINDOWS)
# endif // !defined(NET_TS_WINDOWS_RUNTIME)
#endif // !defined(NET_TS_WINDOWS)

// Windows: target OS version.
#if defined(NET_TS_WINDOWS) || defined(__CYGWIN__)
# if !defined(_WIN32_WINNT) && !defined(_WIN32_WINDOWS)
#  if defined(_MSC_VER) || defined(__BORLANDC__)
#   pragma message( \
  "Please define _WIN32_WINNT or _WIN32_WINDOWS appropriately. For example:\n"\
  "- add -D_WIN32_WINNT=0x0501 to the compiler command line; or\n"\
  "- add _WIN32_WINNT=0x0501 to your project's Preprocessor Definitions.\n"\
  "Assuming _WIN32_WINNT=0x0501 (i.e. Windows XP target).")
#  else // defined(_MSC_VER) || defined(__BORLANDC__)
#   warning Please define _WIN32_WINNT or _WIN32_WINDOWS appropriately.
#   warning For example, add -D_WIN32_WINNT=0x0501 to the compiler command line.
#   warning Assuming _WIN32_WINNT=0x0501 (i.e. Windows XP target).
#  endif // defined(_MSC_VER) || defined(__BORLANDC__)
#  define _WIN32_WINNT 0x0501
# endif // !defined(_WIN32_WINNT) && !defined(_WIN32_WINDOWS)
# if defined(_MSC_VER)
#  if defined(_WIN32) && !defined(WIN32)
#   if !defined(_WINSOCK2API_)
#    define WIN32 // Needed for correct types in winsock2.h
#   else // !defined(_WINSOCK2API_)
#    error Please define the macro WIN32 in your compiler options
#   endif // !defined(_WINSOCK2API_)
#  endif // defined(_WIN32) && !defined(WIN32)
# endif // defined(_MSC_VER)
# if defined(__BORLANDC__)
#  if defined(__WIN32__) && !defined(WIN32)
#   if !defined(_WINSOCK2API_)
#    define WIN32 // Needed for correct types in winsock2.h
#   else // !defined(_WINSOCK2API_)
#    error Please define the macro WIN32 in your compiler options
#   endif // !defined(_WINSOCK2API_)
#  endif // defined(__WIN32__) && !defined(WIN32)
# endif // defined(__BORLANDC__)
# if defined(__CYGWIN__)
#  if !defined(__USE_W32_SOCKETS)
#   error You must add -D__USE_W32_SOCKETS to your compiler options.
#  endif // !defined(__USE_W32_SOCKETS)
# endif // defined(__CYGWIN__)
#endif // defined(NET_TS_WINDOWS) || defined(__CYGWIN__)

// Windows: minimise header inclusion.
#if defined(NET_TS_WINDOWS) || defined(__CYGWIN__)
# if !defined(NET_TS_NO_WIN32_LEAN_AND_MEAN)
#  if !defined(WIN32_LEAN_AND_MEAN)
#   define WIN32_LEAN_AND_MEAN
#  endif // !defined(WIN32_LEAN_AND_MEAN)
# endif // !defined(NET_TS_NO_WIN32_LEAN_AND_MEAN)
#endif // defined(NET_TS_WINDOWS) || defined(__CYGWIN__)

// Windows: suppress definition of "min" and "max" macros.
#if defined(NET_TS_WINDOWS) || defined(__CYGWIN__)
# if !defined(NET_TS_NO_NOMINMAX)
#  if !defined(NOMINMAX)
#   define NOMINMAX 1
#  endif // !defined(NOMINMAX)
# endif // !defined(NET_TS_NO_NOMINMAX)
#endif // defined(NET_TS_WINDOWS) || defined(__CYGWIN__)

// Windows: IO Completion Ports.
#if !defined(NET_TS_HAS_IOCP)
# if defined(NET_TS_WINDOWS) || defined(__CYGWIN__)
#  if defined(_WIN32_WINNT) && (_WIN32_WINNT >= 0x0400)
#   if !defined(UNDER_CE) && !defined(NET_TS_WINDOWS_APP)
#    if !defined(NET_TS_DISABLE_IOCP)
#     define NET_TS_HAS_IOCP 1
#    endif // !defined(NET_TS_DISABLE_IOCP)
#   endif // !defined(UNDER_CE) && !defined(NET_TS_WINDOWS_APP)
#  endif // defined(_WIN32_WINNT) && (_WIN32_WINNT >= 0x0400)
# endif // defined(NET_TS_WINDOWS) || defined(__CYGWIN__)
#endif // !defined(NET_TS_HAS_IOCP)

// On POSIX (and POSIX-like) platforms we need to include unistd.h in order to
// get access to the various platform feature macros, e.g. to be able to test
// for threads support.
#if !defined(NET_TS_HAS_UNISTD_H)
# if !defined(NET_TS_HAS_BOOST_CONFIG)
#  if defined(unix) \
   || defined(__unix) \
   || defined(_XOPEN_SOURCE) \
   || defined(_POSIX_SOURCE) \
   || (defined(__MACH__) && defined(__APPLE__)) \
   || defined(__FreeBSD__) \
   || defined(__NetBSD__) \
   || defined(__OpenBSD__) \
   || defined(__linux__)
#   define NET_TS_HAS_UNISTD_H 1
#  endif
# endif // !defined(NET_TS_HAS_BOOST_CONFIG)
#endif // !defined(NET_TS_HAS_UNISTD_H)
#if defined(NET_TS_HAS_UNISTD_H)
# include <unistd.h>
#endif // defined(NET_TS_HAS_UNISTD_H)

// Linux: epoll, eventfd and timerfd.
#if defined(__linux__)
# include <linux/version.h>
# if !defined(NET_TS_HAS_EPOLL)
#  if !defined(NET_TS_DISABLE_EPOLL)
#   if LINUX_VERSION_CODE >= KERNEL_VERSION(2,5,45)
#    define NET_TS_HAS_EPOLL 1
#   endif // LINUX_VERSION_CODE >= KERNEL_VERSION(2,5,45)
#  endif // !defined(NET_TS_DISABLE_EPOLL)
# endif // !defined(NET_TS_HAS_EPOLL)
# if !defined(NET_TS_HAS_EVENTFD)
#  if !defined(NET_TS_DISABLE_EVENTFD)
#   if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,22)
#    define NET_TS_HAS_EVENTFD 1
#   endif // LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,22)
#  endif // !defined(NET_TS_DISABLE_EVENTFD)
# endif // !defined(NET_TS_HAS_EVENTFD)
# if !defined(NET_TS_HAS_TIMERFD)
#  if defined(NET_TS_HAS_EPOLL)
#   if (__GLIBC__ > 2) || (__GLIBC__ == 2 && __GLIBC_MINOR__ >= 8)
#    define NET_TS_HAS_TIMERFD 1
#   endif // (__GLIBC__ > 2) || (__GLIBC__ == 2 && __GLIBC_MINOR__ >= 8)
#  endif // defined(NET_TS_HAS_EPOLL)
# endif // !defined(NET_TS_HAS_TIMERFD)
#endif // defined(__linux__)

// Mac OS X, FreeBSD, NetBSD, OpenBSD: kqueue.
#if (defined(__MACH__) && defined(__APPLE__)) \
  || defined(__FreeBSD__) \
  || defined(__NetBSD__) \
  || defined(__OpenBSD__)
# if !defined(NET_TS_HAS_KQUEUE)
#  if !defined(NET_TS_DISABLE_KQUEUE)
#   define NET_TS_HAS_KQUEUE 1
#  endif // !defined(NET_TS_DISABLE_KQUEUE)
# endif // !defined(NET_TS_HAS_KQUEUE)
#endif // (defined(__MACH__) && defined(__APPLE__))
       //   || defined(__FreeBSD__)
       //   || defined(__NetBSD__)
       //   || defined(__OpenBSD__)

// Solaris: /dev/poll.
#if defined(__sun)
# if !defined(NET_TS_HAS_DEV_POLL)
#  if !defined(NET_TS_DISABLE_DEV_POLL)
#   define NET_TS_HAS_DEV_POLL 1
#  endif // !defined(NET_TS_DISABLE_DEV_POLL)
# endif // !defined(NET_TS_HAS_DEV_POLL)
#endif // defined(__sun)

// Serial ports.
#if !defined(NET_TS_HAS_SERIAL_PORT)
# if defined(NET_TS_HAS_IOCP) \
  || !defined(NET_TS_WINDOWS) \
  && !defined(NET_TS_WINDOWS_RUNTIME) \
  && !defined(__CYGWIN__)
#  if !defined(__SYMBIAN32__)
#   if !defined(NET_TS_DISABLE_SERIAL_PORT)
#    define NET_TS_HAS_SERIAL_PORT 1
#   endif // !defined(NET_TS_DISABLE_SERIAL_PORT)
#  endif // !defined(__SYMBIAN32__)
# endif // defined(NET_TS_HAS_IOCP)
        //   || !defined(NET_TS_WINDOWS)
        //   && !defined(NET_TS_WINDOWS_RUNTIME)
        //   && !defined(__CYGWIN__)
#endif // !defined(NET_TS_HAS_SERIAL_PORT)

// Windows: stream handles.
#if !defined(NET_TS_HAS_WINDOWS_STREAM_HANDLE)
# if !defined(NET_TS_DISABLE_WINDOWS_STREAM_HANDLE)
#  if defined(NET_TS_HAS_IOCP)
#   define NET_TS_HAS_WINDOWS_STREAM_HANDLE 1
#  endif // defined(NET_TS_HAS_IOCP)
# endif // !defined(NET_TS_DISABLE_WINDOWS_STREAM_HANDLE)
#endif // !defined(NET_TS_HAS_WINDOWS_STREAM_HANDLE)

// Windows: random access handles.
#if !defined(NET_TS_HAS_WINDOWS_RANDOM_ACCESS_HANDLE)
# if !defined(NET_TS_DISABLE_WINDOWS_RANDOM_ACCESS_HANDLE)
#  if defined(NET_TS_HAS_IOCP)
#   define NET_TS_HAS_WINDOWS_RANDOM_ACCESS_HANDLE 1
#  endif // defined(NET_TS_HAS_IOCP)
# endif // !defined(NET_TS_DISABLE_WINDOWS_RANDOM_ACCESS_HANDLE)
#endif // !defined(NET_TS_HAS_WINDOWS_RANDOM_ACCESS_HANDLE)

// Windows: object handles.
#if !defined(NET_TS_HAS_WINDOWS_OBJECT_HANDLE)
# if !defined(NET_TS_DISABLE_WINDOWS_OBJECT_HANDLE)
#  if defined(NET_TS_WINDOWS) || defined(__CYGWIN__)
#   if !defined(UNDER_CE) && !defined(NET_TS_WINDOWS_APP)
#    define NET_TS_HAS_WINDOWS_OBJECT_HANDLE 1
#   endif // !defined(UNDER_CE) && !defined(NET_TS_WINDOWS_APP)
#  endif // defined(NET_TS_WINDOWS) || defined(__CYGWIN__)
# endif // !defined(NET_TS_DISABLE_WINDOWS_OBJECT_HANDLE)
#endif // !defined(NET_TS_HAS_WINDOWS_OBJECT_HANDLE)

// Windows: OVERLAPPED wrapper.
#if !defined(NET_TS_HAS_WINDOWS_OVERLAPPED_PTR)
# if !defined(NET_TS_DISABLE_WINDOWS_OVERLAPPED_PTR)
#  if defined(NET_TS_HAS_IOCP)
#   define NET_TS_HAS_WINDOWS_OVERLAPPED_PTR 1
#  endif // defined(NET_TS_HAS_IOCP)
# endif // !defined(NET_TS_DISABLE_WINDOWS_OVERLAPPED_PTR)
#endif // !defined(NET_TS_HAS_WINDOWS_OVERLAPPED_PTR)

// POSIX: stream-oriented file descriptors.
#if !defined(NET_TS_HAS_POSIX_STREAM_DESCRIPTOR)
# if !defined(NET_TS_DISABLE_POSIX_STREAM_DESCRIPTOR)
#  if !defined(NET_TS_WINDOWS) \
  && !defined(NET_TS_WINDOWS_RUNTIME) \
  && !defined(__CYGWIN__)
#   define NET_TS_HAS_POSIX_STREAM_DESCRIPTOR 1
#  endif // !defined(NET_TS_WINDOWS)
         //   && !defined(NET_TS_WINDOWS_RUNTIME)
         //   && !defined(__CYGWIN__)
# endif // !defined(NET_TS_DISABLE_POSIX_STREAM_DESCRIPTOR)
#endif // !defined(NET_TS_HAS_POSIX_STREAM_DESCRIPTOR)

// UNIX domain sockets.
#if !defined(NET_TS_HAS_LOCAL_SOCKETS)
# if !defined(NET_TS_DISABLE_LOCAL_SOCKETS)
#  if !defined(NET_TS_WINDOWS) \
  && !defined(NET_TS_WINDOWS_RUNTIME) \
  && !defined(__CYGWIN__)
#   define NET_TS_HAS_LOCAL_SOCKETS 1
#  endif // !defined(NET_TS_WINDOWS)
         //   && !defined(NET_TS_WINDOWS_RUNTIME)
         //   && !defined(__CYGWIN__)
# endif // !defined(NET_TS_DISABLE_LOCAL_SOCKETS)
#endif // !defined(NET_TS_HAS_LOCAL_SOCKETS)

// Can use sigaction() instead of signal().
#if !defined(NET_TS_HAS_SIGACTION)
# if !defined(NET_TS_DISABLE_SIGACTION)
#  if !defined(NET_TS_WINDOWS) \
  && !defined(NET_TS_WINDOWS_RUNTIME) \
  && !defined(__CYGWIN__)
#   define NET_TS_HAS_SIGACTION 1
#  endif // !defined(NET_TS_WINDOWS)
         //   && !defined(NET_TS_WINDOWS_RUNTIME)
         //   && !defined(__CYGWIN__)
# endif // !defined(NET_TS_DISABLE_SIGACTION)
#endif // !defined(NET_TS_HAS_SIGACTION)

// Can use signal().
#if !defined(NET_TS_HAS_SIGNAL)
# if !defined(NET_TS_DISABLE_SIGNAL)
#  if !defined(UNDER_CE)
#   define NET_TS_HAS_SIGNAL 1
#  endif // !defined(UNDER_CE)
# endif // !defined(NET_TS_DISABLE_SIGNAL)
#endif // !defined(NET_TS_HAS_SIGNAL)

// Can use getaddrinfo() and getnameinfo().
#if !defined(NET_TS_HAS_GETADDRINFO)
# if !defined(NET_TS_DISABLE_GETADDRINFO)
#  if defined(NET_TS_WINDOWS) || defined(__CYGWIN__)
#   if defined(_WIN32_WINNT) && (_WIN32_WINNT >= 0x0501)
#    define NET_TS_HAS_GETADDRINFO 1
#   elif defined(UNDER_CE)
#    define NET_TS_HAS_GETADDRINFO 1
#   endif // defined(UNDER_CE)
#  elif defined(__MACH__) && defined(__APPLE__)
#   if defined(__MAC_OS_X_VERSION_MIN_REQUIRED)
#    if (__MAC_OS_X_VERSION_MIN_REQUIRED >= 1050)
#     define NET_TS_HAS_GETADDRINFO 1
#    endif // (__MAC_OS_X_VERSION_MIN_REQUIRED >= 1050)
#   else // defined(__MAC_OS_X_VERSION_MIN_REQUIRED)
#    define NET_TS_HAS_GETADDRINFO 1
#   endif // defined(__MAC_OS_X_VERSION_MIN_REQUIRED)
#  else // defined(__MACH__) && defined(__APPLE__)
#   define NET_TS_HAS_GETADDRINFO 1
#  endif // defined(__MACH__) && defined(__APPLE__)
# endif // !defined(NET_TS_DISABLE_GETADDRINFO)
#endif // !defined(NET_TS_HAS_GETADDRINFO)

// Whether standard iostreams are disabled.
#if !defined(NET_TS_NO_IOSTREAM)
# if defined(NET_TS_HAS_BOOST_CONFIG) && defined(BOOST_NO_IOSTREAM)
#  define NET_TS_NO_IOSTREAM 1
# endif // !defined(BOOST_NO_IOSTREAM)
#endif // !defined(NET_TS_NO_IOSTREAM)

// Whether exception handling is disabled.
#if !defined(NET_TS_NO_EXCEPTIONS)
# if defined(NET_TS_HAS_BOOST_CONFIG) && defined(BOOST_NO_EXCEPTIONS)
#  define NET_TS_NO_EXCEPTIONS 1
# endif // !defined(BOOST_NO_EXCEPTIONS)
#endif // !defined(NET_TS_NO_EXCEPTIONS)

// Whether the typeid operator is supported.
#if !defined(NET_TS_NO_TYPEID)
# if defined(NET_TS_HAS_BOOST_CONFIG) && defined(BOOST_NO_TYPEID)
#  define NET_TS_NO_TYPEID 1
# endif // !defined(BOOST_NO_TYPEID)
#endif // !defined(NET_TS_NO_TYPEID)

// Threads.
#if !defined(NET_TS_HAS_THREADS)
# if !defined(NET_TS_DISABLE_THREADS)
#  if defined(NET_TS_HAS_BOOST_CONFIG) && defined(BOOST_HAS_THREADS)
#   define NET_TS_HAS_THREADS 1
#  elif defined(__GNUC__) && !defined(__MINGW32__) \
     && !defined(linux) && !defined(__linux) && !defined(__linux__)
#   define NET_TS_HAS_THREADS 1
#  elif defined(_MT) || defined(__MT__)
#   define NET_TS_HAS_THREADS 1
#  elif defined(_REENTRANT)
#   define NET_TS_HAS_THREADS 1
#  elif defined(__APPLE__)
#   define NET_TS_HAS_THREADS 1
#  elif defined(_POSIX_THREADS) && (_POSIX_THREADS + 0 >= 0)
#   define NET_TS_HAS_THREADS 1
#  elif defined(_PTHREADS)
#   define NET_TS_HAS_THREADS 1
#  endif // defined(NET_TS_HAS_BOOST_CONFIG) && defined(BOOST_HAS_THREADS)
# endif // !defined(NET_TS_DISABLE_THREADS)
#endif // !defined(NET_TS_HAS_THREADS)

// POSIX threads.
#if !defined(NET_TS_HAS_PTHREADS)
# if defined(NET_TS_HAS_THREADS)
#  if defined(NET_TS_HAS_BOOST_CONFIG) && defined(BOOST_HAS_PTHREADS)
#   define NET_TS_HAS_PTHREADS 1
#  elif defined(_POSIX_THREADS) && (_POSIX_THREADS + 0 >= 0)
#   define NET_TS_HAS_PTHREADS 1
#  endif // defined(NET_TS_HAS_BOOST_CONFIG) && defined(BOOST_HAS_PTHREADS)
# endif // defined(NET_TS_HAS_THREADS)
#endif // !defined(NET_TS_HAS_PTHREADS)

// Helper to prevent macro expansion.
#define NET_TS_PREVENT_MACRO_SUBSTITUTION

// Helper to define in-class constants.
#if !defined(NET_TS_STATIC_CONSTANT)
# if !defined(NET_TS_DISABLE_BOOST_STATIC_CONSTANT)
#  define NET_TS_STATIC_CONSTANT(type, assignment) \
    BOOST_STATIC_CONSTANT(type, assignment)
# else // !defined(NET_TS_DISABLE_BOOST_STATIC_CONSTANT)
#  define NET_TS_STATIC_CONSTANT(type, assignment) \
    static const type assignment
# endif // !defined(NET_TS_DISABLE_BOOST_STATIC_CONSTANT)
#endif // !defined(NET_TS_STATIC_CONSTANT)

// Boost array library.
#if !defined(NET_TS_HAS_BOOST_ARRAY)
# if !defined(NET_TS_DISABLE_BOOST_ARRAY)
#  define NET_TS_HAS_BOOST_ARRAY 1
# endif // !defined(NET_TS_DISABLE_BOOST_ARRAY)
#endif // !defined(NET_TS_HAS_BOOST_ARRAY)

// Boost assert macro.
#if !defined(NET_TS_HAS_BOOST_ASSERT)
# if !defined(NET_TS_DISABLE_BOOST_ASSERT)
#  define NET_TS_HAS_BOOST_ASSERT 1
# endif // !defined(NET_TS_DISABLE_BOOST_ASSERT)
#endif // !defined(NET_TS_HAS_BOOST_ASSERT)

// Boost limits header.
#if !defined(NET_TS_HAS_BOOST_LIMITS)
# if !defined(NET_TS_DISABLE_BOOST_LIMITS)
#  define NET_TS_HAS_BOOST_LIMITS 1
# endif // !defined(NET_TS_DISABLE_BOOST_LIMITS)
#endif // !defined(NET_TS_HAS_BOOST_LIMITS)

// Boost throw_exception function.
#if !defined(NET_TS_HAS_BOOST_THROW_EXCEPTION)
# if !defined(NET_TS_DISABLE_BOOST_THROW_EXCEPTION)
#  define NET_TS_HAS_BOOST_THROW_EXCEPTION 1
# endif // !defined(NET_TS_DISABLE_BOOST_THROW_EXCEPTION)
#endif // !defined(NET_TS_HAS_BOOST_THROW_EXCEPTION)

// Boost regex library.
#if !defined(NET_TS_HAS_BOOST_REGEX)
# if !defined(NET_TS_DISABLE_BOOST_REGEX)
#  define NET_TS_HAS_BOOST_REGEX 1
# endif // !defined(NET_TS_DISABLE_BOOST_REGEX)
#endif // !defined(NET_TS_HAS_BOOST_REGEX)

// Boost bind function.
#if !defined(NET_TS_HAS_BOOST_BIND)
# if !defined(NET_TS_DISABLE_BOOST_BIND)
#  define NET_TS_HAS_BOOST_BIND 1
# endif // !defined(NET_TS_DISABLE_BOOST_BIND)
#endif // !defined(NET_TS_HAS_BOOST_BIND)

// Boost's BOOST_WORKAROUND macro.
#if !defined(NET_TS_HAS_BOOST_WORKAROUND)
# if !defined(NET_TS_DISABLE_BOOST_WORKAROUND)
#  define NET_TS_HAS_BOOST_WORKAROUND 1
# endif // !defined(NET_TS_DISABLE_BOOST_WORKAROUND)
#endif // !defined(NET_TS_HAS_BOOST_WORKAROUND)

// Microsoft Visual C++'s secure C runtime library.
#if !defined(NET_TS_HAS_SECURE_RTL)
# if !defined(NET_TS_DISABLE_SECURE_RTL)
#  if defined(NET_TS_MSVC) \
    && (NET_TS_MSVC >= 1400) \
    && !defined(UNDER_CE)
#   define NET_TS_HAS_SECURE_RTL 1
#  endif // defined(NET_TS_MSVC)
         // && (NET_TS_MSVC >= 1400)
         // && !defined(UNDER_CE)
# endif // !defined(NET_TS_DISABLE_SECURE_RTL)
#endif // !defined(NET_TS_HAS_SECURE_RTL)

// Handler hooking. Disabled for ancient Borland C++ and gcc compilers.
#if !defined(NET_TS_HAS_HANDLER_HOOKS)
# if !defined(NET_TS_DISABLE_HANDLER_HOOKS)
#  if defined(__GNUC__)
#   if (__GNUC__ >= 3)
#    define NET_TS_HAS_HANDLER_HOOKS 1
#   endif // (__GNUC__ >= 3)
#  elif !defined(__BORLANDC__)
#   define NET_TS_HAS_HANDLER_HOOKS 1
#  endif // !defined(__BORLANDC__)
# endif // !defined(NET_TS_DISABLE_HANDLER_HOOKS)
#endif // !defined(NET_TS_HAS_HANDLER_HOOKS)

// Support for the __thread keyword extension.
#if !defined(NET_TS_DISABLE_THREAD_KEYWORD_EXTENSION)
# if defined(__linux__)
#  if defined(__GNUC__) && (defined(__i386__) || defined(__x86_64__))
#   if ((__GNUC__ == 3) && (__GNUC_MINOR__ >= 3)) || (__GNUC__ > 3)
#    if !defined(__INTEL_COMPILER) && !defined(__ICL) \
       && !(defined(__clang__) && defined(__ANDROID__))
#     define NET_TS_HAS_THREAD_KEYWORD_EXTENSION 1
#     define NET_TS_THREAD_KEYWORD __thread
#    elif defined(__INTEL_COMPILER) && (__INTEL_COMPILER >= 1100)
#     define NET_TS_HAS_THREAD_KEYWORD_EXTENSION 1
#    endif // defined(__INTEL_COMPILER) && (__INTEL_COMPILER >= 1100)
           // && !(defined(__clang__) && defined(__ANDROID__))
#   endif // ((__GNUC__ == 3) && (__GNUC_MINOR__ >= 3)) || (__GNUC__ > 3)
#  endif // defined(__GNUC__) && (defined(__i386__) || defined(__x86_64__))
# endif // defined(__linux__)
# if defined(NET_TS_MSVC) && defined(NET_TS_WINDOWS_RUNTIME)
#  if (_MSC_VER >= 1700)
#   define NET_TS_HAS_THREAD_KEYWORD_EXTENSION 1
#   define NET_TS_THREAD_KEYWORD __declspec(thread)
#  endif // (_MSC_VER >= 1700)
# endif // defined(NET_TS_MSVC) && defined(NET_TS_WINDOWS_RUNTIME)
#endif // !defined(NET_TS_DISABLE_THREAD_KEYWORD_EXTENSION)
#if !defined(NET_TS_THREAD_KEYWORD)
# define NET_TS_THREAD_KEYWORD __thread
#endif // !defined(NET_TS_THREAD_KEYWORD)

// Support for POSIX ssize_t typedef.
#if !defined(NET_TS_DISABLE_SSIZE_T)
# if defined(__linux__) \
   || (defined(__MACH__) && defined(__APPLE__))
#  define NET_TS_HAS_SSIZE_T 1
# endif // defined(__linux__)
        //   || (defined(__MACH__) && defined(__APPLE__))
#endif // !defined(NET_TS_DISABLE_SSIZE_T)

// Helper macros to manage the transition away from the old services-based API.
#define NET_TS_SVC_TPARAM
#define NET_TS_SVC_TPARAM_DEF1(d1)
#define NET_TS_SVC_TPARAM_DEF2(d1, d2)
#define NET_TS_SVC_TARG
// NET_TS_SVC_T is defined at each point of use.
#define NET_TS_SVC_TPARAM1
#define NET_TS_SVC_TPARAM1_DEF1(d1)
#define NET_TS_SVC_TPARAM1_DEF2(d1, d2)
#define NET_TS_SVC_TARG1
// NET_TS_SVC_T1 is defined at each point of use.
#define NET_TS_SVC_ACCESS protected

// Helper macros to manage transition away from error_code return values.
#define NET_TS_SYNC_OP_VOID void
#define NET_TS_SYNC_OP_VOID_RETURN(e) return
// Newer gcc, clang need special treatment to suppress unused typedef warnings.
#if defined(__clang__)
# if defined(__apple_build_version__)
#  if (__clang_major__ >= 7)
#   define NET_TS_UNUSED_TYPEDEF __attribute__((__unused__))
#  endif // (__clang_major__ >= 7)
# elif ((__clang_major__ == 3) && (__clang_minor__ >= 6)) \
    || (__clang_major__ > 3)
#  define NET_TS_UNUSED_TYPEDEF __attribute__((__unused__))
# endif // ((__clang_major__ == 3) && (__clang_minor__ >= 6))
        //   || (__clang_major__ > 3)
#elif defined(__GNUC__)
# if ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 8)) || (__GNUC__ > 4)
#  define NET_TS_UNUSED_TYPEDEF __attribute__((__unused__))
# endif // ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 8)) || (__GNUC__ > 4)
#endif // defined(__GNUC__)
#if !defined(NET_TS_UNUSED_TYPEDEF)
# define NET_TS_UNUSED_TYPEDEF
#endif // !defined(NET_TS_UNUSED_TYPEDEF)

// Some versions of gcc generate spurious warnings about unused variables.
#if defined(__GNUC__)
# if (__GNUC__ >= 4)
#  define NET_TS_UNUSED_VARIABLE __attribute__((__unused__))
# endif // (__GNUC__ >= 4)
#endif // defined(__GNUC__)
#if !defined(NET_TS_UNUSED_VARIABLE)
# define NET_TS_UNUSED_VARIABLE
#endif // !defined(NET_TS_UNUSED_VARIABLE)

#endif // NET_TS_DETAIL_CONFIG_HPP
