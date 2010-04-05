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
extern "C"
{
    #include <jpeglib.h>
    #include <jerror.h>
}
#include <png.h>
#include <SOIL/SOIL.h>


namespace
{
    ////////////////////////////////////////////////////////////
    /// Error callback for PNG writing
    ////////////////////////////////////////////////////////////
    void PngErrorHandler(png_structp png, png_const_charp message)
    {
        sf::Err() << "Failed to write PNG image. Reason : " << message << std::endl;
        longjmp(png->jmpbuf, 1);
    }
}


namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
/// Get the unique instance of the class
////////////////////////////////////////////////////////////
ImageLoader& ImageLoader::GetInstance()
{
    static ImageLoader Instance;

    return Instance;
}


////////////////////////////////////////////////////////////
/// Default constructor
////////////////////////////////////////////////////////////
ImageLoader::ImageLoader()
{
    // Nothing to do
}


////////////////////////////////////////////////////////////
/// Destructor
////////////////////////////////////////////////////////////
ImageLoader::~ImageLoader()
{
    // Nothing to do
}


////////////////////////////////////////////////////////////
/// Load pixels from an image file
////////////////////////////////////////////////////////////
bool ImageLoader::LoadImageFromFile(const std::string& filename, std::vector<Uint8>& pixels, unsigned int& width, unsigned int& height)
{
    // Clear the array (just in case)
    pixels.clear();

    // Load the image and get a pointer to the pixels in memory
    int imgWidth, imgHeight, imgChannels;
    unsigned char* ptr = SOIL_load_image(filename.c_str(), &imgWidth, &imgHeight, &imgChannels, SOIL_LOAD_RGBA);

    if (ptr)
    {
        // Assign the image properties
        width  = imgWidth;
        height = imgHeight;

        // Copy the loaded pixels to the pixel buffer
        pixels.resize(width * height * 4);
        memcpy(&pixels[0], ptr, pixels.size());

        // Free the loaded pixels (they are now in our own pixel buffer)
        SOIL_free_image_data(ptr);

        return true;
    }
    else
    {
        // Error, failed to load the image
        Err() << "Failed to load image \"" << filename << "\". Reason : " << SOIL_last_result() << std::endl;

        return false;
    }
}


////////////////////////////////////////////////////////////
/// Load pixels from an image file in memory
////////////////////////////////////////////////////////////
bool ImageLoader::LoadImageFromMemory(const void* data, std::size_t sizeInBytes, std::vector<Uint8>& pixels, unsigned int& width, unsigned int& height)
{
    // Clear the array (just in case)
    pixels.clear();

    // Load the image and get a pointer to the pixels in memory
    const unsigned char* buffer = static_cast<const unsigned char*>(data);
    int size = static_cast<int>(sizeInBytes);
    int imgWidth, imgHeight, imgChannels;
    unsigned char* ptr = SOIL_load_image_from_memory(buffer, size, &imgWidth, &imgHeight, &imgChannels, SOIL_LOAD_RGBA);

    if (ptr)
    {
        // Assign the image properties
        width  = imgWidth;
        height = imgHeight;

        // Copy the loaded pixels to the pixel buffer
        pixels.resize(width * height * 4);
        memcpy(&pixels[0], ptr, pixels.size());

        // Free the loaded pixels (they are now in our own pixel buffer)
        SOIL_free_image_data(ptr);

        return true;
    }
    else
    {
        // Error, failed to load the image
        Err() << "Failed to load image from memory. Reason : " << SOIL_last_result() << std::endl;

        return false;
    }
}


