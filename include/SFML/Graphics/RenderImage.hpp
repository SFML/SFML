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

#ifndef SFML_RENDERIMAGE_HPP
#define SFML_RENDERIMAGE_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/RenderTarget.hpp>


namespace sf
{
namespace priv
{
    class RenderImageImpl;
}

////////////////////////////////////////////////////////////
/// \brief Target for off-screen 2D rendering into an image
///
////////////////////////////////////////////////////////////
class SFML_API RenderImage : public RenderTarget
{
public :

    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    /// Constructs an empty, invalid render-image. You must
    /// call Create to have a valid render-image.
    ///
    /// \see Create
    ///
    ////////////////////////////////////////////////////////////
    RenderImage();

    ////////////////////////////////////////////////////////////
    /// \brief Destructor
    ///
    ////////////////////////////////////////////////////////////
    virtual ~RenderImage();

    ////////////////////////////////////////////////////////////
    /// \brief Create the render-image
    ///
    /// Before calling this function, the render-image is in
    /// an invalid state, thus it is mandatory to call it before
    /// doing anything with the render-image.
    /// The last parameter, \a depthBuffer, is useful if you want
    /// to use the render-image for 3D OpenGL rendering that requires
    /// a depth-buffer. Otherwise it is unnecessary, and you should
    /// leave this parameter to false (which is its default value).
    ///
    /// \param width        Width of the render-image
    /// \param height       Height of the render-image
    /// \param depthBuffer  Do you want this render-image to have a depth buffer?
    ///
    /// \return True if creation has been successful
    ///
    ////////////////////////////////////////////////////////////
    bool Create(unsigned int width, unsigned int height, bool depthBuffer = false);

    ////////////////////////////////////////////////////////////
    /// \brief Enable or disable image smoothing
    ///
    /// This function is similar to Image::SetSmooth.
    /// This parameter is enabled by default.
    ///
    /// \param smooth True to enable smoothing, false to disable it
    ///
    /// \see IsSmooth
    ///
    ////////////////////////////////////////////////////////////
    void SetSmooth(bool smooth);

    ////////////////////////////////////////////////////////////
    /// \brief Tell whether the smooth filtering is enabled or not
    ///
    /// \return True if image smoothing is enabled
    ///
    /// \see SetSmooth
    ///
    ////////////////////////////////////////////////////////////
    bool IsSmooth() const;

    ////////////////////////////////////////////////////////////
    /// \brief Activate of deactivate the render-image for rendering
    ///
    /// This function makes the render-image's context current for
    /// future OpenGL rendering operations (so you shouldn't care
    /// about it if you're not doing direct OpenGL stuff).
    /// Only one context can be current on a thread, so if you
    /// want to draw OpenGL geometry to another render target
    /// (like a RenderWindow) don't forget to activate it again.
    ///
    /// \param active True to activate, false to deactivate
    ///
    /// \return True if operation was successful, false otherwise
    ///
    ////////////////////////////////////////////////////////////
    bool SetActive(bool active = true);

    ////////////////////////////////////////////////////////////
    /// \brief Update the contents of the target image
    ///
    /// This function updates the target image with what
    /// has been drawn so far. Like for windows, calling this
    /// function is mandatory at the end of rendering. Not calling
    /// it may leave the image in an undefined state.
    ///
    ////////////////////////////////////////////////////////////
    void Display();

    ////////////////////////////////////////////////////////////
    /// \brief Return the width of the rendering region of the image
    ///
    /// The returned value is the size that you passed to
    /// the Create function.
    ///
    /// \return Width in pixels
    ///
    /// \return GetHeight
    ///
    ////////////////////////////////////////////////////////////
    virtual unsigned int GetWidth() const;

