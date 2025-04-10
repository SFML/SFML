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
#include <SFML/Graphics/Image.hpp>

#include <SFML/System/Err.hpp>
#include <SFML/System/Exception.hpp>
#include <SFML/System/InputStream.hpp>
#include <SFML/System/Utils.hpp>
#ifdef SFML_SYSTEM_ANDROID
#include <SFML/System/Android/Activity.hpp>
#include <SFML/System/Android/ResourceStream.hpp>
#endif

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <memory>
#include <ostream>
#include <string>
#include <utility>

#include <cassert>
#include <cstring>


namespace
{
// stb_image callbacks that operate on a sf::InputStream
int read(void* user, char* data, int size)
{
    auto&               stream = *static_cast<sf::InputStream*>(user);
    const std::optional count  = stream.read(data, static_cast<std::size_t>(size));
    return count ? static_cast<int>(*count) : -1;
}

void skip(void* user, int size)
{
    auto& stream = *static_cast<sf::InputStream*>(user);
    if (!stream.seek(stream.tell().value() + static_cast<std::size_t>(size)).has_value())
        sf::err() << "Failed to seek image loader input stream" << std::endl;
}

int eof(void* user)
{
    auto& stream = *static_cast<sf::InputStream*>(user);
    return stream.tell() >= stream.getSize();
}

// stb_image callback for constructing a buffer
void bufferFromCallback(void* context, void* data, int size)
{
    const auto* source = static_cast<std::uint8_t*>(data);
    auto*       dest   = static_cast<std::vector<std::uint8_t>*>(context);
    std::copy(source, source + size, std::back_inserter(*dest));
}

// Deleter for STB pointers
struct StbDeleter
{
    void operator()(stbi_uc* image) const
    {
        stbi_image_free(image);
    }
};
using StbPtr = std::unique_ptr<stbi_uc, StbDeleter>;
} // namespace


namespace sf
{
////////////////////////////////////////////////////////////
Image::Image(Vector2u size, Color color)
{
    resize(size, color);
}


////////////////////////////////////////////////////////////
Image::Image(Vector2u size, const std::uint8_t* pixels)
{
    resize(size, pixels);
}


////////////////////////////////////////////////////////////
Image::Image(const std::filesystem::path& filename)
{
    if (!loadFromFile(filename))
        throw sf::Exception("Failed to open image from file");
}


////////////////////////////////////////////////////////////
Image::Image(const void* data, std::size_t size)
{
    if (!loadFromMemory(data, size))
        throw sf::Exception("Failed to open image from memory");
}


////////////////////////////////////////////////////////////
Image::Image(InputStream& stream)
{
    if (!loadFromStream(stream))
        throw sf::Exception("Failed to open image from stream");
}


////////////////////////////////////////////////////////////
void Image::resize(Vector2u size, Color color)
{
    if (size.x && size.y)
    {
        // Create a new pixel buffer first for exception safety's sake
        std::vector<std::uint8_t> newPixels(std::size_t{size.x} * std::size_t{size.y} * 4);

        // Fill it with the specified color
        std::uint8_t* ptr = newPixels.data();
        std::uint8_t* end = ptr + newPixels.size();
        while (ptr != end)
        {
            *ptr++ = color.r;
            *ptr++ = color.g;
            *ptr++ = color.b;
            *ptr++ = color.a;
        }

        // Commit the new pixel buffer
        m_pixels = std::move(newPixels);

        // Assign the new size
        m_size = size;
    }
    else
    {
        // Dump the pixel buffer
        std::vector<std::uint8_t>().swap(m_pixels);

        // Assign the new size
        m_size = {};
    }
}


////////////////////////////////////////////////////////////
void Image::resize(Vector2u size, const std::uint8_t* pixels)
{
    if (pixels && size.x && size.y)
    {
        // Create a new pixel buffer first for exception safety's sake
        std::vector<std::uint8_t> newPixels(pixels, pixels + size.x * size.y * 4);

        // Commit the new pixel buffer
        m_pixels = std::move(newPixels);

        // Assign the new size
        m_size = size;
    }
    else
    {
        // Dump the pixel buffer
        std::vector<std::uint8_t>().swap(m_pixels);

        // Assign the new size
        m_size = {};
    }
}


////////////////////////////////////////////////////////////
bool Image::loadFromFile(const std::filesystem::path& filename)
{
#ifdef SFML_SYSTEM_ANDROID

    if (priv::getActivityStatesPtr() != nullptr)
    {
        priv::ResourceStream stream;
        if (!stream.open(filename))
            return false;
        return loadFromStream(stream);
    }

#endif

    // Set up the stb_image callbacks for the std::ifstream
    const auto readStdIfStream = [](void* user, char* data, int size)
    {
        auto& file = *static_cast<std::ifstream*>(user);
        file.read(data, size);
        return static_cast<int>(file.gcount());
    };
    const auto skipStdIfStream = [](void* user, int size)
    {
        auto& file = *static_cast<std::ifstream*>(user);
        if (!file.seekg(size, std::ios_base::cur))
            err() << "Failed to seek image loader std::ifstream" << std::endl;
    };
    const auto eofStdIfStream = [](void* user)
    {
        auto& file = *static_cast<std::ifstream*>(user);
        return static_cast<int>(file.eof());
    };
    const stbi_io_callbacks callbacks{readStdIfStream, skipStdIfStream, eofStdIfStream};

    // Open file
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open())
    {
        // Error, failed to open the file
        err() << "Failed to load image\n"
              << formatDebugPathInfo(filename) << "\nReason: " << std::strerror(errno) << std::endl;
        return false;
    }

