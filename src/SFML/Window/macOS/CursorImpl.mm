////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2025 Marco Antognini (antognini.marco@gmail.com),
//                         Laurent Gomila (laurent@sfml-dev.org)
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
#include <SFML/Window/macOS/AutoreleasePoolWrapper.hpp>
#include <SFML/Window/macOS/CursorImpl.hpp>
#import <SFML/Window/macOS/NSImage+raw.h>

#import <AppKit/AppKit.h>

namespace
{

////////////////////////////////////////////////////////////
NSCursor* loadFromSelector(SEL selector)
{
    // The caller is responsible for retaining the cursor.
    if ([NSCursor respondsToSelector:selector])
        return [NSCursor performSelector:selector];

    return nil;
}

} // namespace

namespace sf::priv
{
////////////////////////////////////////////////////////////
CursorImpl::~CursorImpl()
{
    const AutoreleasePool pool;
    [m_cursor release];
}


////////////////////////////////////////////////////////////
bool CursorImpl::loadFromPixels(const std::uint8_t* pixels, Vector2u size, Vector2u hotspot)
{
    const AutoreleasePool pool;
    if (m_cursor)
    {
        [m_cursor release];
        m_cursor = nil;
    }

    const NSSize   nssize    = NSMakeSize(size.x, size.y);
    NSImage* const image     = [NSImage imageWithRawData:pixels andSize:nssize];
    const NSPoint  nshotspot = NSMakePoint(hotspot.x, hotspot.y);

    m_cursor = [[NSCursor alloc] initWithImage:image hotSpot:nshotspot];

    return m_cursor != nil;
}

////////////////////////////////////////////////////////////
bool CursorImpl::loadFromSystem(Cursor::Type type)
{
    const AutoreleasePool pool;
    NSCursor*             newCursor = nil;

    // clang-format off
    switch (type)
    {
        default: return false;

        case Cursor::Type::Arrow:           newCursor = [NSCursor arrowCursor];               break;
        case Cursor::Type::Text:            newCursor = [NSCursor IBeamCursor];               break;
        case Cursor::Type::Hand:            newCursor = [NSCursor pointingHandCursor];        break;
        case Cursor::Type::SizeHorizontal:  newCursor = [NSCursor resizeLeftRightCursor];     break;
        case Cursor::Type::SizeVertical:    newCursor = [NSCursor resizeUpDownCursor];        break;
        case Cursor::Type::Cross:           newCursor = [NSCursor crosshairCursor];           break;
        case Cursor::Type::NotAllowed:      newCursor = [NSCursor operationNotAllowedCursor]; break;
        case Cursor::Type::SizeLeft:        newCursor = [NSCursor resizeLeftRightCursor];     break;
        case Cursor::Type::SizeRight:       newCursor = [NSCursor resizeLeftRightCursor];     break;
        case Cursor::Type::SizeTop:         newCursor = [NSCursor resizeUpDownCursor];        break;
        case Cursor::Type::SizeBottom:      newCursor = [NSCursor resizeUpDownCursor];        break;

        // These cursor types are undocumented, may not be available on some platforms
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wundeclared-selector"
        case Cursor::Type::SizeTopRight:
        case Cursor::Type::SizeBottomLeft:
        case Cursor::Type::SizeBottomLeftTopRight:
            newCursor = loadFromSelector(@selector(_windowResizeNorthEastSouthWestCursor));
            break;

        case Cursor::Type::SizeTopLeft:
        case Cursor::Type::SizeBottomRight:
        case Cursor::Type::SizeTopLeftBottomRight:
            newCursor = loadFromSelector(@selector(_windowResizeNorthWestSouthEastCursor));
            break;

        case Cursor::Type::Help:
            newCursor = loadFromSelector(@selector(_helpCursor));
            break;
#pragma GCC diagnostic pop
    }
    // clang-format on

    if (newCursor)
    {
        [m_cursor release];
        m_cursor = newCursor;
        [m_cursor retain];
    }

    return newCursor != nil;
}


} // namespace sf::priv
