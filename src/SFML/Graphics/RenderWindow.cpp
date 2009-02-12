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
#include <SFML/Graphics/GraphicsContext.hpp>
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
RenderWindow::RenderWindow(VideoMode Mode, const std::string& Title, unsigned long WindowStyle, const WindowSettings& Params)
{
    Create(Mode, Title, WindowStyle, Params);
}


////////////////////////////////////////////////////////////
/// Construct the window from an existing control
////////////////////////////////////////////////////////////
RenderWindow::RenderWindow(WindowHandle Handle, const WindowSettings& Params)
{
    Create(Handle, Params);
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
        // Make sure we have a valid context
        priv::GraphicsContext Ctx;

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
////////////////////////////////////////////////////////////
sf::Vector2f RenderWindow::ConvertCoords(unsigned int WindowX, unsigned int WindowY, const View* TargetView) const
{
    // Use the current view if none has been passed
    if (!TargetView)
        TargetView = &GetView();

    float Left   = TargetView->GetCenter().x - TargetView->GetHalfSize().x;
    float Top    = TargetView->GetCenter().y - TargetView->GetHalfSize().y;
    float Right  = TargetView->GetCenter().x + TargetView->GetHalfSize().x;
    float Bottom = TargetView->GetCenter().y + TargetView->GetHalfSize().y;

    return sf::Vector2f(Left + WindowX * (Right - Left) / GetWidth(),
                        Top  + WindowY * (Bottom - Top) / GetHeight());
}


////////////////////////////////////////////////////////////
/// Called after the window has been created
////////////////////////////////////////////////////////////
void RenderWindow::OnCreate()
{
    // We can now initialize the render target part
    RenderTarget::Initialize();
}

} // namespace sf
