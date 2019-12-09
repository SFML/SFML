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

#ifndef SFML_TEXTURE_IMPL_DEFAULT_HPP
#define SFML_TEXTURE_IMPL_DEFAULT_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics/TextureImpl.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Export.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Window/GlResource.hpp>


namespace sf
{
class InputStream;
class RenderTarget;
class RenderTexture;
class Text;
class Window;

namespace priv
{
class RenderTargetImplDefault;

////////////////////////////////////////////////////////////
/// \brief Image living on the graphics card that can be used for drawing
///
////////////////////////////////////////////////////////////
class TextureImplDefault : public TextureImpl, private GlResource
{
public:

    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    /// Creates an empty texture.
    ///
    ////////////////////////////////////////////////////////////
    TextureImplDefault();

    ////////////////////////////////////////////////////////////
    /// \brief Copy constructor
    ///
    /// \param copy instance to copy
    ///
    ////////////////////////////////////////////////////////////
    TextureImplDefault(const TextureImplDefault& copy);

    ////////////////////////////////////////////////////////////
    /// \brief Destructor
    ///
    ////////////////////////////////////////////////////////////
    ~TextureImplDefault();

    ////////////////////////////////////////////////////////////
    /// \brief Bind a texture for rendering
    ///
    /// \param texture Pointer to the texture to bind, can be null to use no texture
    /// \param coordinateType Type of texture coordinates to use
    ///
    ////////////////////////////////////////////////////////////
    static void bind(const TextureImplDefault* texture, Texture::CoordinateType coordinateType = Texture::Normalized);

    ////////////////////////////////////////////////////////////
    /// \brief Get the maximum texture size allowed
    ///
    /// \return Maximum size allowed for textures, in pixels
    ///
    ////////////////////////////////////////////////////////////
    static unsigned int getMaximumSize();

private:

    friend class sf::Text;
    friend class sf::RenderTexture;
    friend class RenderTargetImplDefault;

    ////////////////////////////////////////////////////////////
    /// \brief Create the texture
    ///
    /// \param width  Width of the texture
    /// \param height Height of the texture
    ///
    /// \return True if creation was successful
    ///
    ////////////////////////////////////////////////////////////
    virtual bool create(unsigned int width, unsigned int height);

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
    virtual bool loadFromImage(const Image& image, const IntRect& area);

    ////////////////////////////////////////////////////////////
    /// \brief Return the size of the texture
    ///
    /// \return Size in pixels
    ///
    ////////////////////////////////////////////////////////////
    virtual Vector2u getSize() const;

    ////////////////////////////////////////////////////////////
    /// \brief Copy the texture pixels to an image
    ///
    /// \return Image containing the texture's pixels
    ///
    /// \see loadFromImage
    ///
    ////////////////////////////////////////////////////////////
    virtual Image copyToImage() const;

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
    virtual void update(const Uint8* pixels, unsigned int width, unsigned int height, unsigned int x, unsigned int y);

    ////////////////////////////////////////////////////////////
    /// \brief Update a part of this texture from another texture
    ///
    /// \param texture Source texture to copy to this texture
    /// \param x       X offset in this texture where to copy the source texture
    /// \param y       Y offset in this texture where to copy the source texture
    ///
    ////////////////////////////////////////////////////////////
    virtual void update(const TextureImpl& texture, unsigned int x, unsigned int y);

    ////////////////////////////////////////////////////////////
    /// \brief Update a part of the texture from the contents of a window
    ///
    /// \param window Window to copy to the texture
    /// \param x      X offset in the texture where to copy the source window
    /// \param y      Y offset in the texture where to copy the source window
    ///
    ////////////////////////////////////////////////////////////
    virtual void update(const Window& window, unsigned int x, unsigned int y);

    ////////////////////////////////////////////////////////////
    /// \brief Enable or disable the smooth filter
    ///
    /// \param smooth True to enable smoothing, false to disable it
    ///
    /// \see isSmooth
    ///
    ////////////////////////////////////////////////////////////
    virtual void setSmooth(bool smooth);

    ////////////////////////////////////////////////////////////
    /// \brief Tell whether the smooth filter is enabled or not
    ///
    /// \return True if smoothing is enabled, false if it is disabled
    ///
    /// \see setSmooth
    ///
    ////////////////////////////////////////////////////////////
    virtual bool isSmooth() const;

    ////////////////////////////////////////////////////////////
    /// \brief Enable or disable conversion from sRGB
    ///
    /// \param sRgb True to enable sRGB conversion, false to disable it
    ///
    /// \see isSrgb
    ///
    ////////////////////////////////////////////////////////////
    virtual void setSrgb(bool sRgb);

    ////////////////////////////////////////////////////////////
    /// \brief Tell whether the texture source is converted from sRGB or not
    ///
    /// \return True if the texture source is converted from sRGB, false if not
    ///
    /// \see setSrgb
    ///
    ////////////////////////////////////////////////////////////
    virtual bool isSrgb() const;

    ////////////////////////////////////////////////////////////
    /// \brief Enable or disable repeating
    ///
    /// \param repeated True to repeat the texture, false to disable repeating
    ///
    /// \see isRepeated
    ///
    ////////////////////////////////////////////////////////////
    virtual void setRepeated(bool repeated);

    ////////////////////////////////////////////////////////////
    /// \brief Tell whether the texture is repeated or not
    ///
    /// \return True if repeat mode is enabled, false if it is disabled
    ///
    /// \see setRepeated
    ///
    ////////////////////////////////////////////////////////////
    virtual bool isRepeated() const;

    ////////////////////////////////////////////////////////////
    /// \brief Generate a mipmap using the current texture data
    ///
    /// \return True if mipmap generation was successful, false if unsuccessful
    ///
    ////////////////////////////////////////////////////////////
    virtual bool generateMipmap();

    ////////////////////////////////////////////////////////////
    /// \brief Get the underlying OpenGL handle of the texture.
    ///
    /// \return OpenGL handle of the texture or 0 if not yet created
    ///
    ////////////////////////////////////////////////////////////
    virtual unsigned int getNativeHandle() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get a valid image size according to hardware support
    ///
    /// This function checks whether the graphics driver supports
    /// non power of two sizes or not, and adjusts the size
    /// accordingly.
    /// The returned size is greater than or equal to the original size.
    ///
    /// \param size size to convert
    ///
    /// \return Valid nearest size (greater than or equal to specified size)
    ///
    ////////////////////////////////////////////////////////////
    static unsigned int getValidSize(unsigned int size);

    ////////////////////////////////////////////////////////////
    /// \brief Invalidate the mipmap if one exists
    ///
    /// This also resets the texture's minifying function.
    /// This function is mainly for internal use by RenderTexture.
    ///
    ////////////////////////////////////////////////////////////
    void invalidateMipmap();

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    Vector2u     m_size;          ///< Public texture size
    Vector2u     m_actualSize;    ///< Actual texture size (can be greater than public size because of padding)
    unsigned int m_texture;       ///< Internal texture identifier
    bool         m_isSmooth;      ///< Status of the smooth filter
    bool         m_sRgb;          ///< Should the texture source be converted from sRGB?
    bool         m_isRepeated;    ///< Is the texture in repeat mode?
    mutable bool m_pixelsFlipped; ///< To work around the inconsistency in Y orientation
    bool         m_fboAttachment; ///< Is this texture owned by a framebuffer object?
    bool         m_hasMipmap;     ///< Has the mipmap been generated?
    Uint64       m_cacheId;       ///< Unique number that identifies the texture to the render target's cache
};

} // namespace priv

} // namespace sf


#endif // SFML_TEXTURE_IMPL_DEFAULT_HPP
