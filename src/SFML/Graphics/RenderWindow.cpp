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
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/GLCheck.hpp>
#include <iostream>


namespace sf
{
////////////////////////////////////////////////////////////
/// Default constructor
////////////////////////////////////////////////////////////
RenderWindow::RenderWindow()
{
    // Nothing to do
}


////////////////////////////////////////////////////////////
/// Construct the window
////////////////////////////////////////////////////////////
RenderWindow::RenderWindow(VideoMode mode, const std::string& title, unsigned long style, const ContextSettings& settings)
{
    Create(mode, title, style, settings);
}


////////////////////////////////////////////////////////////
/// Construct the window from an existing control
////////////////////////////////////////////////////////////
RenderWindow::RenderWindow(WindowHandle handle, const ContextSettings& settings)
{
    Create(handle, settings);
}


////////////////////////////////////////////////////////////
/// Destructor
////////////////////////////////////////////////////////////
RenderWindow::~RenderWindow()
{
    // Nothing to do
}


////////////////////////////////////////////////////////////
/// /see RenderTarget::Activate
////////////////////////////////////////////////////////////
bool RenderWindow::Activate(bool active)
{
    // For performances and consistency reasons, we only handle activation
    if (active)
        return SetActive();
    else
        return true;
}


////////////////////////////////////////////////////////////
/// Get the width of the rendering region of the window
////////////////////////////////////////////////////////////
unsigned int RenderWindow::GetWidth() const
{
    return sf::Window::GetWidth();
}


////////////////////////////////////////////////////////////
/// Get the height of the rendering region of the window
////////////////////////////////////////////////////////////
unsigned int RenderWindow::GetHeight() const
{
    return sf::Window::GetHeight();
}


////////////////////////////////////////////////////////////
/// Save the content of the window to an image
////////////////////////////////////////////////////////////
Image RenderWindow::Capture() const
{
    // Get the window dimensions
    const unsigned int width  = GetWidth();
    const unsigned int height = GetHeight();

    // Set our window as the current target for rendering
    if (SetActive())
    {
        // Get pixels from the backbuffer
        std::vector<Uint8> pixels(width * height * 4);
        Uint8* ptr = &pixels[0];
        GLCheck(glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, ptr));

        // Flip the pixels
        unsigned int pitch = width * 4;
        for (unsigned int y = 0; y < height / 2; ++y)
            std::swap_ranges(ptr + y * pitch, ptr + (y + 1) * pitch, ptr + (height - y - 1) * pitch);

        // Create an image from the pixel buffer and return it
        return Image(width, height, ptr);
    }
    else
    {
        return Image(width, height, Color::White);
    }
}


////////////////////////////////////////////////////////////
/// Convert a point in window coordinates into view coordinates
/// This version uses the current view of the window
////////////////////////////////////////////////////////////
sf::Vector2f RenderWindow::ConvertCoords(unsigned int x, unsigned int y) const
{
    return ConvertCoords(x, y, GetView());
}


////////////////////////////////////////////////////////////
/// Convert a point in window coordinates into view coordinates
/// This version uses the given view
////////////////////////////////////////////////////////////
sf::Vector2f RenderWindow::ConvertCoords(unsigned int x, unsigned int y, const View& view) const
{
    // First, convert from viewport coordinates to homogeneous coordinates
    const FloatRect& viewport = view.GetViewport();
    int left   = static_cast<int>(0.5f + GetWidth()  * viewport.Left);
    int top    = static_cast<int>(0.5f + GetHeight() * viewport.Top);
    int width  = static_cast<int>(0.5f + GetWidth()  * viewport.GetSize().x);
    int height = static_cast<int>(0.5f + GetHeight() * viewport.GetSize().y);

    Vector2f coords;
    coords.x = -1.f + 2.f * (static_cast<int>(x) - left) / width;
    coords.y =  1.f - 2.f * (static_cast<int>(y) - top) / height;

    // Then transform by the inverse of the view matrix
    return view.GetInverseMatrix().Transform(coords);
}


////////////////////////////////////////////////////////////
/// Called after the window has been created
////////////////////////////////////////////////////////////
void RenderWindow::OnCreate()
{
    // Just initialize the render target part
    RenderTarget::Initialize();
}

} // namespace sf
