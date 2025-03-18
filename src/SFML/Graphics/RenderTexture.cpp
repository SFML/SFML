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

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/RenderTextureImplDefault.hpp>
#include <SFML/Graphics/RenderTextureImplFBO.hpp>

#include <SFML/System/Err.hpp>
#include <SFML/System/Exception.hpp>

#include <memory>
#include <ostream>

#include <cassert>


namespace sf
{
////////////////////////////////////////////////////////////
RenderTexture::RenderTexture() = default;


////////////////////////////////////////////////////////////
RenderTexture::RenderTexture(Vector2u size, const ContextSettings& settings)
{
    if (!resize(size, settings))
        throw sf::Exception("Failed to create render texture");
}


////////////////////////////////////////////////////////////
RenderTexture::~RenderTexture() = default;


////////////////////////////////////////////////////////////
RenderTexture::RenderTexture(RenderTexture&&) noexcept = default;


////////////////////////////////////////////////////////////
RenderTexture& RenderTexture::operator=(RenderTexture&&) noexcept = default;


////////////////////////////////////////////////////////////
bool RenderTexture::resize(Vector2u size, const ContextSettings& settings)
{
    // Create the texture
    // Set texture to be in sRGB scale if requested
    if (!m_texture.resize(size, settings.sRgbCapable))
    {
        err() << "Impossible to create render texture (failed to create the target texture)" << std::endl;
        return false;
    }

    // We disable smoothing by default for render textures
    setSmooth(false);

    // Create the implementation
    if (priv::RenderTextureImplFBO::isAvailable())
    {
        // Use frame-buffer object (FBO)
        m_impl = std::make_unique<priv::RenderTextureImplFBO>();

        // Mark the texture as being a framebuffer object attachment
        m_texture.m_fboAttachment = true;
    }
    else
    {
        // Use default implementation
        m_impl = std::make_unique<priv::RenderTextureImplDefault>();
    }

    // Initialize the render texture
    // We pass the actual size of our texture since OpenGL ES requires that all attachments have identical sizes
    if (!m_impl->create(m_texture.m_actualSize, m_texture.m_texture, settings))
        return false;

    // We can now initialize the render target part
    RenderTarget::initialize();

    return true;
}


////////////////////////////////////////////////////////////
unsigned int RenderTexture::getMaximumAntiAliasingLevel()
{
    if (priv::RenderTextureImplFBO::isAvailable())
    {
        return priv::RenderTextureImplFBO::getMaximumAntiAliasingLevel();
    }

    return priv::RenderTextureImplDefault::getMaximumAntiAliasingLevel();
}


////////////////////////////////////////////////////////////
void RenderTexture::setSmooth(bool smooth)
{
    m_texture.setSmooth(smooth);
}


////////////////////////////////////////////////////////////
bool RenderTexture::isSmooth() const
{
    return m_texture.isSmooth();
}


////////////////////////////////////////////////////////////
void RenderTexture::setRepeated(bool repeated)
{
    m_texture.setRepeated(repeated);
}


////////////////////////////////////////////////////////////
bool RenderTexture::isRepeated() const
{
    return m_texture.isRepeated();
}


////////////////////////////////////////////////////////////
bool RenderTexture::generateMipmap()
{
    return m_texture.generateMipmap();
}


////////////////////////////////////////////////////////////
bool RenderTexture::setActive(bool active)
{
    // Update RenderTarget tracking
    if (m_impl && m_impl->activate(active))
        return RenderTarget::setActive(active);

    return false;
}


////////////////////////////////////////////////////////////
void RenderTexture::display()
{
    if (!m_impl)
        return;

    if (priv::RenderTextureImplFBO::isAvailable())
    {
        // Perform a RenderTarget-only activation if we are using FBOs
        if (!RenderTarget::setActive())
            return;
    }
    else
    {
        // Perform a full activation if we are not using FBOs
        if (!setActive())
            return;
    }

    // Update the target texture
    m_impl->updateTexture(m_texture.m_texture);
    m_texture.m_pixelsFlipped = true;
    m_texture.invalidateMipmap();
}


////////////////////////////////////////////////////////////
Vector2u RenderTexture::getSize() const
{
    return m_texture.getSize();
}


////////////////////////////////////////////////////////////
bool RenderTexture::isSrgb() const
{
    assert(m_impl && "RenderTexture::isSrgb() Must first initialize render texture");
    return m_impl->isSrgb();
}


////////////////////////////////////////////////////////////
const Texture& RenderTexture::getTexture() const
{
    return m_texture;
}

} // namespace sf
