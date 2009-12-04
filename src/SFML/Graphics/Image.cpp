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
#include <SFML/Graphics/RenderImage.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/GLCheck.hpp>
#include <algorithm>
#include <iostream>
#include <vector>
#include <string.h>


namespace sf
{
////////////////////////////////////////////////////////////
/// Default constructor
////////////////////////////////////////////////////////////
Image::Image() :
myWidth            (0),
myHeight           (0),
myTextureWidth     (0),
myTextureHeight    (0),
myTexture          (0),
myIsSmooth         (true),
myNeedTextureUpdate(false),
myNeedArrayUpdate  (false),
myPixelsFlipped    (false)
{

}


////////////////////////////////////////////////////////////
/// Copy constructor
////////////////////////////////////////////////////////////
Image::Image(const Image& copy) :
Resource<Image>(copy)
{
    // First make sure that the source image is up-to-date
    const_cast<Image&>(copy).EnsureArrayUpdate();

    // Copy all its members
    myWidth             = copy.myWidth;
    myHeight            = copy.myHeight;
    myTextureWidth      = copy.myTextureWidth;
    myTextureHeight     = copy.myTextureHeight;
    myTexture           = 0;
    myIsSmooth          = copy.myIsSmooth;
    myPixels            = copy.myPixels;
    myNeedTextureUpdate = false;
    myNeedArrayUpdate   = false;
    myPixelsFlipped     = copy.myPixelsFlipped;

    // Create the texture
    CreateTexture();
}


////////////////////////////////////////////////////////////
/// Construct an empty image
////////////////////////////////////////////////////////////
Image::Image(unsigned int width, unsigned int height, const Color& color) :
myWidth            (0),
myHeight           (0),
myTextureWidth     (0),
myTextureHeight    (0),
myTexture          (0),
myIsSmooth         (true),
myNeedTextureUpdate(false),
myNeedArrayUpdate  (false),
myPixelsFlipped    (false)
{
    Create(width, height, color);
}


////////////////////////////////////////////////////////////
/// Construct the image from pixels in memory
////////////////////////////////////////////////////////////
Image::Image(unsigned int width, unsigned int height, const Uint8* data) :
myWidth            (0),
myHeight           (0),
myTextureWidth     (0),
myTextureHeight    (0),
myTexture          (0),
myIsSmooth         (true),
myNeedTextureUpdate(false),
myNeedArrayUpdate  (false),
myPixelsFlipped    (false)
{
    LoadFromPixels(width, height, data);
}


////////////////////////////////////////////////////////////
/// Destructor
////////////////////////////////////////////////////////////
Image::~Image()
{
    // Destroy the OpenGL texture
    DestroyTexture();
}


////////////////////////////////////////////////////////////
/// Load the image from a file
////////////////////////////////////////////////////////////
bool Image::LoadFromFile(const std::string& filename)
{
    // Let the image loader load the image into our pixel array
    bool success = priv::ImageLoader::GetInstance().LoadImageFromFile(filename, myPixels, myWidth, myHeight);

    if (success)
    {
        // Loading succeeded : we can create the texture
        if (CreateTexture())
            return true;
    }

    // Oops... something failed
    Reset();

    return false;
}


////////////////////////////////////////////////////////////
/// Load the image from a file in memory
////////////////////////////////////////////////////////////
bool Image::LoadFromMemory(const char* data, std::size_t sizeInBytes)
{
    // Check parameters
    if (!data || (sizeInBytes == 0))
    {
        std::cerr << "Failed to image font from memory, no data provided" << std::endl;
        return false;
    }

    // Let the image loader load the image into our pixel array
    bool success = priv::ImageLoader::GetInstance().LoadImageFromMemory(data, sizeInBytes, myPixels, myWidth, myHeight);

    if (success)
    {
        // Loading succeeded : we can create the texture
        if (CreateTexture())
            return true;
    }

    // Oops... something failed
    Reset();

    return false;
}


////////////////////////////////////////////////////////////
/// Load the image directly from an array of pixels
////////////////////////////////////////////////////////////
bool Image::LoadFromPixels(unsigned int width, unsigned int height, const Uint8* data)
{
    if (data)
    {
        // Store the texture dimensions
        myWidth  = width;
        myHeight = height;

        // Fill the pixel buffer with the specified raw data
        const Color* ptr = reinterpret_cast<const Color*>(data);
        myPixels.assign(ptr, ptr + width * height);

        // We can create the texture
        if (CreateTexture())
        {
            return true;
        }
        else
        {
            // Oops... something failed
            Reset();
            return false;
        }
    }
    else
    {
        // No data provided : create a white image
        return Create(width, height, Color(255, 255, 255));
    }
}


////////////////////////////////////////////////////////////
/// Save the content of the image to a file
////////////////////////////////////////////////////////////
bool Image::SaveToFile(const std::string& filename) const
{
    // Check if the array of pixels needs to be updated
    const_cast<Image*>(this)->EnsureArrayUpdate();

    // Let the image loader save our pixel array into the image
    return priv::ImageLoader::GetInstance().SaveImageToFile(filename, myPixels, myWidth, myHeight);
}


////////////////////////////////////////////////////////////
/// Create an empty image
////////////////////////////////////////////////////////////
bool Image::Create(unsigned int width, unsigned int height, const Color& color)
{
    // Store the texture dimensions
    myWidth  = width;
    myHeight = height;

    // Recreate the pixel buffer and fill it with the specified color
    myPixels.clear();
    myPixels.resize(width * height, color);

    // We can create the texture
    if (CreateTexture())
    {
        return true;
    }
    else
    {
        // Oops... something failed
        Reset();
        return false;
    }
}


////////////////////////////////////////////////////////////
/// Create transparency mask from a specified colorkey
////////////////////////////////////////////////////////////
void Image::CreateMaskFromColor(const Color& transparentColor, Uint8 alpha)
{
    // Check if the array of pixels needs to be updated
    EnsureArrayUpdate();

    // Calculate the new color (old color with no alpha)
    Color newColor(transparentColor.r, transparentColor.g, transparentColor.b, alpha);

    // Replace the old color with the new one
    std::replace(myPixels.begin(), myPixels.end(), transparentColor, newColor);

    // The texture will need to be updated
    myNeedTextureUpdate = true;
}


////////////////////////////////////////////////////////////
/// Copy pixels from another image onto this one.
/// This function does a slow pixel copy and should only
/// be used at initialization time
////////////////////////////////////////////////////////////
void Image::Copy(const Image& source, unsigned int destX, unsigned int destY, const IntRect& sourceRect, bool applyAlpha)
{
    // Make sure both images are valid
    if ((source.myWidth == 0) || (source.myHeight == 0) || (myWidth == 0) || (myHeight == 0))
        return;

    // Make sure both images have up-to-date arrays
    EnsureArrayUpdate();
    const_cast<Image&>(source).EnsureArrayUpdate();

    // Adjust the source rectangle
    IntRect srcRect = sourceRect;
    if (srcRect.GetSize().x == 0 || (srcRect.GetSize().y == 0))
    {
        srcRect.Left   = 0;
        srcRect.Top    = 0;
        srcRect.Right  = source.myWidth;
        srcRect.Bottom = source.myHeight;
    }
    else
    {
        if (srcRect.Left   < 0) srcRect.Left = 0;
        if (srcRect.Top    < 0) srcRect.Top  = 0;
        if (srcRect.Right  > static_cast<int>(source.myWidth))  srcRect.Right  = source.myWidth;
        if (srcRect.Bottom > static_cast<int>(source.myHeight)) srcRect.Bottom = source.myHeight;
    }

    // Then find the valid bounds of the destination rectangle
    int width  = srcRect.GetSize().x;
    int height = srcRect.GetSize().y;
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
    const Uint8* srcPixels = source.GetPixelsPtr() + (srcRect.Left + srcRect.Top * source.myWidth) * 4;
    Uint8*       dstPixels = reinterpret_cast<Uint8*>(&myPixels[0]) + (destX + destY * myWidth) * 4;

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

                // Interpolate RGB components using the alpha value of the source pixel
                Uint8 alpha = src[3];
                dst[0] = (src[0] * alpha + dst[0] * (255 - alpha)) / 255;
                dst[1] = (src[1] * alpha + dst[1] * (255 - alpha)) / 255;
                dst[2] = (src[2] * alpha + dst[2] * (255 - alpha)) / 255;
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
            memcpy(dstPixels, srcPixels, pitch);
            srcPixels += srcStride;
            dstPixels += dstStride;
        }
    }

    // The texture will need an update
    myNeedTextureUpdate = true;
}


