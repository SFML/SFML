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
#include <SFML/System/NonCopyable.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/RenderTarget.hpp>


namespace sf
{
namespace priv
{
    class RenderImageImpl;
}

////////////////////////////////////////////////////////////
/// Target for 2D rendering into an image that can be reused
/// in a sprite
////////////////////////////////////////////////////////////
class SFML_API RenderImage : public RenderTarget, NonCopyable
{
public :

    ////////////////////////////////////////////////////////////
    /// Default constructor
    ///
    ////////////////////////////////////////////////////////////
    RenderImage();

    ////////////////////////////////////////////////////////////
    /// Destructor
    ///
    ////////////////////////////////////////////////////////////
    virtual ~RenderImage();

    ////////////////////////////////////////////////////////////
    /// Create the render image from its dimensions
    ///
    /// \param width :       Width of the render image
    /// \param height :      Height of the render image
    /// \param depthBuffer : Do you want this render image to have a depth buffer?
    ///
    /// \return True if creation has been successful
    ///
    ////////////////////////////////////////////////////////////
    bool Create(unsigned int width, unsigned int height, bool depthBuffer = false);

    ////////////////////////////////////////////////////////////
    /// Enable or disable image smooth filter.
    /// This parameter is enabled by default
    ///
    /// \param smooth : True to enable smoothing filter, false to disable it
    ///
    ////////////////////////////////////////////////////////////
    void SetSmooth(bool smooth);

    ////////////////////////////////////////////////////////////
    /// Tells whether the smooth filtering is enabled or not
    ///
    /// \return True if image smoothing is enabled
    ///
    ////////////////////////////////////////////////////////////
    bool IsSmooth() const;

    ////////////////////////////////////////////////////////////
    /// Activate of deactivate the render-image as the current target
    /// for rendering
    ///
    /// \param active : True to activate, false to deactivate
    ///
    /// \return True if operation was successful, false otherwise
    ///
    ////////////////////////////////////////////////////////////
    bool SetActive(bool active = true);

    ////////////////////////////////////////////////////////////
    /// Update the contents of the target image
    ///
    ////////////////////////////////////////////////////////////
    void Display();

    ////////////////////////////////////////////////////////////
    /// Get the width of the rendering region of the image
    ///
    /// \return Width in pixels
    ///
    ////////////////////////////////////////////////////////////
    virtual unsigned int GetWidth() const;

    ////////////////////////////////////////////////////////////
    /// Get the height of the rendering region of the image
    ///
    /// \return Height in pixels
    ///
    ////////////////////////////////////////////////////////////
    virtual unsigned int GetHeight() const;

    ////////////////////////////////////////////////////////////
    /// Get the target image
    ///
    /// \return Target image
    ///
    ////////////////////////////////////////////////////////////
    const Image& GetImage() const;

    ////////////////////////////////////////////////////////////
    /// Check whether the system supports render images or not
    ///
    /// \return True if the RenderImage class can be used
    ///
    ////////////////////////////////////////////////////////////
    static bool IsAvailable();

private :

    ////////////////////////////////////////////////////////////
    /// /see RenderTarget::Activate
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
