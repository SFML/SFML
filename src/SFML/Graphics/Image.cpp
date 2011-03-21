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
#include <SFML/System/Err.hpp>
#include <algorithm>
#include <vector>
#include <cstring>


namespace sf
{
////////////////////////////////////////////////////////////
Image::Image() :
myWidth         (0),
myHeight        (0),
myTextureWidth  (0),
myTextureHeight (0),
myTexture       (0),
myIsSmooth      (true),
myTextureUpdated(true),
myArrayUpdated  (true),
myPixelsFlipped (false)
{

}


////////////////////////////////////////////////////////////
Image::Image(const Image& copy) :
Resource<Image>()
{
    // First make sure that the source image is up-to-date
    copy.EnsureArrayUpdate();

    // Copy all its members
    myWidth          = copy.myWidth;
    myHeight         = copy.myHeight;
    myTextureWidth   = copy.myTextureWidth;
    myTextureHeight  = copy.myTextureHeight;
    myTexture        = 0;
    myIsSmooth       = copy.myIsSmooth;
    myPixels         = copy.myPixels;
    myTextureUpdated = true;
    myArrayUpdated   = true;
    myPixelsFlipped  = false; // pixels can't be flipped, this is handled in copy.EnsureArrayUpdate()

    // Create the texture
    CreateTexture(myWidth, myHeight);
}


////////////////////////////////////////////////////////////
Image::~Image()
{
    // Destroy the OpenGL texture
    if (myTexture)
    {
        EnsureGlContext();

        GLuint Texture = static_cast<GLuint>(myTexture);
        GLCheck(glDeleteTextures(1, &Texture));
    }
}


////////////////////////////////////////////////////////////
bool Image::LoadFromFile(const std::string& filename)
{
    // Forward the job to the image loader
    std::vector<Uint8> pixels;
    unsigned int width;
    unsigned int height;
    if (priv::ImageLoader::GetInstance().LoadImageFromFile(filename, pixels, width, height))
    {
        // Loading succeeded : we can create the texture
        if (CreateTexture(width, height))
        {
            // Copy the pixels
            myPixels.swap(pixels);
            return true;
        }
    }

    return false;
}


////////////////////////////////////////////////////////////
bool Image::LoadFromMemory(const void* data, std::size_t size)
{
    // Forward the job to the image loader
    std::vector<Uint8> pixels;
    unsigned int width;
    unsigned int height;
    if (priv::ImageLoader::GetInstance().LoadImageFromMemory(data, size, pixels, width, height))
    {
        // Loading succeeded : we can create the texture
        if (CreateTexture(width, height))
        {
            // Copy the pixels
            myPixels.swap(pixels);
            return true;
        }
    }

    return false;
}


////////////////////////////////////////////////////////////
bool Image::LoadFromPixels(unsigned int width, unsigned int height, const Uint8* data)
{
    if (data)
    {
        // Create the internal texture
        if (CreateTexture(width, height))
        {
            // Copy the pixels
            std::size_t size = width * height * 4;
            myPixels.resize(size);
            std::memcpy(&myPixels[0], data, size); // faster than vector::assign
            return true;
        }
        else
        {
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
bool Image::SaveToFile(const std::string& filename) const
{
    // Check if the array of pixels needs to be updated
    EnsureArrayUpdate();

    // Let the image loader save our pixel array into the image
    return priv::ImageLoader::GetInstance().SaveImageToFile(filename, myPixels, myWidth, myHeight);
}


////////////////////////////////////////////////////////////
bool Image::Create(unsigned int width, unsigned int height, const Color& color)
{
    // First create the texture
    if (CreateTexture(width, height))
    {
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

        return true;
    }
    else
    {
        return false;
    }
}


////////////////////////////////////////////////////////////
void Image::CreateMaskFromColor(const Color& color, Uint8 alpha)
{
    // Check if the array of pixels needs to be updated
    EnsureArrayUpdate();

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

        // The texture will need to be updated
        myTextureUpdated = false;
    }
}


////////////////////////////////////////////////////////////
void Image::Copy(const Image& source, unsigned int destX, unsigned int destY, const IntRect& sourceRect, bool applyAlpha)
{
    // Make sure that both images are valid
    if ((source.myWidth == 0) || (source.myHeight == 0) || (myWidth == 0) || (myHeight == 0))
        return;

    // Make sure both images have up-to-date arrays
    EnsureArrayUpdate();
    source.EnsureArrayUpdate();

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

    // The texture will need an update
    myTextureUpdated = false;
}


////////////////////////////////////////////////////////////
bool Image::CopyScreen(RenderWindow& window, const IntRect& sourceRect)
{
    // Adjust the source rectangle
    IntRect srcRect = sourceRect;
    if (srcRect.Width == 0 || (srcRect.Height == 0))
    {
        srcRect.Left   = 0;
        srcRect.Top    = 0;
        srcRect.Width  = window.GetWidth();
        srcRect.Height = window.GetHeight();
    }
    else
    {
        if (srcRect.Left   < 0) srcRect.Left = 0;
        if (srcRect.Top    < 0) srcRect.Top  = 0;
        if (srcRect.Width  > static_cast<int>(window.GetWidth()))  srcRect.Width  = window.GetWidth();
        if (srcRect.Height > static_cast<int>(window.GetHeight())) srcRect.Height = window.GetHeight();
    }

    // We can then create the texture
    if (window.SetActive() && CreateTexture(srcRect.Width, srcRect.Height))
    {
        GLint previous;
        GLCheck(glGetIntegerv(GL_TEXTURE_BINDING_2D, &previous));

        GLCheck(glBindTexture(GL_TEXTURE_2D, myTexture));
        GLCheck(glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, srcRect.Left, srcRect.Top, myWidth, myHeight));

        GLCheck(glBindTexture(GL_TEXTURE_2D, previous));

        myTextureUpdated = true;
        myArrayUpdated   = false;
        myPixelsFlipped  = true;

        return true;
    }
    else
    {
        return false;
    }
}


////////////////////////////////////////////////////////////
void Image::SetPixel(unsigned int x, unsigned int y, const Color& color)
{
    // First check if the array of pixels needs to be updated
    EnsureArrayUpdate();

    // Copy the color components
    Uint8* pixel = &myPixels[(x + y * myWidth) * 4];
    *pixel++ = color.r;
    *pixel++ = color.g;
    *pixel++ = color.b;
    *pixel++ = color.a;

    // The texture will need to be updated
    myTextureUpdated = false;
}


////////////////////////////////////////////////////////////
Color Image::GetPixel(unsigned int x, unsigned int y) const
{
    // First check if the array of pixels needs to be updated
    EnsureArrayUpdate();

    // Get the color at (x, y)
    const Uint8* pixel = &myPixels[(x + y * myWidth) * 4];

    return Color(pixel[0], pixel[1], pixel[2], pixel[3]);
}


////////////////////////////////////////////////////////////
const Uint8* Image::GetPixelsPtr() const
{
    // First check if the array of pixels needs to be updated
    EnsureArrayUpdate();

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
void Image::UpdatePixels(const Uint8* pixels)
{
    if (pixels && myTexture)
    {
        EnsureGlContext();

        GLint previous;
        GLCheck(glGetIntegerv(GL_TEXTURE_BINDING_2D, &previous));

        // Update the texture from the array of pixels
        GLCheck(glBindTexture(GL_TEXTURE_2D, myTexture));
        GLCheck(glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, myWidth, myHeight, GL_RGBA, GL_UNSIGNED_BYTE, pixels));

        GLCheck(glBindTexture(GL_TEXTURE_2D, previous));

        myArrayUpdated   = false;
        myTextureUpdated = true;
    }
}


////////////////////////////////////////////////////////////
void Image::UpdatePixels(const Uint8* pixels, const IntRect& rectangle)
{
    // Make sure that the texture is up-to-date
    EnsureTextureUpdate();

    if (pixels && myTexture)
    {
        GLint previous;
        GLCheck(glGetIntegerv(GL_TEXTURE_BINDING_2D, &previous));

        // Update the texture from the array of pixels
        GLCheck(glBindTexture(GL_TEXTURE_2D, myTexture));
        GLCheck(glTexSubImage2D(GL_TEXTURE_2D, 0, rectangle.Left, rectangle.Top, rectangle.Width, rectangle.Height, GL_RGBA, GL_UNSIGNED_BYTE, pixels));

        GLCheck(glBindTexture(GL_TEXTURE_2D, previous));

        // The pixel cache is no longer up-to-date
        myArrayUpdated = false;
    }
}


////////////////////////////////////////////////////////////
void Image::Bind() const
{
    // First check if the texture needs to be updated
    EnsureTextureUpdate();

    // Bind it
    GLCheck(glBindTexture(GL_TEXTURE_2D, myTexture));
}


////////////////////////////////////////////////////////////
void Image::SetSmooth(bool smooth)
{
    if (smooth != myIsSmooth)
    {
        myIsSmooth = smooth;

        if (myTexture)
        {
            EnsureGlContext();

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
bool Image::IsSmooth() const
{
    return myIsSmooth;
}


////////////////////////////////////////////////////////////
FloatRect Image::GetTexCoords(const IntRect& rect) const
{
    if ((myTextureWidth != 0) && (myTextureHeight != 0))
    {
        float width  = static_cast<float>(myTextureWidth);
        float height = static_cast<float>(myTextureHeight);

        if (myPixelsFlipped)
        {
            return FloatRect( rect.Left            / width,
                             (myHeight - rect.Top) / height,
                              rect.Width           / width,
                             -rect.Height          / height);
        }
        else
        {
            return FloatRect(rect.Left   / width,
                             rect.Top    / height,
                             rect.Width  / width,
                             rect.Height / height);
        }
    }
    else
    {
        return FloatRect(0, 0, 0, 0);
    }
}


////////////////////////////////////////////////////////////
unsigned int Image::GetMaximumSize()
{
    EnsureGlContext();

    GLint size;
    GLCheck(glGetIntegerv(GL_MAX_TEXTURE_SIZE, &size));

    return static_cast<unsigned int>(size);
}


////////////////////////////////////////////////////////////
Image& Image::operator =(const Image& right)
{
    Image temp(right);

    std::swap(myWidth,          temp.myWidth);
    std::swap(myHeight,         temp.myHeight);
    std::swap(myTextureWidth,   temp.myTextureWidth);
    std::swap(myTextureHeight,  temp.myTextureHeight);
    std::swap(myTexture,        temp.myTexture);
    std::swap(myIsSmooth,       temp.myIsSmooth);
    std::swap(myArrayUpdated,   temp.myArrayUpdated);
    std::swap(myTextureUpdated, temp.myTextureUpdated);
    std::swap(myPixelsFlipped,  temp.myPixelsFlipped);
    std::swap(myPixels,         temp.myPixels);

    return *this;
}


////////////////////////////////////////////////////////////
unsigned int Image::GetValidSize(unsigned int size)
{
    EnsureGlContext();

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
bool Image::CreateTexture(unsigned int width, unsigned int height)
{
    // Check if texture parameters are valid before creating it
    if (!width || !height)
        return false;

    // Compute the internal texture dimensions depending on NPOT textures support
    unsigned int textureWidth  = GetValidSize(width);
    unsigned int textureHeight = GetValidSize(height);

    // Check the maximum texture size
    unsigned int maxSize = GetMaximumSize();
    if ((textureWidth > maxSize) || (textureHeight > maxSize))
    {
        Err() << "Failed to create image, its internal size is too high "
              << "(" << textureWidth << "x" << textureHeight << ", "
              << "maximum is " << maxSize << "x" << maxSize << ")"
              << std::endl;
        return false;
    }

    // All the validity checks passed, we can store the new texture settings
    myWidth         = width;
    myHeight        = height;
    myTextureWidth  = textureWidth;
    myTextureHeight = textureHeight;

    EnsureGlContext();

    // Create the OpenGL texture if it doesn't exist yet
    if (!myTexture)
    {
        GLuint texture;
        GLCheck(glGenTextures(1, &texture));
        myTexture = static_cast<unsigned int>(texture);
    }

    // Initialize the texture
    GLint previous;
    GLCheck(glGetIntegerv(GL_TEXTURE_BINDING_2D, &previous));
    GLCheck(glBindTexture(GL_TEXTURE_2D, myTexture));
    GLCheck(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, myTextureWidth, myTextureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL));
    GLCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    GLCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, myIsSmooth ? GL_LINEAR : GL_NEAREST));
    GLCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, myIsSmooth ? GL_LINEAR : GL_NEAREST));
    GLCheck(glBindTexture(GL_TEXTURE_2D, previous));

    myTextureUpdated = false;

    return true;
}


////////////////////////////////////////////////////////////
void Image::EnsureTextureUpdate() const
{
    if (!myTextureUpdated)
    {
        if (myTexture && !myPixels.empty())
        {
            EnsureGlContext();

            GLint previous;
            GLCheck(glGetIntegerv(GL_TEXTURE_BINDING_2D, &previous));

            // Update the texture with the pixels array in RAM
            GLCheck(glBindTexture(GL_TEXTURE_2D, myTexture));
            GLCheck(glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, myWidth, myHeight, GL_RGBA, GL_UNSIGNED_BYTE, &myPixels[0]));
            myPixelsFlipped = false;

            GLCheck(glBindTexture(GL_TEXTURE_2D, previous));
        }

        myTextureUpdated = true;
    }
}


