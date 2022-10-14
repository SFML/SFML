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
#include <SFML/Graphics/ImageLoader.hpp>
#include <SFML/System/Err.hpp>
#include <SFML/System/InputStream.hpp>
#include <SFML/System/Utils.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#include <filesystem>
#include <iomanip>
#include <iterator>
#include <ostream>


namespace
{
// stb_image callbacks that operate on a sf::InputStream
int read(void* user, char* data, int size)
{
    auto* stream = static_cast<sf::InputStream*>(user);
    return static_cast<int>(stream->read(data, size));
}
void skip(void* user, int size)
{
    auto* stream = static_cast<sf::InputStream*>(user);

    if (stream->seek(stream->tell() + size) == -1)
        sf::err() << "Failed to seek image loader input stream" << std::endl;
}
int eof(void* user)
{
    auto* stream = static_cast<sf::InputStream*>(user);
    return stream->tell() >= stream->getSize();
}

// stb_image callback for constructing a buffer
void bufferFromCallback(void* context, void* data, int size)
{
    auto* source = static_cast<std::uint8_t*>(data);
    auto* dest   = static_cast<std::vector<std::uint8_t>*>(context);
    std::copy(source, source + size, std::back_inserter(*dest));
}
} // namespace


namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
ImageLoader& ImageLoader::getInstance()
{
    static ImageLoader Instance;

    return Instance;
}


////////////////////////////////////////////////////////////
ImageLoader::ImageLoader()
{
    // Nothing to do
}


////////////////////////////////////////////////////////////
bool ImageLoader::loadImageFromFile(const std::filesystem::path& filename, std::vector<std::uint8_t>& pixels, Vector2u& size)
{
    // Clear the array (just in case)
    pixels.clear();

    // Load the image and get a pointer to the pixels in memory
    int            width    = 0;
    int            height   = 0;
    int            channels = 0;
    unsigned char* ptr      = stbi_load(filename.string().c_str(), &width, &height, &channels, STBI_rgb_alpha);

    if (ptr)
    {
        // Assign the image properties
        size.x = static_cast<unsigned int>(width);
        size.y = static_cast<unsigned int>(height);

        if (width > 0 && height > 0)
        {
            // Copy the loaded pixels to the pixel buffer
            pixels.resize(static_cast<std::size_t>(width * height * 4));
            memcpy(pixels.data(), ptr, pixels.size());
        }

        // Free the loaded pixels (they are now in our own pixel buffer)
        stbi_image_free(ptr);

        return true;
    }
    else
    {
        // Error, failed to load the image
        err() << "Failed to load image\n"
              << formatDebugPathInfo(filename) << "\nReason: " << stbi_failure_reason() << std::endl;

        return false;
    }
}


////////////////////////////////////////////////////////////
bool ImageLoader::loadImageFromMemory(const void* data, std::size_t dataSize, std::vector<std::uint8_t>& pixels, Vector2u& size)
{
    // Check input parameters
    if (data && dataSize)
    {
        // Clear the array (just in case)
        pixels.clear();

        // Load the image and get a pointer to the pixels in memory
        int         width    = 0;
        int         height   = 0;
        int         channels = 0;
        const auto* buffer   = static_cast<const unsigned char*>(data);
        unsigned char* ptr = stbi_load_from_memory(buffer, static_cast<int>(dataSize), &width, &height, &channels, STBI_rgb_alpha);

        if (ptr)
        {
            // Assign the image properties
            size.x = static_cast<unsigned int>(width);
            size.y = static_cast<unsigned int>(height);

            if (width > 0 && height > 0)
            {
                // Copy the loaded pixels to the pixel buffer
                pixels.resize(static_cast<std::size_t>(width * height * 4));
                memcpy(pixels.data(), ptr, pixels.size());
            }

            // Free the loaded pixels (they are now in our own pixel buffer)
            stbi_image_free(ptr);

            return true;
        }
        else
        {
            // Error, failed to load the image
            err() << "Failed to load image from memory. Reason: " << stbi_failure_reason() << std::endl;

            return false;
        }
    }
    else
    {
        err() << "Failed to load image from memory, no data provided" << std::endl;
        return false;
    }
}


