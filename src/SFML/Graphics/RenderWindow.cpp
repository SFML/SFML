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
RenderWindow::RenderWindow(VideoMode Mode, const std::string& Title, unsigned long WindowStyle, const ContextSettings& Settings)
{
    Create(Mode, Title, WindowStyle, Settings);
}


////////////////////////////////////////////////////////////
/// Construct the window from an existing control
////////////////////////////////////////////////////////////
RenderWindow::RenderWindow(WindowHandle Handle, const ContextSettings& Settings)
{
    Create(Handle, Settings);
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
bool RenderWindow::Activate(bool Active)
{
    // For performances and consistency reasons, we only handle activation
    if (Active)
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
    const unsigned int Width  = GetWidth();
    const unsigned int Height = GetHeight();

    // Set our window as the current target for rendering
    if (SetActive())
    {
        // Get pixels from the backbuffer
        std::vector<Uint8> Pixels(Width * Height * 4);
        Uint8* PixelsPtr = &Pixels[0];
        GLCheck(glReadPixels(0, 0, Width, Height, GL_RGBA, GL_UNSIGNED_BYTE, PixelsPtr));

        // Flip the pixels
        unsigned int Pitch = Width * 4;
        for (unsigned int y = 0; y < Height / 2; ++y)
            std::swap_ranges(PixelsPtr + y * Pitch, PixelsPtr + (y + 1) * Pitch, PixelsPtr + (Height - y - 1) * Pitch);

        // Create an image from the pixel buffer and return it
        return Image(Width, Height, PixelsPtr);
    }
    else
    {
        return Image(Width, Height, Color::White);
    }
}


////////////////////////////////////////////////////////////
/// Convert a point in window coordinates into view coordinates
/// This version uses the current view of the window
////////////////////////////////////////////////////////////
sf::Vector2f RenderWindow::ConvertCoords(unsigned int WindowX, unsigned int WindowY) const
{
    return ConvertCoords(WindowX, WindowY, GetView());
}


////////////////////////////////////////////////////////////
/// Convert a point in window coordinates into view coordinates
/// This version uses the given view
////////////////////////////////////////////////////////////
sf::Vector2f RenderWindow::ConvertCoords(unsigned int WindowX, unsigned int WindowY, const View& TargetView) const
{
    // First, convert from viewport coordinates to homogeneous coordinates
    const FloatRect& Viewport = TargetView.GetViewport();
    int Left   = static_cast<int>(0.5f + GetWidth()  * Viewport.Left);
    int Top    = static_cast<int>(0.5f + GetHeight() * Viewport.Top);
    int Width  = static_cast<int>(0.5f + GetWidth()  * Viewport.GetSize().x);
    int Height = static_cast<int>(0.5f + GetHeight() * Viewport.GetSize().y);

    Vector2f Coords;
    Coords.x = -1.f + 2.f * (static_cast<int>(WindowX) - Left) / Width;
    Coords.y =  1.f - 2.f * (static_cast<int>(WindowY) - Top) / Height;

    // Then transform by the inverse of the view matrix
    return TargetView.GetInverseMatrix().Transform(Coords);
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
