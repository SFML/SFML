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
#include <SFML/Graphics/TextureImpl.hpp>

#include <SFML/Window/GlResource.hpp>


namespace sf::priv
{
////////////////////////////////////////////////////////////
/// \brief Default OpenGL implementation of textures
///
////////////////////////////////////////////////////////////
class TextureImplDefault : public TextureImpl, GlResource
{
public:
    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    ////////////////////////////////////////////////////////////
    TextureImplDefault();

    ////////////////////////////////////////////////////////////
    /// \brief Destructor
    ///
    ////////////////////////////////////////////////////////////
    ~TextureImplDefault() override;

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
    [[nodiscard]] bool resize(Vector2u size, bool sRgb, bool isSmooth, bool isRepeated, Vector2u& actualSize, bool& sRgbResult, std::uint64_t& cacheId) override;

    ////////////////////////////////////////////////////////////
    /// \brief Update the texture from an array of pixels
    ///
    /// \param pixels   Array of pixels to copy to the texture
    /// \param size     Size of the pixel region to update
    /// \param position Offset in the texture where to copy the pixels
    ///
    ////////////////////////////////////////////////////////////
    void update(const std::uint8_t* pixels, Vector2u size, Vector2u position) override;

    ////////////////////////////////////////////////////////////
    /// \brief Update the texture from a window
    ///
    /// \param window   Window to copy to the texture
    /// \param position Offset in the texture where to copy the pixels
    ///
    /// \return `true` if the update was successful
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] bool update(const Window& window, Vector2u position) override;

    ////////////////////////////////////////////////////////////
    /// \brief Update the texture from another texture
    ///
    /// \param texture  Texture to copy to this texture
    /// \param position Offset in this texture where to copy the pixels
    ///
    /// \return `true` if the update was successful
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] bool update(const Texture& texture, Vector2u position) override;

    ////////////////////////////////////////////////////////////
    /// \brief Copy the texture pixels to an image
    ///
    /// \param size           Size of the texture
    /// \param pixelsFlipped  Are the pixels flipped?
    ///
    /// \return Image containing the texture pixels
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] Image copyToImage(Vector2u size, bool pixelsFlipped) const override;

    ////////////////////////////////////////////////////////////
    /// \brief Enable or disable texture smoothing
    ///
    /// \param smooth `true` to enable smoothing, `false` to disable it
    ///
    ////////////////////////////////////////////////////////////
    void setSmooth(bool smooth) override;

    ////////////////////////////////////////////////////////////
    /// \brief Enable or disable texture repeating
    ///
    /// \param repeated `true` to enable repeating, `false` to disable it
    ///
    ////////////////////////////////////////////////////////////
    void setRepeated(bool repeated) override;

    ////////////////////////////////////////////////////////////
    /// \brief Generate a mipmap using the current texture data
    ///
    /// \return `true` if mipmap generation was successful, `false` if unsuccessful
    ///
    ////////////////////////////////////////////////////////////
    bool generateMipmap() override;

    ////////////////////////////////////////////////////////////
    /// \brief Get the underlying native handle of the texture
    ///
    /// \return Native handle of the texture or 0 if not yet created
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] unsigned int getNativeHandle() const override;

private:
    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    unsigned int m_texture{}; //!< Internal texture identifier
};

} // namespace sf::priv
