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
    /// \param Width :       Width of the render image
    /// \param Height :      Height of the render image
    /// \param DepthBuffer : Do you want a depth buffer attached? (false by default)
    ///
    /// \return True if creation has been successful
    ///
    ////////////////////////////////////////////////////////////
    bool Create(unsigned int Width, unsigned int Height, bool DepthBuffer = false);

    ////////////////////////////////////////////////////////////
    /// Activate of deactivate the render-image as the current target
    /// for rendering
    ///
    /// \param Active : True to activate, false to deactivate (true by default)
    ///
    /// \return True if operation was successful, false otherwise
    ///
    ////////////////////////////////////////////////////////////
    bool SetActive(bool Active = true);

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
    static bool CanUseRenderImage();

private :

    friend class Image;

    ////////////////////////////////////////////////////////////
    /// /see RenderTarget::Activate
    ///
    ////////////////////////////////////////////////////////////
    virtual bool Activate(bool Active);

    ////////////////////////////////////////////////////////////
    /// Update the pixels of the target image.
    /// This function is called automatically by the image when it
    /// needs to update its pixels, and is only meant for internal use.
    ///
    /// \param Target : Target image to update
    ///
    /// \return True if the new pixels are flipped vertically
    ///
    ////////////////////////////////////////////////////////////
    bool UpdateImage(Image& Target);

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    Image                  myImage;       ///< Target image to draw on
    priv::RenderImageImpl* myRenderImage; ///< Platform / hardware specific implementation
};

} // namespace sf


#endif // SFML_RENDERIMAGE_HPP
