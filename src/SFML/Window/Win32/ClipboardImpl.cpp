////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2023 Laurent Gomila (laurent@sfml-dev.org)
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
#include <SFML/Window/Win32/ClipboardImpl.hpp>
#include <SFML/System/String.hpp>
#include <iostream>
#include <windows.h>


namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
String ClipboardImpl::getString()
{
    String text;

    if (!IsClipboardFormatAvailable(CF_UNICODETEXT))
    {
        std::cerr << "Failed to get the clipboard data in Unicode format." << std::endl;
        return text;
    }

    if (!OpenClipboard(NULL))
    {
        std::cerr << "Failed to open the Win32 clipboard." << std::endl;
        return text;
    }

    HANDLE clipboard_handle = GetClipboardData(CF_UNICODETEXT);

    if (!clipboard_handle)
    {
        std::cerr << "Failed to get Win32 handle for clipboard content." << std::endl;
        CloseClipboard();
        return text;
    }

    text = String(static_cast<wchar_t*>(GlobalLock(clipboard_handle)));
    GlobalUnlock(clipboard_handle);

    CloseClipboard();
    return text;
}


////////////////////////////////////////////////////////////
void ClipboardImpl::setString(const String& text)
{
    if (!OpenClipboard(NULL))
    {
        std::cerr << "Failed to open the Win32 clipboard." << std::endl;
        return;
    }

    if (!EmptyClipboard())
    {
        std::cerr << "Failed to empty the Win32 clipboard." << std::endl;
        return;
    }

    // Create a Win32-compatible string
    size_t string_size = (text.getSize() + 1) * sizeof(WCHAR);
    HANDLE string_handle = GlobalAlloc(GMEM_MOVEABLE, string_size);

    if (string_handle)
    {
        memcpy(GlobalLock(string_handle), text.toWideString().data(), string_size);
        GlobalUnlock(string_handle);
        SetClipboardData(CF_UNICODETEXT, string_handle);
    }

    CloseClipboard();
}

} // namespace priv

} // namespace sf
