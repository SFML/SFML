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
#include <SFML/Window/Win32/CursorImpl.hpp>

#include <SFML/System/Err.hpp>
#include <SFML/System/Win32/WindowsHeader.hpp>

#include <ostream>

#include <cstring>


namespace sf::priv
{
////////////////////////////////////////////////////////////
CursorImpl::~CursorImpl()
{
    release();
}


////////////////////////////////////////////////////////////
bool CursorImpl::loadFromPixels(const std::uint8_t* pixels, Vector2u size, Vector2u hotspot)
{
    release();

    // Create the bitmap that will hold our color data
    auto bitmapHeader           = BITMAPV5HEADER();
    bitmapHeader.bV5Size        = sizeof(BITMAPV5HEADER);
    bitmapHeader.bV5Width       = static_cast<LONG>(size.x);
    bitmapHeader.bV5Height      = -static_cast<LONG>(size.y); // Negative indicates origin is in upper-left corner
    bitmapHeader.bV5Planes      = 1;
    bitmapHeader.bV5BitCount    = 32;
    bitmapHeader.bV5Compression = BI_BITFIELDS;
    bitmapHeader.bV5RedMask     = 0x00ff0000;
    bitmapHeader.bV5GreenMask   = 0x0000ff00;
    bitmapHeader.bV5BlueMask    = 0x000000ff;
    bitmapHeader.bV5AlphaMask   = 0xff000000;

    std::uint32_t* bitmapData = nullptr;

    HDC     screenDC = GetDC(nullptr);
    HBITMAP color    = CreateDIBSection(screenDC,
                                     reinterpret_cast<const BITMAPINFO*>(&bitmapHeader),
                                     DIB_RGB_COLORS,
                                     reinterpret_cast<void**>(&bitmapData),
                                     nullptr,
                                     0);
    ReleaseDC(nullptr, screenDC);

    if (!color)
    {
        err() << "Failed to create cursor color bitmap" << std::endl;
        return false;
    }

    // Fill our bitmap with the cursor color data
    // We'll have to swap the red and blue channels here
    std::uint32_t* bitmapOffset = bitmapData;
    for (std::size_t remaining = size.x * size.y; remaining > 0; --remaining, pixels += 4)
    {
        *bitmapOffset++ = static_cast<std::uint32_t>((pixels[3] << 24) | (pixels[0] << 16) | (pixels[1] << 8) | pixels[2]);
    }

    // Create a dummy mask bitmap (it won't be used)
    HBITMAP mask = CreateBitmap(static_cast<int>(size.x), static_cast<int>(size.y), 1, 1, nullptr);

    if (!mask)
    {
        DeleteObject(color);
        err() << "Failed to create cursor mask bitmap" << std::endl;
        return false;
    }

    // Create the structure that describes our cursor
    auto cursorInfo     = ICONINFO();
    cursorInfo.fIcon    = FALSE; // This is a cursor and not an icon
    cursorInfo.xHotspot = hotspot.x;
    cursorInfo.yHotspot = hotspot.y;
    cursorInfo.hbmColor = color;
    cursorInfo.hbmMask  = mask;

    // Create the cursor
    m_cursor       = reinterpret_cast<HCURSOR>(CreateIconIndirect(&cursorInfo));
    m_systemCursor = false;

    // The data has been copied into the cursor, so get rid of these
    DeleteObject(color);
    DeleteObject(mask);

    if (m_cursor)
    {
        return true;
    }
    else
    {
        err() << "Failed to create cursor from bitmaps" << std::endl;
        return false;
    }
}


////////////////////////////////////////////////////////////
bool CursorImpl::loadFromSystem(Cursor::Type type)
{
    release();

    LPCTSTR shape = nullptr;

    // clang-format off
    switch (type)
    {
        case Cursor::Arrow:                  shape = IDC_ARROW;       break;
        case Cursor::ArrowWait:              shape = IDC_APPSTARTING; break;
        case Cursor::Wait:                   shape = IDC_WAIT;        break;
        case Cursor::Text:                   shape = IDC_IBEAM;       break;
        case Cursor::Hand:                   shape = IDC_HAND;        break;
        case Cursor::SizeHorizontal:         shape = IDC_SIZEWE;      break;
        case Cursor::SizeVertical:           shape = IDC_SIZENS;      break;
        case Cursor::SizeTopLeftBottomRight: shape = IDC_SIZENWSE;    break;
        case Cursor::SizeBottomLeftTopRight: shape = IDC_SIZENESW;    break;
        case Cursor::SizeLeft:               shape = IDC_SIZEWE;      break;
        case Cursor::SizeRight:              shape = IDC_SIZEWE;      break;
        case Cursor::SizeTop:                shape = IDC_SIZENS;      break;
        case Cursor::SizeBottom:             shape = IDC_SIZENS;      break;
        case Cursor::SizeTopLeft:            shape = IDC_SIZENWSE;    break;
        case Cursor::SizeBottomRight:        shape = IDC_SIZENWSE;    break;
        case Cursor::SizeBottomLeft:         shape = IDC_SIZENESW;    break;
        case Cursor::SizeTopRight:           shape = IDC_SIZENESW;    break;
        case Cursor::SizeAll:                shape = IDC_SIZEALL;     break;
        case Cursor::Cross:                  shape = IDC_CROSS;       break;
        case Cursor::Help:                   shape = IDC_HELP;        break;
        case Cursor::NotAllowed:             shape = IDC_NO;          break;
    }
    // clang-format on

    // Get the shared system cursor and make sure not to destroy it
    m_cursor       = LoadCursor(nullptr, shape);
    m_systemCursor = true;

    if (m_cursor)
    {
        return true;
    }
    else
    {
        err() << "Could not create copy of a system cursor" << std::endl;
        return false;
    }
}


////////////////////////////////////////////////////////////
void CursorImpl::release()
{
    if (m_cursor && !m_systemCursor)
    {
        DestroyCursor(static_cast<HCURSOR>(m_cursor));
        m_cursor = nullptr;
    }
}

} // namespace sf::priv
