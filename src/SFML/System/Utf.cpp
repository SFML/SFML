////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2009 Laurent Gomila (laurent.gom@gmail.com)
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

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/System/Utf.hpp>
#include <exception>


////////////////////////////////////////////////////////////
// References :
//
// http://www.unicode.org/
// http://www.unicode.org/Public/PROGRAMS/CVTUTF/ConvertUTF.c
// http://www.unicode.org/Public/PROGRAMS/CVTUTF/ConvertUTF.h
// http://people.w3.org/rishida/scripts/uniview/conversion
//
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Private data
////////////////////////////////////////////////////////////
namespace
{
    // Get the current global locale
    std::locale GetCurrentLocale()
    {
        try
        {
            return std::locale("");
        }
        catch (std::exception&)
        {
            // It seems that some implementations don't know the "" locale (Mac OS X, MinGW)
            return std::locale();
        }
    }
}

namespace sf
{
////////////////////////////////////////////////////////////
/// Get the default system locale
////////////////////////////////////////////////////////////
const std::locale& GetDefaultLocale()
{
    // It seems that getting the default locale is a very expensive operation,
    // so we only do it once and then store the locale for reuse.
    // Warning: this code won't be aware of any change of the default locale during runtime.

    static std::locale locale = GetCurrentLocale();

    return locale;
}

} // namespace sf
