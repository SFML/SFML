////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2018 Marco Antognini (antognini.marco@gmail.com),
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
#include <SFML/Window/OSX/CursorImpl.hpp>

#import <SFML/Window/OSX/AutoreleasePoolWrapper.h>
#import <SFML/Window/OSX/NSImage+raw.h>
#import <AppKit/AppKit.h>

namespace sf
{
namespace priv
{

////////////////////////////////////////////////////////////
CursorImpl::CursorImpl() :
m_cursor(nil)
{
    // Just ask for a pool
    ensureThreadHasPool();
}


////////////////////////////////////////////////////////////
CursorImpl::~CursorImpl()
{
    [m_cursor release];
}


////////////////////////////////////////////////////////////
bool CursorImpl::loadFromPixels(const Uint8* pixels, Vector2u size, Vector2u hotspot)
{
    NSSize   nssize    = NSMakeSize(size.x, size.y);
    NSImage* image     = [NSImage imageWithRawData:pixels andSize:nssize];
    NSPoint  nshotspot = NSMakePoint(hotspot.x, hotspot.y);

    m_cursor = [[NSCursor alloc] initWithImage:image hotSpot:nshotspot];

    return m_cursor != nil;
}


////////////////////////////////////////////////////////////
bool CursorImpl::loadFromSystem(Cursor::Type type)
{
    switch (type)
    {
        default: return false;

        case Cursor::Arrow:           m_cursor = [NSCursor arrowCursor];               break;
        case Cursor::Text:            m_cursor = [NSCursor IBeamCursor];               break;
        case Cursor::Hand:            m_cursor = [NSCursor pointingHandCursor];        break;
        case Cursor::SizeHorizontal:  m_cursor = [NSCursor resizeLeftRightCursor];     break;
        case Cursor::SizeVertical:    m_cursor = [NSCursor resizeUpDownCursor];        break;
        case Cursor::Cross:           m_cursor = [NSCursor crosshairCursor];           break;
        case Cursor::NotAllowed:      m_cursor = [NSCursor operationNotAllowedCursor]; break;
    }

    // Since we didn't allocate the cursor ourself, we have to retain it
    // in order to not break the retain count.
    [m_cursor retain];

    // For all non-default cases, it was a success.
    return true;
}


} // namespace priv

} // namespace sf