////////////////////////////////////////////////////////////
/// Create the image from the current contents of the
/// given window
////////////////////////////////////////////////////////////
bool Image::CopyScreen(RenderWindow& window, const IntRect& sourceRect)
{
    // Adjust the source rectangle
    IntRect srcRect = sourceRect;
    if (srcRect.GetSize().x == 0 || (srcRect.GetSize().y == 0))
    {
        srcRect.Left   = 0;
        srcRect.Top    = 0;
        srcRect.Right  = window.GetWidth();
        srcRect.Bottom = window.GetHeight();
    }
    else
    {
        if (srcRect.Left   < 0) srcRect.Left = 0;
        if (srcRect.Top    < 0) srcRect.Top  = 0;
        if (srcRect.Right  > static_cast<int>(window.GetWidth()))  srcRect.Right  = window.GetWidth();
        if (srcRect.Bottom > static_cast<int>(window.GetHeight())) srcRect.Bottom = window.GetHeight();
    }

    // Store the texture dimensions
    myWidth  = srcRect.GetSize().x;
    myHeight = srcRect.GetSize().y;

    // Make sure that pending drawables are rendered on the target window
    window.Flush();

    // We can then create the texture
    if (window.SetActive() && CreateTexture())
    {
        GLint previous;
        GLCheck(glGetIntegerv(GL_TEXTURE_BINDING_2D, &previous));

        GLCheck(glBindTexture(GL_TEXTURE_2D, myTexture));
        GLCheck(glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, srcRect.Left, srcRect.Top, myWidth, myHeight));

        GLCheck(glBindTexture(GL_TEXTURE_2D, previous));

        myNeedTextureUpdate = false;
        myNeedArrayUpdate   = true;
        myPixelsFlipped     = true;

        return true;
    }
    else
    {
        Reset();
        return false;
    }
}


