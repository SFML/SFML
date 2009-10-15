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

#ifndef SFML_RENDERWINDOW_HPP
#define SFML_RENDERWINDOW_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Window/Window.hpp>
#include <string>


namespace sf
{
class Drawable;

////////////////////////////////////////////////////////////
/// Simple wrapper for sf::Window that allows easy
/// 2D rendering
////////////////////////////////////////////////////////////
class SFML_API RenderWindow : public Window, public RenderTarget
{
public :

    ////////////////////////////////////////////////////////////
    /// Default constructor
    ///
    ////////////////////////////////////////////////////////////
    RenderWindow();

    ////////////////////////////////////////////////////////////
    /// Construct the window
    ///
    /// \param mode :     Video mode to use
    /// \param title :    Title of the window
    /// \param style :    Window style
    /// \param settings : Additional settings for the underlying OpenGL context
    ///
    ////////////////////////////////////////////////////////////
    RenderWindow(VideoMode mode, const std::string& title, unsigned long style = Style::Resize | Style::Close, const ContextSettings& settings = ContextSettings());

    ////////////////////////////////////////////////////////////
    /// Construct the window from an existing control
    ///
    /// \param handle :   Platform-specific handle of the control
    /// \param settings : Additional settings for the underlying OpenGL context (see default constructor for default values)
    ///
    ////////////////////////////////////////////////////////////
    RenderWindow(WindowHandle handle, const ContextSettings& settings = ContextSettings());

    ////////////////////////////////////////////////////////////
    /// Destructor
    ///
    ////////////////////////////////////////////////////////////
    virtual ~RenderWindow();

    ////////////////////////////////////////////////////////////
    /// Get the width of the rendering region of the window
    ///
    /// \return Width in pixels
    ///
    ////////////////////////////////////////////////////////////
    virtual unsigned int GetWidth() const;

    ////////////////////////////////////////////////////////////
    /// Get the height of the rendering region of the window
    ///
    /// \return Height in pixels
    ///
    ////////////////////////////////////////////////////////////
    virtual unsigned int GetHeight() const;

private :

    ////////////////////////////////////////////////////////////
    /// /see Window::OnCreate
    ///
    ////////////////////////////////////////////////////////////
    virtual void OnCreate();

    ////////////////////////////////////////////////////////////
    /// /see Window::OnDisplay
    ///
    ////////////////////////////////////////////////////////////
    virtual void OnDisplay();

    ////////////////////////////////////////////////////////////
    /// /see RenderTarget::Activate
    ///
    ////////////////////////////////////////////////////////////
    virtual bool Activate(bool active);
};

} // namespace sf


#endif // SFML_RENDERWINDOW_HPP
