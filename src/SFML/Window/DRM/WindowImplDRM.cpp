////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2023 Andrew Mickelson
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
#include <SFML/Window/DRM/DRMContext.hpp>
#include <SFML/Window/DRM/InputImplUDev.hpp>
#include <SFML/Window/DRM/WindowImplDRM.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/WindowStyle.hpp>

#include <SFML/System/Err.hpp>


namespace sf::priv
{
////////////////////////////////////////////////////////////
WindowImplDRM::WindowImplDRM(WindowHandle /*handle*/)
{
    sf::priv::InputImpl::setTerminalConfig();
}


////////////////////////////////////////////////////////////
WindowImplDRM::WindowImplDRM(VideoMode mode, const String& /*title*/, unsigned long /*style*/, const ContextSettings& /*settings*/) :
m_size(mode.size)
{
    sf::priv::InputImpl::setTerminalConfig();
}


////////////////////////////////////////////////////////////
WindowImplDRM::~WindowImplDRM()
{
    sf::priv::InputImpl::restoreTerminalConfig();
}


////////////////////////////////////////////////////////////
WindowHandle WindowImplDRM::getSystemHandle() const
{
    const Drm& drm = sf::priv::DRMContext::getDRM();
    return static_cast<WindowHandle>(drm.fileDescriptor);
}

////////////////////////////////////////////////////////////
Vector2i WindowImplDRM::getPosition() const
{
    return Vector2i(0, 0);
}


////////////////////////////////////////////////////////////
void WindowImplDRM::setPosition(const Vector2i& /*position*/)
{
}


////////////////////////////////////////////////////////////
Vector2u WindowImplDRM::getSize() const
{
    return m_size;
}


////////////////////////////////////////////////////////////
void WindowImplDRM::setSize(const Vector2u& /*size*/)
{
}


////////////////////////////////////////////////////////////
void WindowImplDRM::setMinimumSize(const std::optional<Vector2u>& /* minimumSize */)
{
}


////////////////////////////////////////////////////////////
void WindowImplDRM::setMaximumSize(const std::optional<Vector2u>& /* maximumSize */)
{
}


////////////////////////////////////////////////////////////
void WindowImplDRM::setTitle(const String& /*title*/)
{
}


////////////////////////////////////////////////////////////
void WindowImplDRM::setIcon(const Vector2u& /*size*/, const std::uint8_t* /*pixels*/)
{
}


////////////////////////////////////////////////////////////
void WindowImplDRM::setVisible(bool /*visible*/)
{
}

////////////////////////////////////////////////////////////
void WindowImplDRM::setMouseCursorVisible(bool /*visible*/)
{
    // TODO: not implemented
}

////////////////////////////////////////////////////////////
void WindowImplDRM::setMouseCursorGrabbed(bool /*grabbed*/)
{
    //TODO: not implemented
}

////////////////////////////////////////////////////////////
void WindowImplDRM::setMouseCursor(const CursorImpl& /*cursor*/)
{
    // TODO: not implemented
}

////////////////////////////////////////////////////////////
void WindowImplDRM::setKeyRepeatEnabled(bool /*enabled*/)
{
    // TODO: not implemented
}


////////////////////////////////////////////////////////////
void WindowImplDRM::requestFocus()
{
    // Not applicable
}


////////////////////////////////////////////////////////////
bool WindowImplDRM::hasFocus() const
{
    return true;
}

void WindowImplDRM::processEvents()
{
    sf::Event ev;
    while (sf::priv::InputImpl::checkEvent(ev))
        pushEvent(ev);
}

} // namespace sf::priv