////////////////////////////////////////////////////////////
/// Change the color of a pixel
////////////////////////////////////////////////////////////
void Image::SetPixel(unsigned int x, unsigned int y, const Color& color)
{
    // First check if the array of pixels needs to be updated
    EnsureArrayUpdate();

    // Check if pixel is whithin the image bounds
    if ((x >= myWidth) || (y >= myHeight))
    {
        std::cerr << "Cannot set pixel (" << x << "," << y << ") for image "
                  << "(width = " << myWidth << ", height = " << myHeight << ")" << std::endl;
        return;
    }

    myPixels[x + y * myWidth] = color;

    // The texture will need to be updated
    myNeedTextureUpdate = true;
}


////////////////////////////////////////////////////////////
/// Get a pixel from the image
////////////////////////////////////////////////////////////
const Color& Image::GetPixel(unsigned int x, unsigned int y) const
{
    // First check if the array of pixels needs to be updated
    const_cast<Image*>(this)->EnsureArrayUpdate();

    // Check if pixel is whithin the image bounds
    if ((x >= myWidth) || (y >= myHeight))
    {
        std::cerr << "Cannot get pixel (" << x << "," << y << ") for image "
                  << "(width = " << myWidth << ", height = " << myHeight << ")" << std::endl;
        return Color::Black;
    }

    return myPixels[x + y * myWidth];
}


////////////////////////////////////////////////////////////
/// Get a read-only pointer to the array of pixels (RGBA 8 bits integers components)
/// Array size is GetWidth() x GetHeight() x 4
/// This pointer becomes invalid if you reload or resize the image
////////////////////////////////////////////////////////////
const Uint8* Image::GetPixelsPtr() const
{
    // First check if the array of pixels needs to be updated
    const_cast<Image*>(this)->EnsureArrayUpdate();

    if (!myPixels.empty())
    {
        return reinterpret_cast<const Uint8*>(&myPixels[0]);
    }
    else
    {
        std::cerr << "Trying to access the pixels of an empty image" << std::endl;
        return NULL;
    }
}


////////////////////////////////////////////////////////////
/// Bind the image for rendering
////////////////////////////////////////////////////////////
void Image::Bind() const
{
    // First check if the texture needs to be updated
    const_cast<Image*>(this)->EnsureTextureUpdate();

    // Bind it
    if (myTexture)
    {
        GLCheck(glEnable(GL_TEXTURE_2D));
        GLCheck(glBindTexture(GL_TEXTURE_2D, myTexture));
    }
}


////////////////////////////////////////////////////////////
/// Enable or disable image smoothing filter
////////////////////////////////////////////////////////////
void Image::SetSmooth(bool smooth)
{
    if (smooth != myIsSmooth)
    {
        myIsSmooth = smooth;

        if (myTexture)
        {
            GLint previous;
            GLCheck(glGetIntegerv(GL_TEXTURE_BINDING_2D, &previous));

            GLCheck(glBindTexture(GL_TEXTURE_2D, myTexture));
            GLCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, myIsSmooth ? GL_LINEAR : GL_NEAREST));
            GLCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, myIsSmooth ? GL_LINEAR : GL_NEAREST));

            GLCheck(glBindTexture(GL_TEXTURE_2D, previous));
        }
    }
}