////////////////////////////////////////////////////////////
/// Save pixels to an image file
////////////////////////////////////////////////////////////
bool ImageLoader::SaveImageToFile(const std::string& filename, const std::vector<Uint8>& pixels, unsigned int width, unsigned int height)
{
    // Deduce the image type from its extension
    int type = -1;
    if (filename.size() > 3)
    {
        std::string extension = filename.substr(filename.size() - 3);
        if      (extension == "bmp" || extension == "BMP") type = SOIL_SAVE_TYPE_BMP;
        else if (extension == "tga" || extension == "TGA") type = SOIL_SAVE_TYPE_TGA;
        else if (extension == "dds" || extension == "DDS") type = SOIL_SAVE_TYPE_DDS;

        // Special handling for PNG and JPG -- not handled by SOIL
        else if (extension == "png" || extension == "PNG") return WritePng(filename, pixels, width, height);
        else if (extension == "jpg" || extension == "JPG") return WriteJpg(filename, pixels, width, height);
    }

    if (type == -1)
    {
        // Error, incompatible type
        Err() << "Failed to save image \"" << filename << "\". Reason: this image format is not supported" << std::endl;
        return false;
    }

    // Finally save the image
    if (!SOIL_save_image(filename.c_str(), type, static_cast<int>(width), static_cast<int>(height), 4, &pixels[0]))
    {
        // Error, failed to save the image
        Err() << "Failed to save image \"" << filename << "\". Reason: " << SOIL_last_result() << std::endl;
        return false;
    }

    return true;
}


////////////////////////////////////////////////////////////
/// Save a JPG image file
////////////////////////////////////////////////////////////
bool ImageLoader::WriteJpg(const std::string& filename, const std::vector<Uint8>& pixels, unsigned int width, unsigned int height)
{
    // Open the file to write in
    FILE* file = fopen(filename.c_str(), "wb");
    if (!file)
    {
        Err() << "Failed to save image file \"" << filename << "\". Reason : cannot open file" << std::endl;
        return false;
    }

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


////////////////////////////////////////////////////////////
/// Save a PNG image file
////////////////////////////////////////////////////////////
bool ImageLoader::WritePng(const std::string& filename, const std::vector<Uint8>& pixels, unsigned int width, unsigned int height)
{
    // Open the file to write in
    FILE* file = fopen(filename.c_str(), "wb");
    if (!file)
    {
        Err() << "Failed to save image file \"" << filename << "\". Reason : cannot open file" << std::endl;
        return false;
    }

    // Create the main PNG structure
    png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, &PngErrorHandler, NULL);
    if (!png)
    {
        fclose(file);
        Err() << "Failed to save image file \"" << filename << "\". Reason : cannot allocate PNG write structure" << std::endl;
        return false;
    }

    // Initialize the image informations
    png_infop pngInfos = png_create_info_struct(png);
    if (!pngInfos)
    {
        fclose(file);
        png_destroy_write_struct(&png, NULL);
        Err() << "Failed to save image file \"" << filename << "\". Reason : cannot allocate PNG info structure" << std::endl;
        return false;
    }

    // For proper error handling...
    if (setjmp(png->jmpbuf))
    {
        png_destroy_write_struct(&png, &pngInfos);
        return false;
    }

    // Link the file to the PNG structure
    png_init_io(png, file);

    // Set the image informations
    png_set_IHDR(png, pngInfos, width, height, 8, PNG_COLOR_TYPE_RGB_ALPHA, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

    // Write the header
    png_write_info(png, pngInfos);

    // Get the pointers to the pixels rows into an array
    png_byte* ptr = (png_byte*)&pixels[0];
    std::vector<png_byte*> rawPointers(height);
    for (unsigned int i = 0; i < height; ++i)
    {
        rawPointers[i] = ptr;
        ptr += width * 4;
    }

    // Write pixels row by row
    png_set_rows(png, pngInfos, &rawPointers[0]);
    png_write_png(png, pngInfos, PNG_TRANSFORM_IDENTITY, NULL);

    // Finish writing the file
    png_write_end(png, pngInfos);

    // Cleanup resources
    png_destroy_write_struct(&png, &pngInfos);
    fclose(file);

    return true;
}

} // namespace priv

} // namespace sf
