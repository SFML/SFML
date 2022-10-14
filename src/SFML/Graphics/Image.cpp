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
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/ImageLoader.hpp>
#include <SFML/System/Err.hpp>
#ifdef SFML_SYSTEM_ANDROID
#include <SFML/System/Android/ResourceStream.hpp>
#endif
#include <algorithm>
#include <cstring>
#include <ostream>


namespace sf
{
////////////////////////////////////////////////////////////
void Image::create(const Vector2u& size, const Color& color)
{
    if (size.x && size.y)
    {
        // Create a new pixel buffer first for exception safety's sake
        std::vector<std::uint8_t> newPixels(static_cast<std::size_t>(size.x) * static_cast<std::size_t>(size.y) * 4);

        // Fill it with the specified color
        std::uint8_t* ptr = newPixels.data();
        std::uint8_t* end = ptr + newPixels.size();
        while (ptr < end)
        {
            *ptr++ = color.r;
            *ptr++ = color.g;
            *ptr++ = color.b;
            *ptr++ = color.a;
        }

        // Commit the new pixel buffer
        m_pixels.swap(newPixels);

        // Assign the new size
        m_size = size;
    }
    else
    {
        // Dump the pixel buffer
        std::vector<std::uint8_t>().swap(m_pixels);

        // Assign the new size
        m_size.x = 0;
        m_size.y = 0;
    }
}


////////////////////////////////////////////////////////////
void Image::create(const Vector2u& size, const std::uint8_t* pixels)
{
    if (pixels && size.x && size.y)
    {
        // Create a new pixel buffer first for exception safety's sake
        std::vector<std::uint8_t> newPixels(pixels, pixels + size.x * size.y * 4);

        // Commit the new pixel buffer
        m_pixels.swap(newPixels);

        // Assign the new size
        m_size = size;
    }
    else
    {
        // Dump the pixel buffer
        std::vector<std::uint8_t>().swap(m_pixels);

        // Assign the new size
        m_size.x = 0;
        m_size.y = 0;
    }
}


////////////////////////////////////////////////////////////
bool Image::loadFromFile(const std::filesystem::path& filename)
{
#ifndef SFML_SYSTEM_ANDROID

    return priv::ImageLoader::getInstance().loadImageFromFile(filename, m_pixels, m_size);

#else

    priv::ResourceStream stream(filename);
    return loadFromStream(stream);

#endif
}


////////////////////////////////////////////////////////////
bool Image::loadFromMemory(const void* data, std::size_t size)
{
    return priv::ImageLoader::getInstance().loadImageFromMemory(data, size, m_pixels, m_size);
}


////////////////////////////////////////////////////////////
bool Image::loadFromStream(InputStream& stream)
{
    return priv::ImageLoader::getInstance().loadImageFromStream(stream, m_pixels, m_size);
}


////////////////////////////////////////////////////////////
bool Image::saveToFile(const std::filesystem::path& filename) const
{
    return priv::ImageLoader::getInstance().saveImageToFile(filename, m_pixels, m_size);
}

////////////////////////////////////////////////////////////
bool Image::saveToMemory(std::vector<std::uint8_t>& output, const std::string& format) const
{
    return priv::ImageLoader::getInstance().saveImageToMemory(format, output, m_pixels, m_size);
}


////////////////////////////////////////////////////////////
Vector2u Image::getSize() const
{
    return m_size;
}


////////////////////////////////////////////////////////////
void Image::createMaskFromColor(const Color& color, std::uint8_t alpha)
{
    // Make sure that the image is not empty
    if (!m_pixels.empty())
    {
        // Replace the alpha of the pixels that match the transparent color
        std::uint8_t* ptr = m_pixels.data();
        std::uint8_t* end = ptr + m_pixels.size();
        while (ptr < end)
        {
            if ((ptr[0] == color.r) && (ptr[1] == color.g) && (ptr[2] == color.b) && (ptr[3] == color.a))
                ptr[3] = alpha;
            ptr += 4;
        }
    }
}


////////////////////////////////////////////////////////////
[[nodiscard]] bool Image::copy(const Image& source, const Vector2u& dest, const IntRect& sourceRect, bool applyAlpha)
{
    // Make sure that both images are valid
    if (source.m_size.x == 0 || source.m_size.y == 0 || m_size.x == 0 || m_size.y == 0)
        return false;

    IntRect srcRect(sourceRect);

    // Make sure the position and size of sourceRect are not negative
    if (srcRect.position.x < 0 || srcRect.position.y < 0 || srcRect.size.x < 0 || srcRect.size.y < 0)
        return false;

    // If the provided source rectangle is empty, use the entire source image
    if (srcRect.size.x == 0 || srcRect.size.y == 0)
        srcRect = IntRect({0, 0}, Vector2i(source.m_size));

    // Otherwise make sure the provided source rectangle fits into the source image
    else
    {
        const Vector2u end(srcRect.getEnd());

        if (source.m_size.x < end.x || source.m_size.y < end.y)
            return false;
    }

    // Make sure the destination position is within this image bounds
    if (dest.x >= m_size.x || dest.y >= m_size.y)
        return false;

    // Then find the valid size of the destination rectangle
    const Vector2u uSize(srcRect.size);
    const Vector2u dstSize(std::min(m_size.x - dest.x, uSize.x), std::min(m_size.y - dest.y, uSize.y));

    // Precompute as much as possible
    const std::size_t pitch     = dstSize.x * 4;
    const std::size_t srcStride = source.m_size.x * 4;
    const std::size_t dstStride = m_size.x * 4;

    const std::uint8_t* srcPixels = source.m_pixels.data() +
                                    (srcRect.position.x + srcRect.position.y * static_cast<int>(source.m_size.x)) * 4;
    std::uint8_t* dstPixels = m_pixels.data() + (dest.x + dest.y * m_size.x) * 4;

    // Copy the pixels
    if (applyAlpha)
    {
        // Interpolation using alpha values, pixel by pixel (slower)
        for (unsigned int i = 0; i < dstSize.y; ++i)
        {
            for (unsigned int j = 0; j < dstSize.x; ++j)
            {
                // Get a direct pointer to the components of the current pixel
                const std::uint8_t* src = srcPixels + j * 4;
                std::uint8_t*       dst = dstPixels + j * 4;

                // Interpolate RGBA components using the alpha values of the destination and source pixels
                std::uint8_t src_alpha = src[3];
                std::uint8_t dst_alpha = dst[3];
                std::uint8_t out_alpha = static_cast<std::uint8_t>(src_alpha + dst_alpha - src_alpha * dst_alpha / 255);

                dst[3] = out_alpha;

                if (out_alpha)
                    for (int k = 0; k < 3; k++)
                        dst[k] = static_cast<std::uint8_t>(
                            (src[k] * src_alpha + dst[k] * (out_alpha - src_alpha)) / out_alpha);
                else
                    for (int k = 0; k < 3; k++)
                        dst[k] = src[k];
            }

            srcPixels += srcStride;
            dstPixels += dstStride;
        }
    }
    else
    {
        // Optimized copy ignoring alpha values, row by row (faster)
        for (unsigned int i = 0; i < dstSize.y; ++i)
        {
            std::memcpy(dstPixels, srcPixels, pitch);
            srcPixels += srcStride;
            dstPixels += dstStride;
        }
    }

    return true;
}


////////////////////////////////////////////////////////////
void Image::setPixel(const Vector2u& coords, const Color& color)
{
    std::uint8_t* pixel = &m_pixels[(coords.x + coords.y * m_size.x) * 4];
    *pixel++            = color.r;
    *pixel++            = color.g;
    *pixel++            = color.b;
    *pixel++            = color.a;
}


////////////////////////////////////////////////////////////
Color Image::getPixel(const Vector2u& coords) const
{
    const std::uint8_t* pixel = &m_pixels[(coords.x + coords.y * m_size.x) * 4];
    return Color(pixel[0], pixel[1], pixel[2], pixel[3]);
}


////////////////////////////////////////////////////////////
const std::uint8_t* Image::getPixelsPtr() const
{
    if (!m_pixels.empty())
    {
        return m_pixels.data();
    }
    else
    {
        err() << "Trying to access the pixels of an empty image" << std::endl;
        return nullptr;
    }
}


////////////////////////////////////////////////////////////
void Image::flipHorizontally()
{
    if (!m_pixels.empty())
    {
        std::size_t rowSize = m_size.x * 4;

        for (std::size_t y = 0; y < m_size.y; ++y)
        {
            auto left = m_pixels.begin() + static_cast<std::vector<std::uint8_t>::iterator::difference_type>(y * rowSize);
            auto right = m_pixels.begin() +
                         static_cast<std::vector<std::uint8_t>::iterator::difference_type>((y + 1) * rowSize - 4);

            for (std::size_t x = 0; x < m_size.x / 2; ++x)
            {
                std::swap_ranges(left, left + 4, right);

                left += 4;
                right -= 4;
            }
        }
    }
}


////////////////////////////////////////////////////////////
void Image::flipVertically()
{
    if (!m_pixels.empty())
    {
        auto rowSize = static_cast<std::vector<std::uint8_t>::iterator::difference_type>(m_size.x * 4);

        auto top    = m_pixels.begin();
        auto bottom = m_pixels.end() - rowSize;

        for (std::size_t y = 0; y < m_size.y / 2; ++y)
        {
            std::swap_ranges(top, top + rowSize, bottom);

            top += rowSize;
            bottom -= rowSize;
        }
    }
}

} // namespace sf