    // Load the image and get a pointer to the pixels in memory
    sf::Vector2i imageSize;
    int          channels = 0;
    if (const auto ptr = StbPtr(
            stbi_load_from_callbacks(&callbacks, &file, &imageSize.x, &imageSize.y, &channels, STBI_rgb_alpha)))
    {
        resize(Vector2u(imageSize), ptr.get());
        return true;
    }

    // Error, failed to load the image
    err() << "Failed to load image\n"
          << formatDebugPathInfo(filename) << "\nReason: " << stbi_failure_reason() << std::endl;

    return false;
}


////////////////////////////////////////////////////////////
bool Image::loadFromMemory(const void* data, std::size_t size)
{
    // Check input parameters
    if (data && size)
    {
        // Load the image and get a pointer to the pixels in memory
        Vector2i    imageSize;
        int         channels = 0;
        const auto* buffer   = static_cast<const unsigned char*>(data);
        if (const auto ptr = StbPtr(
                stbi_load_from_memory(buffer, static_cast<int>(size), &imageSize.x, &imageSize.y, &channels, STBI_rgb_alpha)))
        {
            resize(Vector2u(imageSize), ptr.get());
            return true;
        }

        // Error, failed to load the image
        err() << "Failed to load image from memory. Reason: " << stbi_failure_reason() << std::endl;

        return false;
    }

    err() << "Failed to load image from memory, no data provided" << std::endl;
    return false;
}


////////////////////////////////////////////////////////////
bool Image::loadFromStream(InputStream& stream)
{
    // Make sure that the stream's reading position is at the beginning
    if (!stream.seek(0).has_value())
    {
        err() << "Failed to seek image stream" << std::endl;
        return false;
    }

    // Setup the stb_image callbacks
    stbi_io_callbacks callbacks;
    callbacks.read = read;
    callbacks.skip = skip;
    callbacks.eof  = eof;

    // Load the image and get a pointer to the pixels in memory
    Vector2i imageSize;
    int      channels = 0;
    if (const auto ptr = StbPtr(
            stbi_load_from_callbacks(&callbacks, &stream, &imageSize.x, &imageSize.y, &channels, STBI_rgb_alpha)))
    {
        resize(Vector2u(imageSize), ptr.get());
        return true;
    }

    // Error, failed to load the image
    err() << "Failed to load image from stream. Reason: " << stbi_failure_reason() << std::endl;
    return false;
}


