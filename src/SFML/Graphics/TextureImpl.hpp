////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2019 Laurent Gomila (laurent@sfml-dev.org)
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

#ifndef SFML_TEXTURE_IMPL_HPP
#define SFML_TEXTURE_IMPL_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics/Export.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>


namespace sf
{
class Image;
class Window;

namespace priv
{
////////////////////////////////////////////////////////////
/// \brief Image living on the graphics card that can be used for drawing
///
////////////////////////////////////////////////////////////
class TextureImpl
{
public:

    ////////////////////////////////////////////////////////////
    /// \brief Destructor
    ///
    ////////////////////////////////////////////////////////////
    virtual ~TextureImpl();

    ////////////////////////////////////////////////////////////
    /// \brief Create the texture implementation
    ///
    /// \param width  Width of the texture
    /// \param height Height of the texture
    ///
    /// \return True if creation was successful
    ///
    ////////////////////////////////////////////////////////////
    virtual bool create(unsigned int width, unsigned int height) = 0;

    ////////////////////////////////////////////////////////////
    /// \brief Load the texture from an image
    ///
    /// \param image Image to load into the texture
    /// \param area  Area of the image to load
    ///
    /// \return True if loading was successful
    ///
    /// \see loadFromFile, loadFromMemory
    ///
    ////////////////////////////////////////////////////////////
    virtual bool loadFromImage(const Image& image, const IntRect& area) = 0;

    ////////////////////////////////////////////////////////////
    /// \brief Return the size of the texture
    ///
    /// \return Size in pixels
    ///
    ////////////////////////////////////////////////////////////
    virtual Vector2u getSize() const = 0;

    ////////////////////////////////////////////////////////////
    /// \brief Copy the texture pixels to an image
    ///
    /// \return Image containing the texture's pixels
    ///
    /// \see loadFromImage
    ///
    ////////////////////////////////////////////////////////////
    virtual Image copyToImage() const = 0;

    ////////////////////////////////////////////////////////////
    /// \brief Update a part of the texture from an array of pixels
    ///
    /// \param pixels Array of pixels to copy to the texture
    /// \param width  Width of the pixel region contained in \a pixels
    /// \param height Height of the pixel region contained in \a pixels
    /// \param x      X offset in the texture where to copy the source pixels
    /// \param y      Y offset in the texture where to copy the source pixels
    ///
    ////////////////////////////////////////////////////////////
    virtual void update(const Uint8* pixels, unsigned int width, unsigned int height, unsigned int x, unsigned int y) = 0;

    ////////////////////////////////////////////////////////////
    /// \brief Update a part of this texture from another texture
    ///
    /// \param texture Source texture to copy to this texture
    /// \param x       X offset in this texture where to copy the source texture
    /// \param y       Y offset in this texture where to copy the source texture
    ///
    ////////////////////////////////////////////////////////////
    virtual void update(const TextureImpl& texture, unsigned int x, unsigned int y) = 0;

    ////////////////////////////////////////////////////////////
    /// \brief Update a part of the texture from the contents of a window
    ///
    /// \param window Window to copy to the texture
    /// \param x      X offset in the texture where to copy the source window
    /// \param y      Y offset in the texture where to copy the source window
    ///
    ////////////////////////////////////////////////////////////
    virtual void update(const Window& window, unsigned int x, unsigned int y) = 0;

    ////////////////////////////////////////////////////////////
    /// \brief Enable or disable the smooth filter
    ///
    /// \param smooth True to enable smoothing, false to disable it
    ///
    /// \see isSmooth
    ///
    ////////////////////////////////////////////////////////////
    virtual void setSmooth(bool smooth) = 0;

    ////////////////////////////////////////////////////////////
    /// \brief Tell whether the smooth filter is enabled or not
    ///
    /// \return True if smoothing is enabled, false if it is disabled
    ///
    /// \see setSmooth
    ///
    ////////////////////////////////////////////////////////////
    virtual bool isSmooth() const = 0;

    ////////////////////////////////////////////////////////////
    /// \brief Enable or disable conversion from sRGB
    ///
    /// \param sRgb True to enable sRGB conversion, false to disable it
    ///
    /// \see isSrgb
    ///
    ////////////////////////////////////////////////////////////
    virtual void setSrgb(bool sRgb) = 0;

    ////////////////////////////////////////////////////////////
    /// \brief Tell whether the texture source is converted from sRGB or not
    ///
    /// \return True if the texture source is converted from sRGB, false if not
    ///
    /// \see setSrgb
    ///
    ////////////////////////////////////////////////////////////
    virtual bool isSrgb() const = 0;

    ////////////////////////////////////////////////////////////
    /// \brief Enable or disable repeating
    ///
    /// \param repeated True to repeat the texture, false to disable repeating
    ///
    /// \see isRepeated
    ///
    ////////////////////////////////////////////////////////////
    virtual void setRepeated(bool repeated) = 0;

    ////////////////////////////////////////////////////////////
    /// \brief Tell whether the texture is repeated or not
    ///
    /// \return True if repeat mode is enabled, false if it is disabled
    ///
    /// \see setRepeated
    ///
    ////////////////////////////////////////////////////////////
    virtual bool isRepeated() const = 0;

    ////////////////////////////////////////////////////////////
    /// \brief Generate a mipmap using the current texture data
    ///
    /// \return True if mipmap generation was successful, false if unsuccessful
    ///
    ////////////////////////////////////////////////////////////
    virtual bool generateMipmap() = 0;

    ////////////////////////////////////////////////////////////
    /// \brief Get the underlying OpenGL handle of the texture.
    ///
    /// \return OpenGL handle of the texture or 0 if not yet created
    ///
    ////////////////////////////////////////////////////////////
    virtual unsigned int getNativeHandle() const = 0;
};

} // namespace priv

} // namespace sf


#endif // SFML_TEXTURE_IMPL_HPP
