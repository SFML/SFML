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

#include <SFML/System/Err.hpp>
#include <SFML/System/String.hpp>
#include <SFML/System/Win32/WindowsHeader.hpp>

#include <ostream>


namespace sf::priv
{
////////////////////////////////////////////////////////////
String ClipboardImpl::getString()
{
    String text;

    if (!IsClipboardFormatAvailable(CF_UNICODETEXT))
    {
        err() << "Failed to get the clipboard data in Unicode format." << std::endl;
        return text;
    }

    if (!OpenClipboard(nullptr))
    {
        err() << "Failed to open the Win32 clipboard." << std::endl;
        return text;
    }

    HANDLE clipboardHandle = GetClipboardData(CF_UNICODETEXT);

    if (!clipboardHandle)
    {
        err() << "Failed to get Win32 handle for clipboard content." << std::endl;
        CloseClipboard();
        return text;
    }

    text = String(static_cast<wchar_t*>(GlobalLock(clipboardHandle)));
    GlobalUnlock(clipboardHandle);

    CloseClipboard();
    return text;
}


////////////////////////////////////////////////////////////
void ClipboardImpl::setString(const String& text)
{
    if (!OpenClipboard(nullptr))
    {
        err() << "Failed to open the Win32 clipboard." << std::endl;
        return;
    }

    if (!EmptyClipboard())
    {
        err() << "Failed to empty the Win32 clipboard." << std::endl;
        return;
    }

    // Create a Win32-compatible string
    const std::size_t stringSize   = (text.getSize() + 1) * sizeof(WCHAR);
    HANDLE            stringHandle = GlobalAlloc(GMEM_MOVEABLE, stringSize);

    if (stringHandle)
    {
        memcpy(GlobalLock(stringHandle), text.toWideString().data(), stringSize);
        GlobalUnlock(stringHandle);
        SetClipboardData(CF_UNICODETEXT, stringHandle);
    }

    CloseClipboard();
}

} // namespace sf::priv