////////////////////////////////////////////////////////////
bool Image::saveToFile(const std::filesystem::path& filename) const
{
    // Make sure the image is not empty
    if (!m_pixels.empty() && m_size.x > 0 && m_size.y > 0)
    {
        // Deduce the image type from its extension

        // Extract the extension
        const std::filesystem::path extension     = filename.extension();
        const Vector2i              convertedSize = Vector2i(m_size);

        // Callback to write to std::ofstream
        auto writeStdOfstream = [](void* context, void* data, int size)
        {
            auto& file = *static_cast<std::ofstream*>(context);
            if (file)
                file.write(static_cast<const char*>(data), static_cast<std::streamsize>(size));
        };

        if (extension == ".bmp")
        {
            // BMP format
            std::ofstream file(filename, std::ios::binary);
            if (stbi_write_bmp_to_func(writeStdOfstream, &file, convertedSize.x, convertedSize.y, 4, m_pixels.data()) && file)
                return true;
        }
        else if (extension == ".tga")
        {
            // TGA format
            std::ofstream file(filename, std::ios::binary);
            if (stbi_write_tga_to_func(writeStdOfstream, &file, convertedSize.x, convertedSize.y, 4, m_pixels.data()) && file)
                return true;
        }
        else if (extension == ".png")
        {
            // PNG format
            std::ofstream file(filename, std::ios::binary);
            if (stbi_write_png_to_func(writeStdOfstream, &file, convertedSize.x, convertedSize.y, 4, m_pixels.data(), 0) &&
                file)
                return true;
        }
        else if (extension == ".jpg" || extension == ".jpeg")
        {
            // JPG format
            std::ofstream file(filename, std::ios::binary);
            if (stbi_write_jpg_to_func(writeStdOfstream, &file, convertedSize.x, convertedSize.y, 4, m_pixels.data(), 90) &&
                file)
                return true;
        }
        else
        {
            err() << "Image file extension " << extension << " not supported\n";
        }
    }

    err() << "Failed to save image\n" << formatDebugPathInfo(filename) << std::endl;
    return false;
}


////////////////////////////////////////////////////////////
std::optional<std::vector<std::uint8_t>> Image::saveToMemory(std::string_view format) const
{
    // Make sure the image is not empty
    if (!m_pixels.empty() && m_size.x > 0 && m_size.y > 0)
    {
        // Choose function based on format
        const std::string specified     = toLower(std::string(format));
        const Vector2i    convertedSize = Vector2i(m_size);

        std::vector<std::uint8_t> buffer;

        if (specified == "bmp")
        {
            // BMP format
            if (stbi_write_bmp_to_func(bufferFromCallback, &buffer, convertedSize.x, convertedSize.y, 4, m_pixels.data()))
                return buffer;
        }
        else if (specified == "tga")
        {
            // TGA format
            if (stbi_write_tga_to_func(bufferFromCallback, &buffer, convertedSize.x, convertedSize.y, 4, m_pixels.data()))
                return buffer;
        }
        else if (specified == "png")
        {
            // PNG format
            if (stbi_write_png_to_func(bufferFromCallback, &buffer, convertedSize.x, convertedSize.y, 4, m_pixels.data(), 0))
                return buffer;
        }
        else if (specified == "jpg" || specified == "jpeg")
        {
            // JPG format
            if (stbi_write_jpg_to_func(bufferFromCallback, &buffer, convertedSize.x, convertedSize.y, 4, m_pixels.data(), 90))
                return buffer;
        }
    }

    err() << "Failed to save image with format " << std::quoted(format) << std::endl;
    return std::nullopt;
}


////////////////////////////////////////////////////////////
Vector2u Image::getSize() const
{
    return m_size;
}


////////////////////////////////////////////////////////////
void Image::createMaskFromColor(Color color, std::uint8_t alpha)
{
    // Make sure that the image is not empty
    if (!m_pixels.empty())
    {
        // Replace the alpha of the pixels that match the transparent color
        std::uint8_t* ptr = m_pixels.data();
        std::uint8_t* end = ptr + m_pixels.size();
        while (ptr != end)
        {
            if ((ptr[0] == color.r) && (ptr[1] == color.g) && (ptr[2] == color.b) && (ptr[3] == color.a))
                ptr[3] = alpha;
            ptr += 4;
        }
    }
}


