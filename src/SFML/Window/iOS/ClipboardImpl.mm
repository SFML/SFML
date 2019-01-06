////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2019 Laurent Gomila (laurent@sfml-dev.org)
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
#include <SFML/Window/iOS/ClipboardImpl.hpp>

#import <UIKit/UIKit.h>

namespace sf
{
namespace priv
{

////////////////////////////////////////////////////////////
String ClipboardImpl::getString()
{
    UIPasteboard* pboard = [UIPasteboard generalPasteboard];
    if (pboard.hasStrings)
    {
        NSString* data = pboard.string;

        char const* utf8 = [data cStringUsingEncoding:NSUTF8StringEncoding];
        NSUInteger length = [data lengthOfBytesUsingEncoding:NSUTF8StringEncoding];

        return String::fromUtf8(utf8, utf8 + length);
    }
    else
    {
        return String();
    }
}


////////////////////////////////////////////////////////////
void ClipboardImpl::setString(const String& text)
{
    std::basic_string<Uint8> utf8 = text.toUtf8();
    NSString* data = [[NSString alloc] initWithBytes:utf8.data()
                                              length:utf8.length()
                                            encoding:NSUTF8StringEncoding];

    UIPasteboard* pboard = [UIPasteboard generalPasteboard];
    pboard.string = data;
}

} // namespace priv

} // namespace sf

