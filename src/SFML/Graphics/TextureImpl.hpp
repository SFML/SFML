////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2025 Laurent Gomila (laurent@sfml-dev.org)
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

#pragma once

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics/Texture.hpp>

#include <SFML/System/Vector2.hpp>

#include <cstdint>


namespace sf
{
class Image;
class Window;

namespace priv
{
////////////////////////////////////////////////////////////
/// \brief Abstract base class for texture implementations
///
////////////////////////////////////////////////////////////
class TextureImpl
{
public:
    ////////////////////////////////////////////////////////////
    /// \brief Destructor
    ///
    ////////////////////////////////////////////////////////////
    virtual ~TextureImpl() = default;

    ////////////////////////////////////////////////////////////
    /// \brief Resize the texture
    ///
    /// \param size           New size of the texture
    /// \param sRgb           Should the texture source be converted from sRGB?
    /// \param isSmooth       Is the texture smooth?
    /// \param isRepeated    Is the texture repeated?
    /// \param actualSize     Output: actual size of the texture
    /// \param sRgbResult     Output: was sRGB actually enabled?
    /// \param cacheId        Output: new cache identifier
    ///
    /// \return `true` if resizing has been successful
    ///
    ////////////////////////////////////////////////////////////
    virtual bool resize(Vector2u size, bool sRgb, bool isSmooth, bool isRepeated, Vector2u& actualSize, bool& sRgbResult, std::uint64_t& cacheId) = 0;

    ////////////////////////////////////////////////////////////
    /// \brief Update the texture from an array of pixels
    ///
    /// \param pixels   Array of pixels to copy to the texture
    /// \param size     Size of the pixel region to update
    /// \param position Offset in the texture where to copy the pixels
    ///
    ////////////////////////////////////////////////////////////
    virtual void update(const std::uint8_t* pixels, Vector2u size, Vector2u position) = 0;

    ////////////////////////////////////////////////////////////
    /// \brief Update the texture from a window
    ///
    /// \param window   Window to copy to the texture
    /// \param position Offset in the texture where to copy the pixels
    ///
    /// \return `true` if the update was successful
    ///
    ////////////////////////////////////////////////////////////
    virtual bool update(const Window& window, Vector2u position) = 0;

    ////////////////////////////////////////////////////////////
    /// \brief Update the texture from another texture
    ///
    /// \param texture  Texture to copy to this texture
    /// \param position Offset in this texture where to copy the pixels
    ///
    /// \return `true` if the update was successful
    ///
    ////////////////////////////////////////////////////////////
    virtual bool update(const Texture& texture, Vector2u position) = 0;

    ////////////////////////////////////////////////////////////
    /// \brief Copy the texture pixels to an image
    ///
    /// \param size           Size of the texture
    /// \param pixelsFlipped  Are the pixels flipped?
    ///
    /// \return Image containing the texture pixels
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] virtual Image copyToImage(Vector2u size, bool pixelsFlipped) const = 0;

    ////////////////////////////////////////////////////////////
    /// \brief Enable or disable texture smoothing
    ///
    /// \param smooth `true` to enable smoothing, `false` to disable it
    ///
    ////////////////////////////////////////////////////////////
    virtual void setSmooth(bool smooth) = 0;

    ////////////////////////////////////////////////////////////
    /// \brief Enable or disable texture repeating
    ///
    /// \param repeated `true` to enable repeating, `false` to disable it
    ///
    ////////////////////////////////////////////////////////////
    virtual void setRepeated(bool repeated) = 0;

    ////////////////////////////////////////////////////////////
    /// \brief Generate a mipmap using the current texture data
    ///
    /// \return `true` if mipmap generation was successful, `false` if unsuccessful
    ///
    ////////////////////////////////////////////////////////////
    virtual bool generateMipmap() = 0;

    ////////////////////////////////////////////////////////////
    /// \brief Get the underlying native handle of the texture
    ///
    /// \return Native handle of the texture or 0 if not yet created
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] virtual unsigned int getNativeHandle() const = 0;

    ////////////////////////////////////////////////////////////
    /// \brief Bind a texture for rendering
    ///
    /// \param texture Pointer to the texture to bind, can be null to use no texture
    ///
    ////////////////////////////////////////////////////////////
    static void bind(const Texture* texture);

    ////////////////////////////////////////////////////////////
    /// \brief Get the maximum texture size allowed
    ///
    /// \return Maximum size allowed for textures, in pixels
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] static unsigned int getMaximumSize();
};

} // namespace priv
} // namespace sf