////////////////////////////////////////////////////////////
/// Return the width of the image
////////////////////////////////////////////////////////////
unsigned int Image::GetWidth() const
{
    return myWidth;
}


////////////////////////////////////////////////////////////
/// Return the height of the image
////////////////////////////////////////////////////////////
unsigned int Image::GetHeight() const
{
    return myHeight;
}


////////////////////////////////////////////////////////////
/// Tells whether the smooth filtering is enabled or not
////////////////////////////////////////////////////////////
bool Image::IsSmooth() const
{
    return myIsSmooth;
}


////////////////////////////////////////////////////////////
/// Convert a subrect expressed in pixels, into float
/// texture coordinates
////////////////////////////////////////////////////////////
FloatRect Image::GetTexCoords(const IntRect& rect) const
{
    float width  = static_cast<float>(myTextureWidth);
    float height = static_cast<float>(myTextureHeight);

    if (myPixelsFlipped)
    {
        return FloatRect(rect.Left   / width,
                         rect.Bottom / height,
                         rect.Right  / width,
                         rect.Top    / height);
    }
    else
    {
        return FloatRect(rect.Left   / width,
                         rect.Top    / height,
                         rect.Right  / width,
                         rect.Bottom / height);
    }
}


////////////////////////////////////////////////////////////
/// Get the maximum image size according to hardware support
////////////////////////////////////////////////////////////
unsigned int Image::GetMaximumSize()
{
    GLint size;
    GLCheck(glGetIntegerv(GL_MAX_TEXTURE_SIZE, &size));

    return static_cast<unsigned int>(size);
}


////////////////////////////////////////////////////////////
/// Get a valid image size according to hardware support
////////////////////////////////////////////////////////////
unsigned int Image::GetValidSize(unsigned int size)
{
    // Make sure that GLEW is initialized
    priv::EnsureGlewInit();

    if (GLEW_ARB_texture_non_power_of_two)
    {
        // If hardware supports NPOT textures, then just return the unmodified size
        return size;
    }
    else
    {
        // If hardware doesn't support NPOT textures, we calculate the nearest power of two
        unsigned int powerOfTwo = 1;
        while (powerOfTwo < size)
            powerOfTwo *= 2;

        return powerOfTwo;
    }
}


////////////////////////////////////////////////////////////
/// Assignment operator
////////////////////////////////////////////////////////////
Image& Image::operator =(const Image& other)
{
    Image temp(other);

    std::swap(myWidth,             temp.myWidth);
    std::swap(myHeight,            temp.myHeight);
    std::swap(myTextureWidth,      temp.myTextureWidth);
    std::swap(myTextureHeight,     temp.myTextureHeight);
    std::swap(myTexture,           temp.myTexture);
    std::swap(myIsSmooth,          temp.myIsSmooth);
    std::swap(myNeedArrayUpdate,   temp.myNeedArrayUpdate);
    std::swap(myNeedTextureUpdate, temp.myNeedTextureUpdate);
    std::swap(myPixelsFlipped,     temp.myPixelsFlipped);
    myPixels.swap(temp.myPixels);

    return *this;
}


////////////////////////////////////////////////////////////
/// Create the OpenGL texture
////////////////////////////////////////////////////////////
bool Image::CreateTexture()
{
    // Check if texture parameters are valid before creating it
    if (!myWidth || !myHeight)
        return false;

    // Adjust internal texture dimensions depending on NPOT textures support
    unsigned int textureWidth  = GetValidSize(myWidth);
    unsigned int textureHeight = GetValidSize(myHeight);

    // Check the maximum texture size
    unsigned int maxSize = GetMaximumSize();
    if ((textureWidth > maxSize) || (textureHeight > maxSize))
    {
        std::cerr << "Failed to create image, its internal size is too high "
                  << "(" << textureWidth << "x" << textureHeight << ", "
                  << "maximum is " << maxSize << "x" << maxSize << ")"
                  << std::endl;
        return false;
    }

    // Destroy the previous OpenGL texture if it already exists with another size
    if ((textureWidth != myTextureWidth) || (textureHeight != myTextureHeight))
    {
        DestroyTexture();
        myTextureWidth  = textureWidth;
        myTextureHeight = textureHeight;
    }

    // Create the OpenGL texture
    if (!myTexture)
    {
        GLint previous;
        GLCheck(glGetIntegerv(GL_TEXTURE_BINDING_2D, &previous));

        GLuint texture = 0;
        GLCheck(glGenTextures(1, &texture));
        GLCheck(glBindTexture(GL_TEXTURE_2D, texture));
        GLCheck(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, myTextureWidth, myTextureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL));
        GLCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP));
        GLCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP));
        GLCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, myIsSmooth ? GL_LINEAR : GL_NEAREST));
        GLCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, myIsSmooth ? GL_LINEAR : GL_NEAREST));
        myTexture = static_cast<unsigned int>(texture);

        GLCheck(glBindTexture(GL_TEXTURE_2D, previous));
    }

    myNeedTextureUpdate = true;

    return true;
}


