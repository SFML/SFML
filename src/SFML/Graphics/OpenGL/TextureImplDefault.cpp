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
#include <SFML/Graphics/OpenGL/TextureImplDefault.hpp>
#include <SFML/Graphics/GLCheck.hpp>
#include <SFML/Graphics/GLExtensions.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/TextureSaver.hpp>

#include <SFML/Window/Context.hpp>
#include <SFML/Window/Window.hpp>

#include <SFML/System/Err.hpp>

#include <algorithm>
#include <atomic>
#include <ostream>
#include <vector>

#include <cassert>
#include <cstring>


namespace
{
// Thread-safe unique identifier generator,
// is used for states cache (see RenderTarget)
std::uint64_t getUniqueId() noexcept
{
    static std::atomic<std::uint64_t> id(1); // start at 1, zero is "no texture"

    return id.fetch_add(1);
}

// Get the next power of two of an integer
unsigned int getValidSize(unsigned int size)
{
    if (sf::Context::isExtensionAvailable("GL_ARB_texture_non_power_of_two"))
    {
        return size;
    }

    unsigned int powerOfTwo = 1;
    while (powerOfTwo < size)
        powerOfTwo *= 2;

    return powerOfTwo;
}
} // namespace


namespace sf::priv
{
////////////////////////////////////////////////////////////
TextureImplDefault::TextureImplDefault() = default;


////////////////////////////////////////////////////////////
TextureImplDefault::~TextureImplDefault()
{
    // Destroy the OpenGL texture
    if (m_texture)
    {
        const GlResource::TransientContextLock lock;

        const GLuint texture = m_texture;
        glCheck(glDeleteTextures(1, &texture));
    }
}


////////////////////////////////////////////////////////////
bool TextureImplDefault::resize(Vector2u     size,
                                bool         sRgb,
                                bool         isSmooth,
                                bool         isRepeated,
                                Vector2u&    actualSize,
                                bool&        sRgbResult,
                                std::uint64_t& cacheId)
{
    const GlResource::TransientContextLock lock;

    // Make sure that extensions are initialized
    ensureExtensionsInit();

    // Compute the internal texture dimensions depending on NPOT textures support
    actualSize = Vector2u(getValidSize(size.x), getValidSize(size.y));

    // Check the maximum texture size
    const unsigned int maxSize = TextureImpl::getMaximumSize();
    if ((actualSize.x > maxSize) || (actualSize.y > maxSize))
    {
        err() << "Failed to create texture, its internal size is too high "
              << "(" << actualSize.x << "x" << actualSize.y << ", "
              << "maximum is " << maxSize << "x" << maxSize << ")" << std::endl;
        return false;
    }

    // Create the OpenGL texture if it doesn't exist yet
    if (!m_texture)
    {
        GLuint texture = 0;
        glCheck(glGenTextures(1, &texture));
        m_texture = texture;
    }

    // Make sure that the current texture binding will be preserved
    const TextureSaver save;

    static const bool textureEdgeClamp = GLEXT_texture_edge_clamp || GLEXT_GL_VERSION_1_2 ||
                                         Context::isExtensionAvailable("GL_EXT_texture_edge_clamp");

    if (!textureEdgeClamp)
    {
        static bool warned = false;

        if (!warned)
        {
            err() << "OpenGL extension SGIS_texture_edge_clamp unavailable" << '\n'
                  << "Artifacts may occur along texture edges" << '\n'
                  << "Ensure that hardware acceleration is enabled if available" << std::endl;

            warned = true;
        }
    }

    static const bool textureSrgb = GLEXT_texture_sRGB;

    sRgbResult = sRgb;

    if (sRgbResult && !textureSrgb)
    {
        static bool warned = false;

        if (!warned)
        {
#ifndef SFML_OPENGL_ES
            err() << "OpenGL extension EXT_texture_sRGB unavailable" << '\n';
#else
            err() << "OpenGL ES extension EXT_sRGB unavailable" << '\n';
#endif
            err() << "Automatic sRGB to linear conversion disabled" << std::endl;

            warned = true;
        }

        sRgbResult = false;
    }

#ifndef SFML_OPENGL_ES
    const GLint textureWrapParam = isRepeated ? GL_REPEAT : (textureEdgeClamp ? GLEXT_GL_CLAMP_TO_EDGE : GLEXT_GL_CLAMP);
#else
    const GLint textureWrapParam = isRepeated ? GL_REPEAT : GLEXT_GL_CLAMP_TO_EDGE;
#endif

    // Initialize the texture
    glCheck(glBindTexture(GL_TEXTURE_2D, m_texture));
    glCheck(glTexImage2D(GL_TEXTURE_2D,
                         0,
                         (sRgbResult ? GLEXT_GL_SRGB8_ALPHA8 : GL_RGBA),
                         static_cast<GLsizei>(actualSize.x),
                         static_cast<GLsizei>(actualSize.y),
                         0,
                         GL_RGBA,
                         GL_UNSIGNED_BYTE,
                         nullptr));
    glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, textureWrapParam));
    glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, textureWrapParam));
    glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, isSmooth ? GL_LINEAR : GL_NEAREST));
    glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, isSmooth ? GL_LINEAR : GL_NEAREST));
    cacheId = getUniqueId();

    return true;
}


