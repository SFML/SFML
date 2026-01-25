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
#include <SFML/Graphics/GLCheck.hpp>
#include <SFML/Graphics/GLExtensions.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/TextureSaver.hpp>
#include <SFML/Graphics/OpenGL/TextureImplDefault.hpp>

#include <SFML/Window/Context.hpp>
#include <SFML/Window/Window.hpp>

#include <SFML/System/Err.hpp>
#include <SFML/System/Exception.hpp>

#include <algorithm>
#include <array>
#include <atomic>
#include <ostream>
#include <utility>

#include <cassert>
#include <cstring>


namespace
{
// A nested named namespace is used here to allow unity builds of SFML.
namespace TextureImpl
{
// Thread-safe unique identifier generator,
// is used for states cache (see RenderTarget)
std::uint64_t getUniqueId() noexcept
{
    static std::atomic<std::uint64_t> id(1); // start at 1, zero is "no texture"

    return id.fetch_add(1);
}
} // namespace TextureImpl
} // namespace


namespace sf
{
////////////////////////////////////////////////////////////
Texture::Texture() :
    m_impl(std::make_unique<priv::TextureImplDefault>()),
    m_cacheId(TextureImpl::getUniqueId())
{
}


////////////////////////////////////////////////////////////
Texture::Texture(const std::filesystem::path& filename, bool sRgb) : Texture()
{
    if (!loadFromFile(filename, sRgb))
        throw Exception("Failed to load texture from file");
}


////////////////////////////////////////////////////////////
Texture::Texture(const std::filesystem::path& filename, bool sRgb, const IntRect& area) : Texture()
{
    if (!loadFromFile(filename, sRgb, area))
        throw Exception("Failed to load texture from file");
}


////////////////////////////////////////////////////////////
Texture::Texture(const void* data, std::size_t size, bool sRgb) : Texture()
{
    if (!loadFromMemory(data, size, sRgb))
        throw Exception("Failed to load texture from memory");
}


////////////////////////////////////////////////////////////
Texture::Texture(const void* data, std::size_t size, bool sRgb, const IntRect& area) : Texture()
{
    if (!loadFromMemory(data, size, sRgb, area))
        throw Exception("Failed to load texture from memory");
}


////////////////////////////////////////////////////////////
Texture::Texture(InputStream& stream, bool sRgb) : Texture()
{
    if (!loadFromStream(stream, sRgb))
        throw Exception("Failed to load texture from stream");
}


////////////////////////////////////////////////////////////
Texture::Texture(InputStream& stream, bool sRgb, const IntRect& area) : Texture()
{
    if (!loadFromStream(stream, sRgb, area))
        throw Exception("Failed to load texture from stream");
}


////////////////////////////////////////////////////////////
Texture::Texture(const Image& image, bool sRgb) : Texture()
{
    if (!loadFromImage(image, sRgb))
        throw Exception("Failed to load texture from image");
}


////////////////////////////////////////////////////////////
Texture::Texture(const Image& image, bool sRgb, const IntRect& area) : Texture()
{
    if (!loadFromImage(image, sRgb, area))
        throw Exception("Failed to load texture from image");
}


////////////////////////////////////////////////////////////
Texture::Texture(Vector2u size, bool sRgb) : Texture()
{
    if (!resize(size, sRgb))
        throw Exception("Failed to create texture");
}


////////////////////////////////////////////////////////////
Texture::Texture(const Texture& copy) :
    GlResource(copy),
    m_impl(std::make_unique<priv::TextureImplDefault>()),
    m_isSmooth(copy.m_isSmooth),
    m_sRgb(copy.m_sRgb),
    m_isRepeated(copy.m_isRepeated),
    m_cacheId(TextureImpl::getUniqueId())
{
    if (copy.getNativeHandle() == 0)
    {
        return;
    }

    if (resize(copy.getSize(), copy.isSrgb()))
    {
        update(copy);
    }
    else
    {
        err() << "Failed to copy texture, failed to resize texture" << std::endl;
    }
}


////////////////////////////////////////////////////////////
Texture::~Texture()
{
#ifndef NDEBUG
    // Set m_cacheId to an invalid value to help the assert and glIsTexture in bind detect trying
    // to bind this texture in cases where it has already been destroyed but its memory not yet deallocated
    m_cacheId = 0xFFFFFFFFFFFFFFFFull;
#endif
}

////////////////////////////////////////////////////////////
Texture::Texture(Texture&& right) noexcept = default;


////////////////////////////////////////////////////////////
Texture& Texture::operator=(Texture&& right) noexcept = default;



////////////////////////////////////////////////////////////
bool Texture::resize(Vector2u size, bool sRgb)
{
    if (size.x == 0 || size.y == 0)
    {
        err() << "Failed to create texture, its size must be non-zero" << std::endl;
        return false;
    }
    if (m_impl->resize(size, sRgb, m_isSmooth, m_isRepeated, m_actualSize, m_sRgb, m_cacheId))
    {
        m_size          = size;
        m_pixelsFlipped = false;
        m_fboAttachment = false;
        m_hasMipmap     = false;
        return true;
    }

    return false;
}


////////////////////////////////////////////////////////////
bool Texture::loadFromFile(const std::filesystem::path& filename, bool sRgb, const IntRect& area)
{
    Image image;
    return image.loadFromFile(filename) && loadFromImage(image, sRgb, area);
}


////////////////////////////////////////////////////////////
bool Texture::loadFromMemory(const void* data, std::size_t size, bool sRgb, const IntRect& area)
{
    Image image;
    return image.loadFromMemory(data, size) && loadFromImage(image, sRgb, area);
}


////////////////////////////////////////////////////////////
bool Texture::loadFromStream(InputStream& stream, bool sRgb, const IntRect& area)
{
    Image image;
    return image.loadFromStream(stream) && loadFromImage(image, sRgb, area);
}


////////////////////////////////////////////////////////////
bool Texture::loadFromImage(const Image& image, bool sRgb, const IntRect& area)
{
    // Retrieve the image size
    const auto size = Vector2i(image.getSize());

    if (size.x == 0 || size.y == 0)
        return false;

    // Load the entire image if the source area is either empty or contains the whole image
    if (area.size.x == 0 || (area.size.y == 0) ||
        ((area.position.x <= 0) && (area.position.y <= 0) && (area.size.x >= size.x) && (area.size.y >= size.y)))
    {
        // Load the entire image
        if (resize(image.getSize(), sRgb))
        {
            update(image);
            return true;
        }

        // Error message generated in called function.
        return false;
    }

    // Load a sub-area of the image
    assert(area.size.x > 0 && "Area size x cannot be negative");
    assert(area.size.y > 0 && "Area size y cannot be negative");
    assert(area.position.x < size.x && "Area position x is out of image bounds");
    assert(area.position.y < size.y && "Area position y is out of image bounds");

    // Adjust the rectangle to the size of the image
    IntRect rectangle    = area;
    rectangle.position.x = std::max(rectangle.position.x, 0);
    rectangle.position.y = std::max(rectangle.position.y, 0);
    rectangle.size.x     = std::min(rectangle.size.x, size.x - rectangle.position.x);
    rectangle.size.y     = std::min(rectangle.size.y, size.y - rectangle.position.y);

    // Create the texture and upload the pixels
    if (resize(Vector2u(rectangle.size), sRgb))
    {
        // Copy the pixels to the texture, row by row
        const std::uint8_t* pixels = image.getPixelsPtr() + 4 * (rectangle.position.x + (size.x * rectangle.position.y));
        for (int i = 0; i < rectangle.size.y; ++i)
        {
            m_impl->update(pixels, {static_cast<unsigned int>(rectangle.size.x), 1}, {0, static_cast<unsigned int>(i)});
            pixels += 4 * size.x;
        }

        m_impl->setSmooth(m_isSmooth);
        m_hasMipmap = false;

        return true;
    }

    // Error message generated in called function.
    return false;
}


////////////////////////////////////////////////////////////
Vector2u Texture::getSize() const
{
    return m_size;
}


////////////////////////////////////////////////////////////
Image Texture::copyToImage() const
{
    return m_impl->copyToImage(m_size, m_pixelsFlipped);
}


////////////////////////////////////////////////////////////
void Texture::update(const std::uint8_t* pixels)
{
    // Update the whole texture
    update(pixels, m_size, {0, 0});
}


////////////////////////////////////////////////////////////
void Texture::update(const std::uint8_t* pixels, Vector2u size, Vector2u dest)
{
    assert(dest.x + size.x <= m_size.x && "Destination x coordinate is outside of texture");
    assert(dest.y + size.y <= m_size.y && "Destination y coordinate is outside of texture");

    if (!pixels)
        return;

    m_impl->update(pixels, size, dest);
    m_hasMipmap     = false;
    m_pixelsFlipped = false;
    m_cacheId       = TextureImpl::getUniqueId();
}


////////////////////////////////////////////////////////////
void Texture::update(const Texture& texture)
{
    // Update the whole texture
    update(texture, {0, 0});
}


////////////////////////////////////////////////////////////
void Texture::update(const Texture& texture, Vector2u dest)
{
    assert(dest.x + texture.m_size.x <= m_size.x && "Destination x coordinate is outside of texture");
    assert(dest.y + texture.m_size.y <= m_size.y && "Destination y coordinate is outside of texture");

    if (m_impl->update(texture, dest))
    {
        m_hasMipmap     = false;
        m_pixelsFlipped = false;
        m_cacheId       = TextureImpl::getUniqueId();
    }
}


////////////////////////////////////////////////////////////
void Texture::update(const Image& image)
{
    // Update the whole texture
    update(image.getPixelsPtr(), image.getSize(), {0, 0});
}


////////////////////////////////////////////////////////////
void Texture::update(const Image& image, Vector2u dest)
{
    update(image.getPixelsPtr(), image.getSize(), dest);
}


////////////////////////////////////////////////////////////
void Texture::update(const Window& window)
{
    update(window, {0, 0});
}


////////////////////////////////////////////////////////////
void Texture::update(const Window& window, Vector2u dest)
{
    assert(dest.x + window.getSize().x <= m_size.x && "Destination x coordinate is outside of texture");
    assert(dest.y + window.getSize().y <= m_size.y && "Destination y coordinate is outside of texture");

    if (m_impl->update(window, dest))
    {
        m_hasMipmap     = false;
        m_pixelsFlipped = true;
        m_cacheId       = TextureImpl::getUniqueId();
    }
}


////////////////////////////////////////////////////////////
void Texture::setSmooth(bool smooth)
{
    if (smooth == m_isSmooth)
    {
        return;
    }

    m_isSmooth = smooth;

    m_impl->setSmooth(m_isSmooth);
}


////////////////////////////////////////////////////////////
bool Texture::isSmooth() const
{
    return m_isSmooth;
}


////////////////////////////////////////////////////////////
bool Texture::isSrgb() const
{
    return m_sRgb;
}


////////////////////////////////////////////////////////////
void Texture::setRepeated(bool repeated)
{
    if (repeated == m_isRepeated)
    {
        return;
    }

    m_isRepeated = repeated;

    m_impl->setRepeated(m_isRepeated);
}


////////////////////////////////////////////////////////////
bool Texture::isRepeated() const
{
    return m_isRepeated;
}


////////////////////////////////////////////////////////////
bool Texture::generateMipmap()
{
    if (m_impl->generateMipmap())
    {
        m_hasMipmap = true;
        return true;
    }

    return false;
}


////////////////////////////////////////////////////////////
void Texture::invalidateMipmap()
{
    if (!m_hasMipmap)
        return;

    m_impl->setSmooth(m_isSmooth);

    m_hasMipmap = false;
}


////////////////////////////////////////////////////////////
void Texture::bind(const Texture* texture, CoordinateType coordinateType)
{
    const TransientContextLock lock;

    if (texture && texture->getNativeHandle())
    {
        // When debugging, ensure that the texture name is valid
        assert((glIsTexture(texture->getNativeHandle()) == GL_TRUE) &&
               "Texture to be bound is invalid, check if the texture is still being used after it has been destroyed");

        // Bind the texture
        glCheck(glBindTexture(GL_TEXTURE_2D, texture->getNativeHandle()));

        // Check if we need to define a special texture matrix
        if ((coordinateType == CoordinateType::Pixels) || texture->m_pixelsFlipped ||
            ((coordinateType == CoordinateType::Normalized) && (texture->m_size != texture->m_actualSize)))
        {
            // clang-format off
            std::array matrix = {1.f, 0.f, 0.f, 0.f,
                                 0.f, 1.f, 0.f, 0.f,
                                 0.f, 0.f, 1.f, 0.f,
                                 0.f, 0.f, 0.f, 1.f};
            // clang-format on

            // If non-normalized coordinates (= pixels) are requested, we need to
            // setup scale factors that convert the range [0 .. size] to [0 .. 1]
            if (coordinateType == CoordinateType::Pixels)
            {
                matrix[0] = 1.f / static_cast<float>(texture->m_actualSize.x);
                matrix[5] = 1.f / static_cast<float>(texture->m_actualSize.y);
            }

            // If normalized coordinates are used when NPOT textures aren't supported,
            // then we need to setup scale factors to make the coordinates relative to the actual POT size
            if ((coordinateType == CoordinateType::Normalized) && (texture->m_size != texture->m_actualSize))
            {
                matrix[0] = static_cast<float>(texture->m_size.x) / static_cast<float>(texture->m_actualSize.x);
                matrix[5] = static_cast<float>(texture->m_size.y) / static_cast<float>(texture->m_actualSize.y);
            }

            // If pixels are flipped we must invert the Y axis
            if (texture->m_pixelsFlipped)
            {
                matrix[5]  = -matrix[5];
                matrix[13] = static_cast<float>(texture->m_size.y) / static_cast<float>(texture->m_actualSize.y);
            }

            // Load the matrix
            glCheck(glMatrixMode(GL_TEXTURE));
            glCheck(glLoadMatrixf(matrix.data()));
        }
        else
        {
            // Reset the texture matrix
            glCheck(glMatrixMode(GL_TEXTURE));
            glCheck(glLoadIdentity());
        }

        // Go back to model-view mode (sf::RenderTarget relies on it)
        glCheck(glMatrixMode(GL_MODELVIEW));
    }
    else
    {
        // Bind no texture
        glCheck(glBindTexture(GL_TEXTURE_2D, 0));

        // Reset the texture matrix
        glCheck(glMatrixMode(GL_TEXTURE));
        glCheck(glLoadIdentity());

        // Go back to model-view mode (sf::RenderTarget relies on it)
        glCheck(glMatrixMode(GL_MODELVIEW));
    }
}


////////////////////////////////////////////////////////////
unsigned int Texture::getMaximumSize()
{
    return priv::TextureImpl::getMaximumSize();
}


////////////////////////////////////////////////////////////
Texture& Texture::operator=(const Texture& right)
{
    Texture temp(right);

    swap(temp);

    return *this;
}


////////////////////////////////////////////////////////////
void Texture::swap(Texture& right) noexcept
{
    std::swap(m_impl, right.m_impl);
    std::swap(m_size, right.m_size);
    std::swap(m_actualSize, right.m_actualSize);
    std::swap(m_isSmooth, right.m_isSmooth);
    std::swap(m_sRgb, right.m_sRgb);
    std::swap(m_isRepeated, right.m_isRepeated);
    std::swap(m_pixelsFlipped, right.m_pixelsFlipped);
    std::swap(m_fboAttachment, right.m_fboAttachment);
    std::swap(m_hasMipmap, right.m_hasMipmap);
    std::swap(m_cacheId, right.m_cacheId);
}


////////////////////////////////////////////////////////////
unsigned int Texture::getNativeHandle() const
{
    return m_impl->getNativeHandle();
}


////////////////////////////////////////////////////////////
unsigned int Texture::getValidSize(unsigned int size)
{
    if (GLEXT_texture_non_power_of_two)
    {
        // If hardware supports NPOT textures, then just return the unmodified size
        return size;
    }

    // If hardware doesn't support NPOT textures, we calculate the nearest power of two
    unsigned int powerOfTwo = 1;
    while (powerOfTwo < size)
        powerOfTwo *= 2;

    return powerOfTwo;
}


////////////////////////////////////////////////////////////
void swap(Texture& left, Texture& right) noexcept
{
    left.swap(right);
}

} // namespace sf
