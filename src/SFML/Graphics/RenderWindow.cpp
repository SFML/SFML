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
#include <SFML/Graphics/RenderWindow.hpp>


namespace sf
{
////////////////////////////////////////////////////////////
RenderWindow::RenderWindow()
{
    // Nothing to do
}


////////////////////////////////////////////////////////////
RenderWindow::RenderWindow(VideoMode mode, const std::string& title, unsigned long style, const ContextSettings& settings)
{
    // Don't call the base class constructor because it contains virtual function calls
    Create(mode, title, style, settings);
}


////////////////////////////////////////////////////////////
RenderWindow::RenderWindow(WindowHandle handle, const ContextSettings& settings)
{
    // Don't call the base class constructor because it contains virtual function calls
    Create(handle, settings);
}


////////////////////////////////////////////////////////////
RenderWindow::~RenderWindow()
{
    // Nothing to do
}


////////////////////////////////////////////////////////////
bool RenderWindow::Activate(bool active)
{
    return SetActive(active);
}


////////////////////////////////////////////////////////////
unsigned int RenderWindow::GetWidth() const
{
    return Window::GetWidth();
}


////////////////////////////////////////////////////////////
unsigned int RenderWindow::GetHeight() const
{
    return Window::GetHeight();
}


////////////////////////////////////////////////////////////
void RenderWindow::OnCreate()
{
    // Just initialize the render target part
    RenderTarget::Initialize();
}


////////////////////////////////////////////////////////////
void RenderWindow::OnResize()
{
    // Update the current view (recompute the viewport, which is stored in relative coordinates)
    SetView(GetView());
}

} // namespace sf