////////////////////////////////////////////////////////////
bool Image::copy(const Image& source, Vector2u dest, const IntRect& sourceRect, bool applyAlpha)
{
    // Make sure that both images are valid
    if (source.m_size.x == 0 || source.m_size.y == 0 || m_size.x == 0 || m_size.y == 0)
        return false;

    // Make sure the sourceRect components are non-negative before casting them to unsigned values
    if (sourceRect.position.x < 0 || sourceRect.position.y < 0 || sourceRect.size.x < 0 || sourceRect.size.y < 0)
        return false;

    Rect<unsigned int> srcRect(sourceRect);

    // Use the whole source image as srcRect if the provided source rectangle is empty
    if (srcRect.size.x == 0 || srcRect.size.y == 0)
    {
        srcRect = Rect<unsigned int>({0, 0}, source.m_size);
    }
    // Otherwise make sure the provided source rectangle fits into the source image
    else
    {
        // Checking the bottom right corner is enough because
        // left and top are non-negative and width and height are positive.
        if (source.m_size.x < srcRect.position.x + srcRect.size.x || source.m_size.y < srcRect.position.y + srcRect.size.y)
            return false;
    }

    // Make sure the destination position is within this image bounds
    if (m_size.x <= dest.x || m_size.y <= dest.y)
        return false;

    // Then find the valid size of the destination rectangle
    const Vector2u dstSize(std::min(m_size.x - dest.x, srcRect.size.x), std::min(m_size.y - dest.y, srcRect.size.y));

    // Precompute as much as possible
    const std::size_t  pitch     = static_cast<std::size_t>(dstSize.x) * 4;
    const unsigned int srcStride = source.m_size.x * 4;
    const unsigned int dstStride = m_size.x * 4;

    const std::uint8_t* srcPixels = source.m_pixels.data() + (srcRect.position.x + srcRect.position.y * source.m_size.x) * 4;
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
                const std::uint8_t srcAlpha = src[3];
                const std::uint8_t dstAlpha = dst[3];
                const auto outAlpha = static_cast<std::uint8_t>(srcAlpha + dstAlpha - srcAlpha * dstAlpha / 255);

                dst[3] = outAlpha;

                if (outAlpha)
                    for (int k = 0; k < 3; k++)
                        dst[k] = static_cast<std::uint8_t>((src[k] * srcAlpha + dst[k] * (outAlpha - srcAlpha)) / outAlpha);
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
void Image::setPixel(Vector2u coords, Color color)
{
    assert(coords.x < m_size.x && "Image::setPixel() x coordinate is out of bounds");
    assert(coords.y < m_size.y && "Image::setPixel() y coordinate is out of bounds");

    const auto    index = (coords.x + coords.y * m_size.x) * 4;
    std::uint8_t* pixel = &m_pixels[index];
    *pixel++            = color.r;
    *pixel++            = color.g;
    *pixel++            = color.b;
    *pixel++            = color.a;
}


////////////////////////////////////////////////////////////
Color Image::getPixel(Vector2u coords) const
{
    assert(coords.x < m_size.x && "Image::getPixel() x coordinate is out of bounds");
    assert(coords.y < m_size.y && "Image::getPixel() y coordinate is out of bounds");

    const auto          index = (coords.x + coords.y * m_size.x) * 4;
    const std::uint8_t* pixel = &m_pixels[index];
    return {pixel[0], pixel[1], pixel[2], pixel[3]};
}


////////////////////////////////////////////////////////////
const std::uint8_t* Image::getPixelsPtr() const
{
    if (!m_pixels.empty())
    {
        return m_pixels.data();
    }

    err() << "Trying to access the pixels of an empty image" << std::endl;
    return nullptr;
}


////////////////////////////////////////////////////////////
void Image::flipHorizontally()
{
    if (!m_pixels.empty())
    {
        const std::size_t rowSize = m_size.x * 4;

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
        const auto rowSize = static_cast<std::vector<std::uint8_t>::iterator::difference_type>(m_size.x * 4);

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
