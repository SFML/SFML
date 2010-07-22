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
////////////////////////////////////////////////////////////
/// \brief Window that can serve as a target for 2D drawing
///
////////////////////////////////////////////////////////////
class SFML_API RenderWindow : public Window, public RenderTarget
{
public :

    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    /// This constructor doesn't actually create the window,
    /// use the other constructors or call Create to do so.
    ///
    ////////////////////////////////////////////////////////////
    RenderWindow();

    ////////////////////////////////////////////////////////////
    /// \brief Construct a new window
    ///
    /// This constructor creates the window with the size and pixel
    /// depth defined in \a mode. An optional style can be passed to
    /// customize the look and behaviour of the window (borders,
    /// title bar, resizable, closable, ...).
    ///
    /// The fourth parameter is an optional structure specifying
    /// advanced OpenGL context settings such as antialiasing,
    /// depth-buffer bits, etc. You shouldn't care about these
    /// parameters for a regular usage of the graphics module.
    ///
    /// \param mode     Video mode to use (defines the width, height and depth of the rendering area of the window)
    /// \param title    Title of the window
    /// \param style    Window style
    /// \param settings Additional settings for the underlying OpenGL context
    ///
    ////////////////////////////////////////////////////////////
    RenderWindow(VideoMode mode, const std::string& title, unsigned long style = Style::Default, const ContextSettings& settings = ContextSettings());

    ////////////////////////////////////////////////////////////
    /// \brief Construct the window from an existing control
    ///
    /// Use this constructor if you want to create an SFML
    /// rendering area into an already existing control.
    ///
    /// The fourth parameter is an optional structure specifying
    /// advanced OpenGL context settings such as antialiasing,
    /// depth-buffer bits, etc. You shouldn't care about these
    /// parameters for a regular usage of the graphics module.
    ///
    /// \param handle   Platform-specific handle of the control
    /// \param settings Additional settings for the underlying OpenGL context
    ///
    ////////////////////////////////////////////////////////////
    explicit RenderWindow(WindowHandle handle, const ContextSettings& settings = ContextSettings());

    ////////////////////////////////////////////////////////////
    /// \brief Destructor
    ///
    /// Closes the window and free all the resources attached to it.
    ///
    ////////////////////////////////////////////////////////////
    virtual ~RenderWindow();

    ////////////////////////////////////////////////////////////
    /// \brief Get the width of the rendering region of the window
    ///
    /// The width doesn't include the titlebar and borders
    /// of the window.
    ///
    /// \return Width in pixels
    ///
    /// \see GetHeight
    ///
    ////////////////////////////////////////////////////////////
    virtual unsigned int GetWidth() const;

    ////////////////////////////////////////////////////////////
    /// Get the height of the rendering region of the window
    ///
    /// The height doesn't include the titlebar and borders
    /// of the window.
    ///
    /// \return Height in pixels
    ///
    /// \see GetWidth
    ///
    ////////////////////////////////////////////////////////////
    virtual unsigned int GetHeight() const;

private :

    ////////////////////////////////////////////////////////////
    /// \brief Function called after the window has been created
    ///
    /// This function is called so that derived classes can
    /// perform their own specific initialization as soon as
    /// the window is created.
    ///
    ////////////////////////////////////////////////////////////
    virtual void OnCreate();

    ////////////////////////////////////////////////////////////
    /// \brief Function called after the window has been resized
    ///
    /// This function is called so that derived classes can
    /// perform custom actions when the size of the window changes.
    ///
    ////////////////////////////////////////////////////////////
    virtual void OnResize();

    ////////////////////////////////////////////////////////////
    /// \brief Activate the target for rendering
    ///
    /// \param active True to make the target active, false to deactivate it
    ///
    /// \return True if the function succeeded
    ///
    ////////////////////////////////////////////////////////////
    virtual bool Activate(bool active);
};

} // namespace sf


#endif // SFML_RENDERWINDOW_HPP


////////////////////////////////////////////////////////////
/// \class sf::RenderWindow
/// \ingroup graphics
///
/// sf::RenderWindow is the main class of the Graphics module.
/// It defines an OS window that can be painted using the other
/// classes of the graphics module.
///
/// sf::RenderWindow is derived from sf::Window, thus it inherits
/// all its features: mouse/keyboard/joystick input, events, window
/// handling, OpenGL rendering, etc. See the documentation of
/// sf::Window for a more complete description of all these features
/// and code samples.
///
/// On top of that, sf::RenderWindow adds more features related to
/// 2D drawing with the graphics module (see its base class
/// sf::RenderTarget for more details).
/// Here is a typical rendering / event loop with a sf::RenderWindow:
///
/// \code
/// // Declare and create a new render-window
/// sf::RenderWindow window(sf::VideoMode(800, 600), "SFML window");
///
/// // Limit the framerate to 60 frames per second (this step is optional)
/// window.SetFramerateLimit(60);
///
/// // The main loop - ends as soon as the window is closed
/// while (window.IsOpened())
/// {
///    // Event processing
///    sf::Event event;
///    while (window.GetEvent(event))
///    {
///        // Request for closing the window
///        if (event.Type == sf::Event::Closed)
///            window.Close();
///    }
///
///    // Clear the whole window before rendering a new frame
///    window.Clear();
///
///    // Draw some sprites / shapes / texts
///    window.Draw(sprite);  // sprite is a sf::Sprite
///    window.Draw(shape);   // shape is a sf::Shape
///    window.Draw(text);    // text is a sf::Text
///
///    // End the current frame and display its contents on screen
///    window.Display();
/// }
/// \endcode
///
/// Like sf::Window, sf::RenderWindow is still able to render direct
/// OpenGL stuff. It is even possible to mix together OpenGL calls
/// and regular SFML drawing commands. When doing so, make sure that
/// OpenGL states are not messed up by calling the SaveGLStates /
/// RestoreGLStates functions.
///
/// \code
/// // Create the render window
/// sf::RenderWindow window(sf::VideoMode(800, 600), "SFML OpenGL");
///
/// // Create a sprite and a text to display
/// sf::Sprite sprite;
/// sf::Text text;
/// ...
///
/// // Perform OpenGL initializations
/// glMatrixMode(GL_PROJECTION);
/// ...
///
/// // Start the rendering loop
/// while (window.IsOpened())
/// {
///     // Process events
///     ...
///
///     // Draw a background sprite
///     window.SaveGLStates();
///     window.Draw(sprite);
///     window.RestoreGLStates();
///
///     // Draw a 3D object using OpenGL
///     glBegin(GL_QUADS);
///         glVertex3f(...);
///         ...
///     glEnd();
///
///     // Draw text on top of the 3D object
///     window.SaveGLStates();
///     window.Draw(text);
///     window.RestoreGLStates();
///
///     // Finally, display the rendered frame on screen
///     window.Display();
/// }
/// \endcode
///
/// \see sf::Window, sf::RenderTarget, sf::RenderImage, sf::View
///
////////////////////////////////////////////////////////////
