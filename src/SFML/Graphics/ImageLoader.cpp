////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2009 Laurent Gomila (laurent.gom@gmail.com)
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
#include <SFML/Graphics/stb_image/stb_image.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <SFML/Graphics/stb_image/stb_image_write.h>
extern "C"
{
    #include <jpeglib.h>
    #include <jerror.h>
}
#include <cctype>


namespace
{
    // Convert a string to lower case
    std::string ToLower(std::string str)
    {
        for (std::string::iterator i = str.begin(); i != str.end(); ++i)
            *i = static_cast<char>(std::tolower(*i));
        return str;
    }
}


namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
ImageLoader& ImageLoader::GetInstance()
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
ImageLoader::~ImageLoader()
{
    // Nothing to do
}


////////////////////////////////////////////////////////////
bool ImageLoader::LoadImageFromFile(const std::string& filename, std::vector<Uint8>& pixels, unsigned int& width, unsigned int& height)
{
    // Clear the array (just in case)
    pixels.clear();

    // Load the image and get a pointer to the pixels in memory
    int imgWidth, imgHeight, imgChannels;
    unsigned char* ptr = stbi_load(filename.c_str(), &imgWidth, &imgHeight, &imgChannels, STBI_rgb_alpha);

    if (ptr && imgWidth && imgHeight)
    {
        // Assign the image properties
        width  = imgWidth;
        height = imgHeight;

        // Copy the loaded pixels to the pixel buffer
        pixels.resize(width * height * 4);
        memcpy(&pixels[0], ptr, pixels.size());

        // Free the loaded pixels (they are now in our own pixel buffer)
        stbi_image_free(ptr);

        return true;
    }
    else
    {
        // Error, failed to load the image
        Err() << "Failed to load image \"" << filename << "\". Reason : " << stbi_failure_reason() << std::endl;

        return false;
    }
}


////////////////////////////////////////////////////////////
bool ImageLoader::LoadImageFromMemory(const void* data, std::size_t size, std::vector<Uint8>& pixels, unsigned int& width, unsigned int& height)
{
    // Check input parameters
    if (data && size)
    {
        // Clear the array (just in case)
        pixels.clear();

        // Load the image and get a pointer to the pixels in memory
        int imgWidth, imgHeight, imgChannels;
        const unsigned char* buffer = static_cast<const unsigned char*>(data);
        unsigned char* ptr = stbi_load_from_memory(buffer, static_cast<int>(size), &imgWidth, &imgHeight, &imgChannels, STBI_rgb_alpha);

        if (ptr && imgWidth && imgHeight)
        {
            // Assign the image properties
            width  = imgWidth;
            height = imgHeight;

            // Copy the loaded pixels to the pixel buffer
            pixels.resize(width * height * 4);
            memcpy(&pixels[0], ptr, pixels.size());

            // Free the loaded pixels (they are now in our own pixel buffer)
            stbi_image_free(ptr);

            return true;
        }
        else
        {
            // Error, failed to load the image
            Err() << "Failed to load image from memory. Reason : " << stbi_failure_reason() << std::endl;

            return false;
        }
    }
    else
    {
        Err() << "Failed to load image from memory, no data provided" << std::endl;
        return false;
    }
}


////////////////////////////////////////////////////////////
bool ImageLoader::SaveImageToFile(const std::string& filename, const std::vector<Uint8>& pixels, unsigned int width, unsigned int height)
{
    // Make sure the image is not empty
    if (!pixels.empty() && width && height)
    {
        // Deduce the image type from its extension
        if (filename.size() > 3)
        {
            // Extract the extension
            std::string extension = filename.substr(filename.size() - 3);

            if (ToLower(extension) == "bmp")
            {
                // BMP format
                if (stbi_write_bmp(filename.c_str(), width, height, 4, &pixels[0]))
                    return true;
            }
            else if (ToLower(extension) == "tga")
            {
                // TGA format
                if (stbi_write_tga(filename.c_str(), width, height, 4, &pixels[0]))
                    return true;
            }
            else if(ToLower(extension) == "png")
            {
                // PNG format
                if (stbi_write_png(filename.c_str(), width, height, 4, &pixels[0], 0))
                    return true;
            }
            else if (ToLower(extension) == "jpg")
            {
                // JPG format
                if (WriteJpg(filename, pixels, width, height))
                    return true;
            }
        }
    }

    Err() << "Failed to save image \"" << filename << "\"" << std::endl;
    return false;
}


////////////////////////////////////////////////////////////
bool ImageLoader::WriteJpg(const std::string& filename, const std::vector<Uint8>& pixels, unsigned int width, unsigned int height)
{
    // Open the file to write in
    FILE* file = fopen(filename.c_str(), "wb");
    if (!file)
        return false;

    // Initialize the error handler
    jpeg_compress_struct compressInfos;
    jpeg_error_mgr errorManager;
    compressInfos.err = jpeg_std_error(&errorManager);

    // Initialize all the writing and compression infos
    jpeg_create_compress(&compressInfos);
    compressInfos.image_width      = width;
    compressInfos.image_height     = height;
    compressInfos.input_components = 3;
    compressInfos.in_color_space   = JCS_RGB;
    jpeg_stdio_dest(&compressInfos, file);
    jpeg_set_defaults(&compressInfos);
    jpeg_set_quality(&compressInfos, 90, TRUE);

    // Get rid of the aplha channel
    std::vector<Uint8> buffer(width * height * 3);
    for (std::size_t i = 0; i < width * height; ++i)
    {
        buffer[i * 3 + 0] = pixels[i * 4 + 0];
        buffer[i * 3 + 1] = pixels[i * 4 + 1];
        buffer[i * 3 + 2] = pixels[i * 4 + 2];
    }
    Uint8* ptr = &buffer[0];

    // Start compression
    jpeg_start_compress(&compressInfos, TRUE);

    // Write each row of the image
    while (compressInfos.next_scanline < compressInfos.image_height)
    {
        JSAMPROW rawPointer = ptr + (compressInfos.next_scanline * width * 3);
        jpeg_write_scanlines(&compressInfos, &rawPointer, 1);
    }

    // Finish compression
    jpeg_finish_compress(&compressInfos);
    jpeg_destroy_compress(&compressInfos);

    // Close the file
    fclose(file);

    return true;
}

} // namespace priv

} // namespace sf