////////////////////////////////////////////////////////////
void TextureImplDefault::update(const std::uint8_t* pixels, Vector2u size, Vector2u position)
{
    const GlResource::TransientContextLock lock;

    // Make sure that the current texture binding will be preserved
    const TextureSaver save;

    // Copy the pixels to the texture
    glCheck(glBindTexture(GL_TEXTURE_2D, m_texture));
    glCheck(glTexSubImage2D(GL_TEXTURE_2D,
                            0,
                            static_cast<GLint>(position.x),
                            static_cast<GLint>(position.y),
                            static_cast<GLsizei>(size.x),
                            static_cast<GLsizei>(size.y),
                            GL_RGBA,
                            GL_UNSIGNED_BYTE,
                            pixels));

    // Force an OpenGL flush, so that the texture will appear updated
    // in all contexts immediately (solves problems in multi-threaded apps)
    glCheck(glFlush());
}


////////////////////////////////////////////////////////////
bool TextureImplDefault::update(const Window& window, Vector2u position)
{
    if (m_texture)
    {
        const GlResource::TransientContextLock lock;

        // Make sure that the current texture binding will be preserved
        const TextureSaver save;

        glCheck(glBindTexture(GL_TEXTURE_2D, m_texture));
        glCheck(glCopyTexSubImage2D(GL_TEXTURE_2D,
                                    0,
                                    static_cast<GLint>(position.x),
                                    static_cast<GLint>(position.y),
                                    0,
                                    0,
                                    static_cast<GLsizei>(window.getSize().x),
                                    static_cast<GLsizei>(window.getSize().y)));

        // Force an OpenGL flush, so that the texture will appear updated
        // in all contexts immediately (solves problems in multi-threaded apps)
        glCheck(glFlush());

        return true;
    }

    return false;
}


////////////////////////////////////////////////////////////
bool TextureImplDefault::update(const Texture& texture, Vector2u position)
{
    if (m_texture && texture.getNativeHandle())
    {
        const GlResource::TransientContextLock lock;

        // Make sure that extensions are initialized
        ensureExtensionsInit();

        if (GLEXT_copy_image)
        {
            glCheck(GLEXT_glCopyImageSubData(texture.getNativeHandle(),
                                             GL_TEXTURE_2D,
                                             0,
                                             0,
                                             0,
                                             0,
                                             m_texture,
                                             GL_TEXTURE_2D,
                                             0,
                                             static_cast<GLint>(position.x),
                                             static_cast<GLint>(position.y),
                                             0,
                                             static_cast<GLsizei>(texture.getSize().x),
                                             static_cast<GLsizei>(texture.getSize().y),
                                             1));
        }
        else
        {
            // OpenGL ES or old OpenGL: use a FBO to copy from one texture to another
            GLuint frameBuffer = 0;
            glCheck(GLEXT_glGenFramebuffers(1, &frameBuffer));
            if (frameBuffer)
            {
                GLint previousFrameBuffer = 0;
                glCheck(glGetIntegerv(GLEXT_GL_FRAMEBUFFER_BINDING, &previousFrameBuffer));

                glCheck(GLEXT_glBindFramebuffer(GLEXT_GL_FRAMEBUFFER, frameBuffer));
                glCheck(GLEXT_glFramebufferTexture2D(GLEXT_GL_FRAMEBUFFER,
                                                     GLEXT_GL_COLOR_ATTACHMENT0,
                                                     GL_TEXTURE_2D,
                                                     texture.getNativeHandle(),
                                                     0));

                glCheck(glBindTexture(GL_TEXTURE_2D, m_texture));
                glCheck(glCopyTexSubImage2D(GL_TEXTURE_2D,
                                            0,
                                            static_cast<GLint>(position.x),
                                            static_cast<GLint>(position.y),
                                            0,
                                            0,
                                            static_cast<GLsizei>(texture.getSize().x),
                                            static_cast<GLsizei>(texture.getSize().y)));

                glCheck(GLEXT_glDeleteFramebuffers(1, &frameBuffer));
                glCheck(GLEXT_glBindFramebuffer(GLEXT_GL_FRAMEBUFFER, static_cast<GLuint>(previousFrameBuffer)));
            }
        }

        // Force an OpenGL flush, so that the texture will appear updated
        // in all contexts immediately (solves problems in multi-threaded apps)
        glCheck(glFlush());

        return true;
    }

    return false;
}