    ////////////////////////////////////////////////////////////
    /// \brief Return the height of the rendering region of the image
    ///
    /// The returned value is the size that you passed to
    /// the Create function.
    ///
    /// \return Height in pixels
    ///
    /// \return GetWidth
    ///
    ////////////////////////////////////////////////////////////
    virtual unsigned int GetHeight() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get a read-only reference to the target image
    ///
    /// After drawing to the render-image and calling Display,
    /// you can retrieve the updated image using this function,
    /// and draw it using a sprite (for example).
    /// The internal sf::Image of a render-image is always the
    /// same instance, so that it is possible to call this function
    /// once and keep a reference to the image even after is it
    /// modified.
    ///
    /// \return Const reference to the image
    ///
    ////////////////////////////////////////////////////////////
    const Image& GetImage() const;

    ////////////////////////////////////////////////////////////
    /// \brief Check whether the system supports render images or not
    ///
    /// It is very important to always call this function before
    /// trying to use the RenderImage class, as the feature may not
    /// be supported on all platforms (especially very old ones).
    /// If this function returns false, then you won't be able
    /// to use the class at all.
    ///
    /// \return True if the RenderImage class can be used
    ///
    ////////////////////////////////////////////////////////////
    static bool IsAvailable();

private :

    ////////////////////////////////////////////////////////////
    /// \brief Activate the targt efor rendering
    ///
    /// This function is called by the base class
    /// everytime it's going to use OpenGL calls.
    ///
    /// \param active True to make the target active, false to deactivate it
    ///
    /// \return True if the function succeeded
    ///
    ////////////////////////////////////////////////////////////
    virtual bool Activate(bool active);

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    Image                  myImage;       ///< Target image to draw on
    priv::RenderImageImpl* myRenderImage; ///< Platform / hardware specific implementation
};

} // namespace sf


#endif // SFML_RENDERIMAGE_HPP


////////////////////////////////////////////////////////////
/// \class sf::RenderImage
/// \ingroup graphics
///
/// sf::RenderImage is the little brother of sf::RenderWindow.
/// It implements the same 2D drawing and OpenGL-related functions
/// (see their base class sf::RenderTarget for more details),
/// the difference is that the result is stored in an off-screen
/// image rather than being show in a window.
///
/// Rendering to an image can be useful in a variety of situations:
/// \li precomputing a complex static image (like a level's background from multiple tiles)
/// \li applying post-effects to the whole scene with shaders
/// \li creating a sprite from a 3D object rendered with OpenGL
/// \li etc.
///
/// Usage example:
///
/// \code
/// // First of all: make sure that rendering to image is supported
/// if (!sf::RenderImage::IsAvailable())
///    return -1;
///
/// // Create a new render-window
/// sf::RenderWindow window(sf::VideoMode(800, 600), "SFML window");
///
/// // Create a new render-image
/// sf::RenderImage image;
/// if (!image.Create(500, 500))
///     return -1
///
/// // The main loop
/// while (window.IsOpened())
/// {
///    // Event processing
///    // ...
///
///    // Clear the whole image with red color
///    image.Clear(sf::Color::Red);
///
///    // Draw stuff to the image
///    image.Draw(sprite);  // sprite is a sf::Sprite
///    image.Draw(shape);   // shape is a sf::Shape
///    image.Draw(text);    // text is a sf::Text
///
///    // We're done drawing to the image
///    image.Display();
///
///    // Now we start rendering to the window, clear it first
///    window.Clear();
///
///    // Draw the image
///    sf::Sprite sprite(image.GetImage());
///    window.Draw(sprite);
///
///    // End the current frame and display its contents on screen
///    window.Display();
/// }
/// \endcode
///
/// Like sf::RenderWindow, sf::RenderImage is still able to render direct
/// OpenGL stuff. It is even possible to mix together OpenGL calls
/// and regular SFML drawing commands. If you need a depth buffer for
/// 3D rendering, don't forget to request it when calling RenderImage::Create.
///
/// \see sf::RenderTarget, sf::RenderWindow, sf::View
///
////////////////////////////////////////////////////////////
