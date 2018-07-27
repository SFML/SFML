////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2018 Laurent Gomila (laurent@sfml-dev.org)
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
#include <SFML/Window/Unix/CursorImpl.hpp>
#include <SFML/Window/Unix/Display.hpp>
#include <X11/cursorfont.h>
#include <X11/Xutil.h>
#include <cassert>
#include <cstdlib>
#include <vector>

namespace sf
{
namespace priv
{

////////////////////////////////////////////////////////////
CursorImpl::CursorImpl() :
m_display(OpenDisplay()),
m_cursor(None)
{
    // That's it.
}


////////////////////////////////////////////////////////////
CursorImpl::~CursorImpl()
{
    release();

    CloseDisplay(m_display);
}


////////////////////////////////////////////////////////////
bool CursorImpl::loadFromPixels(const Uint8* pixels, Vector2u size, Vector2u hotspot)
{
    release();

    // Convert the image into a bitmap (monochrome!).
    std::size_t bytes = (size.x + 7) / 8 * size.y;
    std::vector<Uint8> mask(bytes, 0); // Defines which pixel is transparent.
    std::vector<Uint8> data(bytes, 1); // Defines which pixel is white/black.

    for (std::size_t j = 0; j < size.y; ++j)
    {
        for (std::size_t i = 0; i < size.x; ++i)
        {
            std::size_t pixelIndex = i + j * size.x;
            std::size_t byteIndex  = pixelIndex / 8;
            std::size_t bitIndex   = i % 8;

            // Turn on pixel that are not transparent
            Uint8 opacity = pixels[pixelIndex * 4 + 3] > 0 ? 1 : 0;
            mask[byteIndex] |= opacity << bitIndex;

            // Choose between black/background & white/foreground color for each pixel,
            // based on the pixel color intensity: on average, if a channel is "active"
            // at 25%, the bit is white.
            int intensity = pixels[pixelIndex * 4 + 0] + pixels[pixelIndex * 4 + 1] + pixels[pixelIndex * 4 + 2];
            Uint8 bit = intensity > 64 ? 1 : 0;
            data[byteIndex] |= bit << bitIndex;
        }
    }

    Pixmap maskPixmap = XCreateBitmapFromData(m_display, XDefaultRootWindow(m_display),
                                              (char*)&mask[0], size.x, size.y);
    Pixmap dataPixmap = XCreateBitmapFromData(m_display, XDefaultRootWindow(m_display),
                                              (char*)&data[0], size.x, size.y);

    // Define the foreground color as white and the background as black.
    XColor fg, bg;
    fg.red = fg.blue = fg.green = -1;
    bg.red = bg.blue = bg.green =  0;

    // Create the monochrome cursor.
    m_cursor = XCreatePixmapCursor(m_display,
                                   dataPixmap, maskPixmap,
                                   &fg, &bg,
                                   hotspot.x, hotspot.y);

    // Free the resources
    XFreePixmap(m_display, dataPixmap);
    XFreePixmap(m_display, maskPixmap);

    // We assume everything went fine...
    return true;
}


////////////////////////////////////////////////////////////
bool CursorImpl::loadFromSystem(Cursor::Type type)
{
    release();

    unsigned int shape;
    switch (type)
    {
        default: return false;

        case Cursor::Arrow:          shape = XC_arrow;              break;
        case Cursor::Wait:           shape = XC_watch;              break;
        case Cursor::Text:           shape = XC_xterm;              break;
        case Cursor::Hand:           shape = XC_hand1;              break;
        case Cursor::SizeHorizontal: shape = XC_sb_h_double_arrow;  break;
        case Cursor::SizeVertical:   shape = XC_sb_v_double_arrow;  break;
        case Cursor::SizeAll:        shape = XC_fleur;              break;
        case Cursor::Cross:          shape = XC_crosshair;          break;
        case Cursor::Help:           shape = XC_question_arrow;     break;
        case Cursor::NotAllowed:     shape = XC_X_cursor;           break;
    }

    m_cursor = XCreateFontCursor(m_display, shape);
    return true;
}


////////////////////////////////////////////////////////////
void CursorImpl::release()
{
    if (m_cursor != None)
    {
        XFreeCursor(m_display, m_cursor);
        m_cursor = None;
    }
}


} // namespace priv

} // namespace sf

