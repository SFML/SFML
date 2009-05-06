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
extern "C"
{
    #include <SFML/Graphics/libjpeg/jpeglib.h>
    #include <SFML/Graphics/libjpeg/jerror.h>
}
#include <SFML/Graphics/libpng/png.h>
#include <SFML/Graphics/SOIL/SOIL.h>
#include <iostream>


namespace
{
    ////////////////////////////////////////////////////////////
    /// Error callback for PNG writing
    ////////////////////////////////////////////////////////////
    void PngErrorHandler(png_structp Png, png_const_charp Message)
    {
        std::cerr << "Failed to write PNG image. Reason : " << Message << std::endl;
        longjmp(Png->jmpbuf, 1);
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
bool ImageLoader::LoadImageFromFile(const std::string& Filename, std::vector<Color>& Pixels, unsigned int& Width, unsigned int& Height)
{
    // Clear the array (just in case)
    Pixels.clear();

    // Load the image and get a pointer to the pixels in memory
    int ImgWidth, ImgHeight, ImgChannels;
    unsigned char* PixelsPtr = SOIL_load_image(Filename.c_str(), &ImgWidth, &ImgHeight, &ImgChannels, SOIL_LOAD_RGBA);

    if (PixelsPtr)
    {
        // Assign the image properties
        Width  = ImgWidth;
        Height = ImgHeight;

        // Copy the loaded pixels to the pixel buffer
        Pixels.resize(Width * Height);
        memcpy(&Pixels[0], PixelsPtr, Width * Height * 4);

        // Free the loaded pixels (they are now in our own pixel buffer)
        SOIL_free_image_data(PixelsPtr);

        return true;
    }
    else
    {
        // Error, failed to load the image
        std::cerr << "Failed to load image \"" << Filename << "\". Reason : " << SOIL_last_result() << std::endl;

        return false;
    }
}


////////////////////////////////////////////////////////////
/// Load pixels from an image file in memory
////////////////////////////////////////////////////////////
bool ImageLoader::LoadImageFromMemory(const char* Data, std::size_t SizeInBytes, std::vector<Color>& Pixels, unsigned int& Width, unsigned int& Height)
{
    // Clear the array (just in case)
    Pixels.clear();

    // Load the image and get a pointer to the pixels in memory
    const unsigned char* Buffer = reinterpret_cast<const unsigned char*>(Data);
    int Size = static_cast<int>(SizeInBytes);
    int ImgWidth, ImgHeight, ImgChannels;
    unsigned char* PixelsPtr = SOIL_load_image_from_memory(Buffer, Size, &ImgWidth, &ImgHeight, &ImgChannels, SOIL_LOAD_RGBA);

    if (PixelsPtr)
    {
        // Assign the image properties
        Width  = ImgWidth;
        Height = ImgHeight;

        // Copy the loaded pixels to the pixel buffer
        Pixels.resize(Width * Height);
        memcpy(&Pixels[0], PixelsPtr, Width * Height * 4);

        // Free the loaded pixels (they are now in our own pixel buffer)
        SOIL_free_image_data(PixelsPtr);

        return true;
    }
    else
    {
        // Error, failed to load the image
        std::cerr << "Failed to load image from memory. Reason : " << SOIL_last_result() << std::endl;

        return false;
    }
}


////////////////////////////////////////////////////////////
/// Save pixels to an image file
////////////////////////////////////////////////////////////
bool ImageLoader::SaveImageToFile(const std::string& Filename, const std::vector<Color>& Pixels, unsigned int Width, unsigned int Height)
{
    // Deduce the image type from its extension
    int Type = -1;
    if (Filename.size() > 3)
    {
        std::string Extension = Filename.substr(Filename.size() - 3);
        if      (Extension == "bmp" || Extension == "BMP") Type = SOIL_SAVE_TYPE_BMP;
        else if (Extension == "tga" || Extension == "TGA") Type = SOIL_SAVE_TYPE_TGA;
        else if (Extension == "dds" || Extension == "DDS") Type = SOIL_SAVE_TYPE_DDS;

        // Special handling for PNG and JPG -- not handled by SOIL
        else if (Extension == "png" || Extension == "PNG") return WritePng(Filename, Pixels, Width, Height);
        else if (Extension == "jpg" || Extension == "JPG") return WriteJpg(Filename, Pixels, Width, Height);
    }

    if (Type == -1)
    {
        // Error, incompatible type
        std::cerr << "Failed to save image \"" << Filename << "\". Reason : this image format is not supported" << std::endl;
        return false;
    }

    // Finally save the image
    const unsigned char* PixelsPtr = reinterpret_cast<const unsigned char*>(&Pixels[0]);
    if (!SOIL_save_image(Filename.c_str(), Type, static_cast<int>(Width), static_cast<int>(Height), 4, PixelsPtr))
    {
        // Error, failed to save the image
        std::cerr << "Failed to save image \"" << Filename << "\". Reason : " << SOIL_last_result() << std::endl;
        return false;
    }

    return true;
}


////////////////////////////////////////////////////////////
/// Save a JPG image file
////////////////////////////////////////////////////////////
bool ImageLoader::WriteJpg(const std::string& Filename, const std::vector<Color>& Pixels, unsigned int Width, unsigned int Height)
{
    // Open the file to write in
    FILE* File = fopen(Filename.c_str(), "wb");
    if (!File)
    {
        std::cerr << "Failed to save image file \"" << Filename << "\". Reason : cannot open file" << std::endl;
        return false;
    }

    // Initialize the error handler
    jpeg_compress_struct CompressInfo;
    jpeg_error_mgr ErrorManager;
    CompressInfo.err = jpeg_std_error(&ErrorManager);

    // Initialize all the writing and compression infos
    jpeg_create_compress(&CompressInfo);
    CompressInfo.image_width      = Width;
    CompressInfo.image_height     = Height;
    CompressInfo.input_components = 3;
    CompressInfo.in_color_space   = JCS_RGB;
    jpeg_stdio_dest(&CompressInfo, File);
    jpeg_set_defaults(&CompressInfo);
    jpeg_set_quality(&CompressInfo, 90, TRUE);

    // Get rid of the aplha channel
    std::vector<Uint8> PixelsBuffer(Width * Height * 3);
    for (std::size_t i = 0; i < Pixels.size(); ++i)
    {
        PixelsBuffer[i * 3 + 0] = Pixels[i].r;
        PixelsBuffer[i * 3 + 1] = Pixels[i].g;
        PixelsBuffer[i * 3 + 2] = Pixels[i].b;
    }
    Uint8* PixelsPtr = &PixelsBuffer[0];

    // Start compression
    jpeg_start_compress(&CompressInfo, TRUE);

    // Write each row of the image
    while (CompressInfo.next_scanline < CompressInfo.image_height)
    {
        JSAMPROW RowPointer = PixelsPtr + (CompressInfo.next_scanline * Width * 3);
        jpeg_write_scanlines(&CompressInfo, &RowPointer, 1);
    }

    // Finish compression
    jpeg_finish_compress(&CompressInfo);
    jpeg_destroy_compress(&CompressInfo);

    // Close the file
    fclose(File);

    return true;
}


////////////////////////////////////////////////////////////
/// Save a PNG image file
////////////////////////////////////////////////////////////
bool ImageLoader::WritePng(const std::string& Filename, const std::vector<Color>& Pixels, unsigned int Width, unsigned int Height)
{
    // Open the file to write in
    FILE* File = fopen(Filename.c_str(), "wb");
    if (!File)
    {
        std::cerr << "Failed to save image file \"" << Filename << "\". Reason : cannot open file" << std::endl;
        return false;
    }

    // Create the main PNG structure
    png_structp Png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, &PngErrorHandler, NULL);
    if (!Png)
    {
        fclose(File);
        std::cerr << "Failed to save image file \"" << Filename << "\". Reason : cannot allocate PNG write structure" << std::endl;
        return false;
    }

    // Initialize the image informations
    png_infop PngInfo = png_create_info_struct(Png);
    if (!PngInfo)
    {
        fclose(File);
        png_destroy_write_struct(&Png, NULL);
        std::cerr << "Failed to save image file \"" << Filename << "\". Reason : cannot allocate PNG info structure" << std::endl;
        return false;
    }

    // For proper error handling...
    if (setjmp(Png->jmpbuf))
    {
        png_destroy_write_struct(&Png, &PngInfo);
        return false;
    }

    // Link the file to the PNG structure
    png_init_io(Png, File);

    // Set the image informations
    png_set_IHDR(Png, PngInfo, Width, Height, 8, PNG_COLOR_TYPE_RGB_ALPHA, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

    // Write the header
    png_write_info(Png, PngInfo);

    // Get the pointers to the pixels rows into an array
    png_byte* PixelsPtr = (png_byte*)&Pixels[0];
    std::vector<png_byte*> RowPointers(Height);
    for (unsigned int i = 0; i < Height; ++i)
    {
        RowPointers[i] = PixelsPtr;
        PixelsPtr += Width * 4;
    }

    // Write pixels row by row
    png_set_rows(Png, PngInfo, &RowPointers[0]);
    png_write_png(Png, PngInfo, PNG_TRANSFORM_IDENTITY, NULL);

    // Finish writing the file
    png_write_end(Png, PngInfo);

    // Cleanup resources
    png_destroy_write_struct(&Png, &PngInfo);
    fclose(File);

    return true;
}

} // namespace priv

} // namespace sf
