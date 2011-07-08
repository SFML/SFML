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
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/InputImpl.hpp>
#include <SFML/Window/Window.hpp>


namespace sf
{
////////////////////////////////////////////////////////////
bool Mouse::IsButtonPressed(Button button)
{
    return priv::InputImpl::IsMouseButtonPressed(button);
}


////////////////////////////////////////////////////////////
Vector2i Mouse::GetPosition()
{
    return priv::InputImpl::GetMousePosition();
}


////////////////////////////////////////////////////////////
Vector2i Mouse::GetPosition(const Window& relativeTo)
{
    return priv::InputImpl::GetMousePosition(relativeTo);
}


////////////////////////////////////////////////////////////
void Mouse::SetPosition(const Vector2i& position)
{
    priv::InputImpl::SetMousePosition(position);
}


////////////////////////////////////////////////////////////
void Mouse::SetPosition(const Vector2i& position, const Window& relativeTo)
{
    priv::InputImpl::SetMousePosition(position, relativeTo);
}

} // namespace sf
