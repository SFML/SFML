////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2022 Laurent Gomila (laurent@sfml-dev.org)
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

#include <memory>
#include <ostream>


namespace sf
{
////////////////////////////////////////////////////////////
RenderTexture::RenderTexture() = default;


////////////////////////////////////////////////////////////
RenderTexture::~RenderTexture() = default;


////////////////////////////////////////////////////////////
bool RenderTexture::create(const Vector2u& size, const ContextSettings& settings)
{
    // Set texture to be in sRGB scale if requested
    m_texture.setSrgb(settings.sRgbCapable);

    // Create the texture
    if (!m_texture.create(size))
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
    if (!m_impl->create(size, m_texture.m_texture, settings))
        return false;

    // We can now initialize the render target part
    RenderTarget::initialize();

    return true;
}


////////////////////////////////////////////////////////////
unsigned int RenderTexture::getMaximumAntialiasingLevel()
{
    if (priv::RenderTextureImplFBO::isAvailable())
    {
        return priv::RenderTextureImplFBO::getMaximumAntialiasingLevel();
    }
    else
    {
        return priv::RenderTextureImplDefault::getMaximumAntialiasingLevel();
    }
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
    // Update the target texture
    if (m_impl && (priv::RenderTextureImplFBO::isAvailable() || setActive(true)))
    {
        m_impl->updateTexture(m_texture.m_texture);
        m_texture.m_pixelsFlipped = true;
        m_texture.invalidateMipmap();
    }
}


////////////////////////////////////////////////////////////
Vector2u RenderTexture::getSize() const
{
    return m_texture.getSize();
}


////////////////////////////////////////////////////////////
bool RenderTexture::isSrgb() const
{
    return m_impl->isSrgb();
}


////////////////////////////////////////////////////////////
const Texture& RenderTexture::getTexture() const
{
    return m_texture;
}

} // namespace sf
