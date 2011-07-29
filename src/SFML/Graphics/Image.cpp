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
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/ImageLoader.hpp>
#include <SFML/System/Err.hpp>
#include <algorithm>
#include <cstring>


namespace sf
{
////////////////////////////////////////////////////////////
Image::Image() :
myWidth (0),
myHeight(0)
{

}


////////////////////////////////////////////////////////////
void Image::Create(unsigned int width, unsigned int height, const Color& color)
{
    // Assign the new size
    myWidth = width;
    myHeight = height;

    // Resize the pixel buffer
    myPixels.resize(width * height * 4);

    // Fill it with the specified color
    Uint8* ptr = &myPixels[0];
    Uint8* end = ptr + myPixels.size();
    while (ptr < end)
    {
        *ptr++ = color.r;
        *ptr++ = color.g;
        *ptr++ = color.b;
        *ptr++ = color.a;
    }
}


////////////////////////////////////////////////////////////
void Image::Create(unsigned int width, unsigned int height, const Uint8* pixels)
{
    if (pixels)
    {
        // Assign the new size
        myWidth = width;
        myHeight = height;

        // Copy the pixels
        std::size_t size = width * height * 4;
        myPixels.resize(size);
        std::memcpy(&myPixels[0], pixels, size); // faster than vector::assign
    }
    else
    {
        // Create an empty image
        myWidth = 0;
        myHeight = 0;
        myPixels.clear();
    }
}


////////////////////////////////////////////////////////////
bool Image::LoadFromFile(const std::string& filename)
{
    return priv::ImageLoader::GetInstance().LoadImageFromFile(filename, myPixels, myWidth, myHeight);
}


////////////////////////////////////////////////////////////
bool Image::LoadFromMemory(const void* data, std::size_t size)
{
    return priv::ImageLoader::GetInstance().LoadImageFromMemory(data, size, myPixels, myWidth, myHeight);
}


////////////////////////////////////////////////////////////
bool Image::LoadFromStream(InputStream& stream)
{
    return priv::ImageLoader::GetInstance().LoadImageFromStream(stream, myPixels, myWidth, myHeight);
}


////////////////////////////////////////////////////////////
bool Image::SaveToFile(const std::string& filename) const
{
    return priv::ImageLoader::GetInstance().SaveImageToFile(filename, myPixels, myWidth, myHeight);
}


////////////////////////////////////////////////////////////
unsigned int Image::GetWidth() const
{
    return myWidth;
}


////////////////////////////////////////////////////////////
unsigned int Image::GetHeight() const
{
    return myHeight;
}


////////////////////////////////////////////////////////////
void Image::CreateMaskFromColor(const Color& color, Uint8 alpha)
{
    // Make sure that the image is not empty
    if (!myPixels.empty())
    {
        // Replace the alpha of the pixels that match the transparent color
        Uint8* ptr = &myPixels[0];
        Uint8* end = ptr + myPixels.size();
        while (ptr < end)
        {
            if ((ptr[0] == color.r) && (ptr[1] == color.g) && (ptr[2] == color.b) && (ptr[3] == color.a))
                ptr[3] = alpha;
            ptr += 4;
        }
    }
}


////////////////////////////////////////////////////////////
void Image::Copy(const Image& source, unsigned int destX, unsigned int destY, const IntRect& sourceRect, bool applyAlpha)
{
    // Make sure that both images are valid
    if ((source.myWidth == 0) || (source.myHeight == 0) || (myWidth == 0) || (myHeight == 0))
        return;

    // Adjust the source rectangle
    IntRect srcRect = sourceRect;
    if (srcRect.Width == 0 || (srcRect.Height == 0))
    {
        srcRect.Left   = 0;
        srcRect.Top    = 0;
        srcRect.Width  = source.myWidth;
        srcRect.Height = source.myHeight;
    }
    else
    {
        if (srcRect.Left   < 0) srcRect.Left = 0;
        if (srcRect.Top    < 0) srcRect.Top  = 0;
        if (srcRect.Width  > static_cast<int>(source.myWidth))  srcRect.Width  = source.myWidth;
        if (srcRect.Height > static_cast<int>(source.myHeight)) srcRect.Height = source.myHeight;
    }

    // Then find the valid bounds of the destination rectangle
    int width  = srcRect.Width;
    int height = srcRect.Height;
    if (destX + width  > myWidth)  width  = myWidth  - destX;
    if (destY + height > myHeight) height = myHeight - destY;

    // Make sure the destination area is valid
    if ((width <= 0) || (height <= 0))
        return;

    // Precompute as much as possible
    int          pitch     = width * 4;
    int          rows      = height;
    int          srcStride = source.myWidth * 4;
    int          dstStride = myWidth * 4;
    const Uint8* srcPixels = &source.myPixels[0] + (srcRect.Left + srcRect.Top * source.myWidth) * 4;
    Uint8*       dstPixels = &myPixels[0] + (destX + destY * myWidth) * 4;

    // Copy the pixels
    if (applyAlpha)
    {
        // Interpolation using alpha values, pixel by pixel (slower)
        for (int i = 0; i < rows; ++i)
        {
            for (int j = 0; j < width; ++j)
            {
                // Get a direct pointer to the components of the current pixel
                const Uint8* src = srcPixels + j * 4;
                Uint8*       dst = dstPixels + j * 4;

                // Interpolate RGBA components using the alpha value of the source pixel
                Uint8 alpha = src[3];
                dst[0] = (src[0] * alpha + dst[0] * (255 - alpha)) / 255;
                dst[1] = (src[1] * alpha + dst[1] * (255 - alpha)) / 255;
                dst[2] = (src[2] * alpha + dst[2] * (255 - alpha)) / 255;
                dst[3] = alpha + dst[3] * (255 - alpha) / 255;
            }

            srcPixels += srcStride;
            dstPixels += dstStride;
        }
    }
    else
    {
        // Optimized copy ignoring alpha values, row by row (faster)
        for (int i = 0; i < rows; ++i)
        {
            std::memcpy(dstPixels, srcPixels, pitch);
            srcPixels += srcStride;
            dstPixels += dstStride;
        }
    }
}


////////////////////////////////////////////////////////////
void Image::SetPixel(unsigned int x, unsigned int y, const Color& color)
{
    Uint8* pixel = &myPixels[(x + y * myWidth) * 4];
    *pixel++ = color.r;
    *pixel++ = color.g;
    *pixel++ = color.b;
    *pixel++ = color.a;
}


////////////////////////////////////////////////////////////
Color Image::GetPixel(unsigned int x, unsigned int y) const
{
    const Uint8* pixel = &myPixels[(x + y * myWidth) * 4];
    return Color(pixel[0], pixel[1], pixel[2], pixel[3]);
}


////////////////////////////////////////////////////////////
const Uint8* Image::GetPixelsPtr() const
{
    if (!myPixels.empty())
    {
        return &myPixels[0];
    }
    else
    {
        Err() << "Trying to access the pixels of an empty image" << std::endl;
        return NULL;
    }
}


////////////////////////////////////////////////////////////
void Image::FlipHorizontally()
{
    if (!myPixels.empty())
    {
        std::vector<Uint8> before = myPixels;
        for (unsigned int y = 0; y < myHeight; ++y)
        {
            const Uint8* source = &before[y * myWidth * 4];
            Uint8* dest = &myPixels[(y + 1) * myWidth * 4 - 4];
            for (unsigned int x = 0; x < myWidth; ++x)
            {
                dest[0] = source[0];
                dest[1] = source[1];
                dest[2] = source[2];
                dest[3] = source[3];

                source += 4;
                dest -= 4;
            }
        }
    }
}


////////////////////////////////////////////////////////////
void Image::FlipVertically()
{
    if (!myPixels.empty())
    {
        std::vector<Uint8> before = myPixels;
        const Uint8* source = &before[myWidth * (myHeight - 1) * 4];
        Uint8* dest = &myPixels[0];
        std::size_t rowSize = myWidth * 4;

        for (unsigned int y = 0; y < myHeight; ++y)
        {
            std::memcpy(dest, source, rowSize);
            source -= rowSize;
            dest += rowSize;
        }
    }
}

} // namespace sf