////////////////////////////////////////////////////////////
void Image::EnsureArrayUpdate() const
{
    if (!myArrayUpdated)
    {
        EnsureGlContext();

        // Save the previous texture
        GLint previous;
        GLCheck(glGetIntegerv(GL_TEXTURE_BINDING_2D, &previous));

        // Resize the destination array of pixels
        myPixels.resize(myWidth * myHeight * 4);

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
            ColorArray allPixels(myTextureWidth * myTextureHeight * 4);
            GLCheck(glBindTexture(GL_TEXTURE_2D, myTexture));
            GLCheck(glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, &allPixels[0]));

            // Then we copy the useful pixels from the temporary array to the final one
            const Uint8* src = &allPixels[0];
            Uint8* dst = &myPixels[0];
            int srcPitch = myTextureWidth * 4;
            int dstPitch = myWidth * 4;

            // Handle the case where source pixels are flipped vertically
            if (myPixelsFlipped)
            {
                src += srcPitch * (myHeight - 1);
                srcPitch = -srcPitch;
            }

            for (unsigned int i = 0; i < myHeight; ++i)
            {
                std::memcpy(dst, src, dstPitch);
                src += srcPitch;
                dst += dstPitch;
            }
        }

        // Restore the previous texture
        GLCheck(glBindTexture(GL_TEXTURE_2D, previous));

        myArrayUpdated = true;
    }
}


////////////////////////////////////////////////////////////
void Image::Use() const
{
    EnsureTextureUpdate();
}

} // namespace sf
