////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2022 Laurent Gomila (laurent@sfml-dev.org)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
////////////////////////////////////////////////////////////

#ifndef SFML_SYSTEM_SPAN_HPP
#define SFML_SYSTEM_SPAN_HPP

////////////////////////////////////////////////////////////
// Include <span> if C++20 or greater is used.
// Otherwise, use an alternative implementation for older compilers.
////////////////////////////////////////////////////////////
#if __cplusplus >= 202002L || (defined(_MSVC_LANG) && _MSVC_LANG >= 202002L)

    #include <span>

#else

    #define TCB_SPAN_NAMESPACE_NAME std
    #include <SFML/System/detail/span.hpp>

#endif

#endif // SFML_SYSTEM_SPAN_HPP