////////////////////////////////////////////////////////////
/// Make sure the texture in video memory is updated with the
/// array of pixels
////////////////////////////////////////////////////////////
void Image::EnsureTextureUpdate()
{
    if (myNeedTextureUpdate)
    {
        if (myTexture && !myPixels.empty())
        {
            GLint previous;
            GLCheck(glGetIntegerv(GL_TEXTURE_BINDING_2D, &previous));

            // Update the texture with the pixels array in RAM
            GLCheck(glBindTexture(GL_TEXTURE_2D, myTexture));
            GLCheck(glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, myWidth, myHeight, GL_RGBA, GL_UNSIGNED_BYTE, &myPixels[0]));
            myPixelsFlipped = false;

            GLCheck(glBindTexture(GL_TEXTURE_2D, previous));
        }

        myNeedTextureUpdate = false;
    }
}


////////////////////////////////////////////////////////////
/// Make sure the array of pixels is updated with the
/// texture in video memory
////////////////////////////////////////////////////////////
void Image::EnsureArrayUpdate()
{
    if (myNeedArrayUpdate)
    {
        // First make sure the texture is up-to-date
        // (may not be the case if an external update has been scheduled)
        EnsureTextureUpdate();

        // Save the previous texture
        GLint previous;
        GLCheck(glGetIntegerv(GL_TEXTURE_BINDING_2D, &previous));

        // Resize the destination array of pixels
        myPixels.resize(myWidth * myHeight);

        if ((myWidth == myTextureWidth) && (myHeight == myTextureHeight) && !myPixelsFlipped)
        {
            // Texture and array have the same size, we can use a direct copy

            // Copy pixels from texture to array
            GLCheck(glBindTexture(GL_TEXTURE_2D, myTexture));
            GLCheck(glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, &myPixels[0]));
        }
        else
        {
            // Texture and array don't have the same size, we have to use a slower algorithm

            // All the pixels will first be copied to a temporary array
            std::vector<Color> allPixels(myTextureWidth * myTextureHeight);
            GLCheck(glBindTexture(GL_TEXTURE_2D, myTexture));
            GLCheck(glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, &allPixels[0]));

            // Then we copy the useful pixels from the temporary array to the final one
            const Color* src = &allPixels[0];
            Color* dst = &myPixels[0];
            int srcPitch = myTextureWidth;

            // Handle the case where source pixels are flipped vertically
            if (myPixelsFlipped)
            {
                src += myTextureWidth * (myHeight - 1);
                srcPitch = -srcPitch;
            }

            for (unsigned int i = 0; i < myHeight; ++i)
            {
                std::copy(src, src + myWidth, dst);
                src += srcPitch;
                dst += myWidth;
            }
        }

        // Restore the previous texture
        GLCheck(glBindTexture(GL_TEXTURE_2D, previous));

        myNeedArrayUpdate = false;
    }
}


////////////////////////////////////////////////////////////
/// Reset the image attributes
////////////////////////////////////////////////////////////
void Image::Reset()
{
    DestroyTexture();

    myWidth             = 0;
    myHeight            = 0;
    myTextureWidth      = 0;
    myTextureHeight     = 0;
    myTexture           = 0;
    myIsSmooth          = true;
    myNeedTextureUpdate = false;
    myNeedArrayUpdate   = false;
    myPixelsFlipped     = false;
    myPixels.clear();
}


////////////////////////////////////////////////////////////
/// Destroy the OpenGL texture
////////////////////////////////////////////////////////////
void Image::DestroyTexture()
{
    // Destroy the internal texture
    if (myTexture)
    {
        GLuint Texture = static_cast<GLuint>(myTexture);
        GLCheck(glDeleteTextures(1, &Texture));
        myTexture           = 0;
        myNeedTextureUpdate = false;
        myNeedArrayUpdate   = false;
    }
}

} // namespace sf