////////////////////////////////////////////////////////////
bool ImageLoader::loadImageFromStream(InputStream& stream, std::vector<std::uint8_t>& pixels, Vector2u& size)
{
    // Clear the array (just in case)
    pixels.clear();

    // Make sure that the stream's reading position is at the beginning
    if (stream.seek(0) == -1)
    {
        err() << "Failed to seek image stream" << std::endl;
        return false;
    }

    // Setup the stb_image callbacks
    stbi_io_callbacks callbacks;
    callbacks.read = &read;
    callbacks.skip = &skip;
    callbacks.eof  = &eof;

    // Load the image and get a pointer to the pixels in memory
    int            width    = 0;
    int            height   = 0;
    int            channels = 0;
    unsigned char* ptr      = stbi_load_from_callbacks(&callbacks, &stream, &width, &height, &channels, STBI_rgb_alpha);

    if (ptr)
    {
        // Assign the image properties
        size.x = static_cast<unsigned int>(width);
        size.y = static_cast<unsigned int>(height);

        if (width && height)
        {
            // Copy the loaded pixels to the pixel buffer
            pixels.resize(static_cast<std::size_t>(width * height * 4));
            memcpy(pixels.data(), ptr, pixels.size());
        }

        // Free the loaded pixels (they are now in our own pixel buffer)
        stbi_image_free(ptr);

        return true;
    }
    else
    {
        // Error, failed to load the image
        err() << "Failed to load image from stream. Reason: " << stbi_failure_reason() << std::endl;

        return false;
    }
}


////////////////////////////////////////////////////////////
bool ImageLoader::saveImageToFile(const std::filesystem::path&     filename,
                                  const std::vector<std::uint8_t>& pixels,
                                  const Vector2u&                  size)
{
    // Make sure the image is not empty
    if (!pixels.empty() && (size.x > 0) && (size.y > 0))
    {
        // Deduce the image type from its extension

        // Extract the extension
        const std::filesystem::path extension     = filename.extension();
        const Vector2i              convertedSize = Vector2i(size);

        if (extension == ".bmp")
        {
            // BMP format
            if (stbi_write_bmp(filename.string().c_str(), convertedSize.x, convertedSize.y, 4, pixels.data()))
                return true;
        }
        else if (extension == ".tga")
        {
            // TGA format
            if (stbi_write_tga(filename.string().c_str(), convertedSize.x, convertedSize.y, 4, pixels.data()))
                return true;
        }
        else if (extension == ".png")
        {
            // PNG format
            if (stbi_write_png(filename.string().c_str(), convertedSize.x, convertedSize.y, 4, pixels.data(), 0))
                return true;
        }
        else if (extension == ".jpg" || extension == ".jpeg")
        {
            // JPG format
            if (stbi_write_jpg(filename.string().c_str(), convertedSize.x, convertedSize.y, 4, pixels.data(), 90))
                return true;
        }
    }

    err() << "Failed to save image\n" << formatDebugPathInfo(filename) << std::endl;
    return false;
}

////////////////////////////////////////////////////////////
bool ImageLoader::saveImageToMemory(const std::string&               format,
                                    std::vector<std::uint8_t>&       output,
                                    const std::vector<std::uint8_t>& pixels,
                                    const Vector2u&                  size)
{
    // Make sure the image is not empty
    if (!pixels.empty() && (size.x > 0) && (size.y > 0))
    {
        // Choose function based on format

        std::string    specified     = toLower(format);
        const Vector2i convertedSize = Vector2i(size);

        if (specified == "bmp")
        {
            // BMP format
            if (stbi_write_bmp_to_func(&bufferFromCallback, &output, convertedSize.x, convertedSize.y, 4, pixels.data()))
                return true;
        }
        else if (specified == "tga")
        {
            // TGA format
            if (stbi_write_tga_to_func(&bufferFromCallback, &output, convertedSize.x, convertedSize.y, 4, pixels.data()))
                return true;
        }
        else if (specified == "png")
        {
            // PNG format
            if (stbi_write_png_to_func(&bufferFromCallback, &output, convertedSize.x, convertedSize.y, 4, pixels.data(), 0))
                return true;
        }
        else if (specified == "jpg" || specified == "jpeg")
        {
            // JPG format
            if (stbi_write_jpg_to_func(&bufferFromCallback, &output, convertedSize.x, convertedSize.y, 4, pixels.data(), 90))
                return true;
        }
    }

    err() << "Failed to save image with format " << std::quoted(format) << std::endl;
    return false;
}

} // namespace priv

} // namespace sf