////////////////////////////////////////////////////////////
Image TextureImplDefault::copyToImage(Vector2u size, bool pixelsFlipped) const
{
    // Easy case: empty texture
    if (!m_texture)
        return {};

    const GlResource::TransientContextLock lock;

    // Make sure that the current texture binding will be preserved
    const TextureSaver save;

    // Create an array of pixels
    std::vector<std::uint8_t> pixels(size.x * size.y * 4);

#ifdef SFML_OPENGL_ES

    // OpenGL ES doesn't have the glGetTexImage function, the only way to read
    // from a texture is to bind it to a FBO and use glReadPixels
    GLuint frameBuffer = 0;
    glCheck(GLEXT_glGenFramebuffers(1, &frameBuffer));
    if (frameBuffer)
    {
        GLint previousFrameBuffer = 0;
        glCheck(glGetIntegerv(GLEXT_GL_FRAMEBUFFER_BINDING, &previousFrameBuffer));

        glCheck(GLEXT_glBindFramebuffer(GLEXT_GL_FRAMEBUFFER, frameBuffer));
        glCheck(GLEXT_glFramebufferTexture2D(GLEXT_GL_FRAMEBUFFER, GLEXT_GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture, 0));
        glCheck(glReadPixels(0,
                             0,
                             static_cast<GLsizei>(size.x),
                             static_cast<GLsizei>(size.y),
                             GL_RGBA,
                             GL_UNSIGNED_BYTE,
                             pixels.data()));
        glCheck(GLEXT_glDeleteFramebuffers(1, &frameBuffer));

        glCheck(GLEXT_glBindFramebuffer(GLEXT_GL_FRAMEBUFFER, static_cast<GLuint>(previousFrameBuffer)));

        if (pixelsFlipped)
        {
            // Flip the texture vertically
            const auto stride             = static_cast<std::ptrdiff_t>(size.x * 4);
            auto       currentRowIterator = pixels.begin();
            auto       nextRowIterator    = pixels.begin() + stride;
            auto       reverseRowIterator = pixels.begin() + (stride * static_cast<std::ptrdiff_t>(size.y - 1));
            for (unsigned int i = 0; i < size.y / 2; ++i)
            {
                std::swap_ranges(currentRowIterator, nextRowIterator, reverseRowIterator);
                currentRowIterator = nextRowIterator;
                nextRowIterator += stride;
                reverseRowIterator -= stride;
            }
        }
    }

#else

    // Desktop OpenGL can retrieve the texture pixels directly
    glCheck(glBindTexture(GL_TEXTURE_2D, m_texture));
    glCheck(glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data()));

#endif // SFML_OPENGL_ES

    (void)pixelsFlipped;
    Image image(size, pixels.data());

    return image;
}


////////////////////////////////////////////////////////////
void TextureImplDefault::setSmooth(bool smooth)
{
    if (m_texture)
    {
        const GlResource::TransientContextLock lock;

        // Make sure that the current texture binding will be preserved
        const TextureSaver save;

        glCheck(glBindTexture(GL_TEXTURE_2D, m_texture));
        glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, smooth ? GL_LINEAR : GL_NEAREST));
        glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, smooth ? GL_LINEAR : GL_NEAREST));
    }
}


////////////////////////////////////////////////////////////
void TextureImplDefault::setRepeated(bool repeated)
{
    if (m_texture)
    {
        const GlResource::TransientContextLock lock;

        // Make sure that the current texture binding will be preserved
        const TextureSaver save;

        static const bool textureEdgeClamp = GLEXT_texture_edge_clamp || GLEXT_GL_VERSION_1_2 ||
                                             Context::isExtensionAvailable("GL_EXT_texture_edge_clamp");

#ifndef SFML_OPENGL_ES
        const GLint textureWrapParam = repeated ? GL_REPEAT : (textureEdgeClamp ? GLEXT_GL_CLAMP_TO_EDGE : GLEXT_GL_CLAMP);
#else
        const GLint textureWrapParam = repeated ? GL_REPEAT : GLEXT_GL_CLAMP_TO_EDGE;
#endif

        glCheck(glBindTexture(GL_TEXTURE_2D, m_texture));
        glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, textureWrapParam));
        glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, textureWrapParam));
    }
}


////////////////////////////////////////////////////////////
bool TextureImplDefault::generateMipmap()
{
    if (!m_texture)
        return false;

    const GlResource::TransientContextLock lock;

    // Make sure that extensions are initialized
    ensureExtensionsInit();

    if (!GLEXT_framebuffer_object)
        return false;

    // Make sure that the current texture binding will be preserved
    const TextureSaver save;

    glCheck(glBindTexture(GL_TEXTURE_2D, m_texture));
    glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
    glCheck(GLEXT_glGenerateMipmap(GL_TEXTURE_2D));

    return true;
}


////////////////////////////////////////////////////////////
unsigned int TextureImplDefault::getNativeHandle() const
{
    return m_texture;
}


////////////////////////////////////////////////////////////
void TextureImpl::bind(const Texture* texture)
{
    const GlResource::TransientContextLock lock;

    if (texture && texture->getNativeHandle())
    {
        glCheck(glBindTexture(GL_TEXTURE_2D, texture->getNativeHandle()));
    }
    else
    {
        glCheck(glBindTexture(GL_TEXTURE_2D, 0));
    }
}


////////////////////////////////////////////////////////////
unsigned int TextureImpl::getMaximumSize()
{
    static const unsigned int size = []
    {
        const GlResource::TransientContextLock lock;

        GLint maxSize = 0;
        glCheck(glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxSize));
        return static_cast<unsigned int>(maxSize);
    }();

    return size;
}

} // namespace sf::priv
