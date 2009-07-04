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
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/GraphicsContext.hpp>
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
myNeedArrayUpdate  (false)
{

}


////////////////////////////////////////////////////////////
/// Copy constructor
////////////////////////////////////////////////////////////
Image::Image(const Image& Copy) :
Resource<Image>    (Copy),
myWidth            (Copy.myWidth),
myHeight           (Copy.myHeight),
myTextureWidth     (Copy.myTextureWidth),
myTextureHeight    (Copy.myTextureHeight),
myTexture          (0),
myIsSmooth         (Copy.myIsSmooth),
myPixels           (Copy.myPixels),
myNeedTextureUpdate(false),
myNeedArrayUpdate  (false)
{
    CreateTexture();
}


////////////////////////////////////////////////////////////
/// Construct an empty image
////////////////////////////////////////////////////////////
Image::Image(unsigned int Width, unsigned int Height, const Color& Col) :
myWidth            (0),
myHeight           (0),
myTextureWidth     (0),
myTextureHeight    (0),
myTexture          (0),
myIsSmooth         (true),
myNeedTextureUpdate(false),
myNeedArrayUpdate  (false)
{
    Create(Width, Height, Col);
}


////////////////////////////////////////////////////////////
/// Construct the image from pixels in memory
////////////////////////////////////////////////////////////
Image::Image(unsigned int Width, unsigned int Height, const Uint8* Data) :
myWidth            (0),
myHeight           (0),
myTextureWidth     (0),
myTextureHeight    (0),
myTexture          (0),
myIsSmooth         (true),
myNeedTextureUpdate(false),
myNeedArrayUpdate  (false)
{
    LoadFromPixels(Width, Height, Data);
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
bool Image::LoadFromFile(const std::string& Filename)
{
    // Let the image loader load the image into our pixel array
    bool Success = priv::ImageLoader::GetInstance().LoadImageFromFile(Filename, myPixels, myWidth, myHeight);

    if (Success)
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
bool Image::LoadFromMemory(const char* Data, std::size_t SizeInBytes)
{
    // Check parameters
    if (!Data || (SizeInBytes == 0))
    {
        std::cerr << "Failed to image font from memory, no data provided" << std::endl;
        return false;
    }

    // Let the image loader load the image into our pixel array
    bool Success = priv::ImageLoader::GetInstance().LoadImageFromMemory(Data, SizeInBytes, myPixels, myWidth, myHeight);

    if (Success)
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
bool Image::LoadFromPixels(unsigned int Width, unsigned int Height, const Uint8* Data)
{
    if (Data)
    {
        // Store the texture dimensions
        myWidth  = Width;
        myHeight = Height;

        // Fill the pixel buffer with the specified raw data
        const Color* Ptr = reinterpret_cast<const Color*>(Data);
        myPixels.assign(Ptr, Ptr + Width * Height);

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
        return Create(Width, Height, Color(255, 255, 255, 255));
    }
}


////////////////////////////////////////////////////////////
/// Save the content of the image to a file
////////////////////////////////////////////////////////////
bool Image::SaveToFile(const std::string& Filename) const
{
    // Check if the array of pixels needs to be updated
    EnsureArrayUpdate();

    // Let the image loader save our pixel array into the image
    return priv::ImageLoader::GetInstance().SaveImageToFile(Filename, myPixels, myWidth, myHeight);
}


////////////////////////////////////////////////////////////
/// Create an empty image
////////////////////////////////////////////////////////////
bool Image::Create(unsigned int Width, unsigned int Height, Color Col)
{
    // Store the texture dimensions
    myWidth  = Width;
    myHeight = Height;

    // Recreate the pixel buffer and fill it with the specified color
    myPixels.clear();
    myPixels.resize(Width * Height, Col);

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
void Image::CreateMaskFromColor(Color ColorKey, Uint8 Alpha)
{
    // Check if the array of pixels needs to be updated
    EnsureArrayUpdate();

    // Calculate the new color (old color with no alpha)
    Color NewColor(ColorKey.r, ColorKey.g, ColorKey.b, Alpha);

    // Replace the old color by the new one
    std::replace(myPixels.begin(), myPixels.end(), ColorKey, NewColor);

    // The texture will need to be updated
    myNeedTextureUpdate = true;
}


////////////////////////////////////////////////////////////
/// Copy pixels from another image onto this one.
/// This function does a slow pixel copy and should only
/// be used at initialization time
////////////////////////////////////////////////////////////
void Image::Copy(const Image& Source, unsigned int DestX, unsigned int DestY, const IntRect& SourceRect, bool ApplyAlpha)
{
    // Make sure both images are valid
    if ((Source.myWidth == 0) || (Source.myHeight == 0) || (myWidth == 0) || (myHeight == 0))
        return;

    // Make sure both images have up-to-date arrays
    EnsureArrayUpdate();
    Source.EnsureArrayUpdate();

    // Adjust the source rectangle
    IntRect SrcRect = SourceRect;
    if (SrcRect.GetWidth() == 0 || (SrcRect.GetHeight() == 0))
    {
        SrcRect.Left   = 0;
        SrcRect.Top    = 0;
        SrcRect.Right  = Source.myWidth;
        SrcRect.Bottom = Source.myHeight;
    }
    else
    {
        if (SrcRect.Left   < 0) SrcRect.Left = 0;
        if (SrcRect.Top    < 0) SrcRect.Top  = 0;
        if (SrcRect.Right  > static_cast<int>(Source.myWidth))  SrcRect.Right  = Source.myWidth;
        if (SrcRect.Bottom > static_cast<int>(Source.myHeight)) SrcRect.Bottom = Source.myHeight;
    }

    // Then find the valid bounds of the destination rectangle
    int Width  = SrcRect.GetWidth();
    int Height = SrcRect.GetHeight();
    if (DestX + Width  > myWidth)  Width  = myWidth  - DestX;
    if (DestY + Height > myHeight) Height = myHeight - DestY;

    // Make sure the destination area is valid
    if ((Width <= 0) || (Height <= 0))
        return;

    // Precompute as much as possible
    int          Pitch     = Width * 4;
    int          Rows      = Height;
    int          SrcStride = Source.myWidth * 4;
    int          DstStride = myWidth * 4;
    const Uint8* SrcPixels = Source.GetPixelsPtr() + (SrcRect.Left + SrcRect.Top * Source.myWidth) * 4;
    Uint8*       DstPixels = reinterpret_cast<Uint8*>(&myPixels[0]) + (DestX + DestY * myWidth) * 4;

    // Copy the pixels
    if (ApplyAlpha)
    {
        // Interpolation using alpha values, pixel by pixel (slower)
        for (int i = 0; i < Rows; ++i)
        {
            for (int j = 0; j < Width; ++j)
            {
                // Get a direct pointer to the components of the current pixel
                const Uint8* Src   = SrcPixels + j * 4;
                Uint8*       Dst   = DstPixels + j * 4;

                // Interpolate RGB components using the alpha value of the source pixel
                Uint8 Alpha = Src[3];
                Dst[0] = (Src[0] * Alpha + Dst[0] * (255 - Alpha)) / 255;
                Dst[1] = (Src[1] * Alpha + Dst[1] * (255 - Alpha)) / 255;
                Dst[2] = (Src[2] * Alpha + Dst[2] * (255 - Alpha)) / 255;
            }

            SrcPixels += SrcStride;
            DstPixels += DstStride;
        }
    }
    else
    {
        // Optimized copy ignoring alpha values, row by row (faster)
        for (int i = 0; i < Rows; ++i)
        {
            memcpy(DstPixels, SrcPixels, Pitch);
            SrcPixels += SrcStride;
            DstPixels += DstStride;
        }
    }

    // The texture will need an update
    myNeedTextureUpdate = true;
}


////////////////////////////////////////////////////////////
/// Create the image from the current contents of the
/// given window
////////////////////////////////////////////////////////////
bool Image::CopyScreen(RenderWindow& Window, const IntRect& SourceRect)
{
    // Adjust the source rectangle
    IntRect SrcRect = SourceRect;
    if (SrcRect.GetWidth() == 0 || (SrcRect.GetHeight() == 0))
    {
        SrcRect.Left   = 0;
        SrcRect.Top    = 0;
        SrcRect.Right  = Window.GetWidth();
        SrcRect.Bottom = Window.GetHeight();
    }
    else
    {
        if (SrcRect.Left   < 0) SrcRect.Left = 0;
        if (SrcRect.Top    < 0) SrcRect.Top  = 0;
        if (SrcRect.Right  > static_cast<int>(Window.GetWidth()))  SrcRect.Right  = Window.GetWidth();
        if (SrcRect.Bottom > static_cast<int>(Window.GetHeight())) SrcRect.Bottom = Window.GetHeight();
    }

    // Store the texture dimensions
    myWidth  = SrcRect.GetWidth();
    myHeight = SrcRect.GetHeight();

    // We can then create the texture
    if (Window.SetActive() && CreateTexture())
    {
        GLint PreviousTexture;
        GLCheck(glGetIntegerv(GL_TEXTURE_BINDING_2D, &PreviousTexture));

        GLCheck(glBindTexture(GL_TEXTURE_2D, myTexture));
        GLCheck(glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, SrcRect.Left, SrcRect.Top, myWidth, myHeight));

        GLCheck(glBindTexture(GL_TEXTURE_2D, PreviousTexture));

        myNeedTextureUpdate = false;
        myNeedArrayUpdate = true;

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
void Image::SetPixel(unsigned int X, unsigned int Y, const Color& Col)
{
    // First check if the array of pixels needs to be updated
    EnsureArrayUpdate();

    // Check if pixel is whithin the image bounds
    if ((X >= myWidth) || (Y >= myHeight))
    {
        std::cerr << "Cannot set pixel (" << X << "," << Y << ") for image "
                  << "(width = " << myWidth << ", height = " << myHeight << ")" << std::endl;
        return;
    }

    myPixels[X + Y * myWidth] = Col;

    // The texture will need to be updated
    myNeedTextureUpdate = true;
}


////////////////////////////////////////////////////////////
/// Get a pixel from the image
////////////////////////////////////////////////////////////
const Color& Image::GetPixel(unsigned int X, unsigned int Y) const
{
    // First check if the array of pixels needs to be updated
    EnsureArrayUpdate();

    // Check if pixel is whithin the image bounds
    if ((X >= myWidth) || (Y >= myHeight))
    {
        std::cerr << "Cannot get pixel (" << X << "," << Y << ") for image "
                  << "(width = " << myWidth << ", height = " << myHeight << ")" << std::endl;
        return Color::Black;
    }

    return myPixels[X + Y * myWidth];
}


////////////////////////////////////////////////////////////
/// Get a read-only pointer to the array of pixels (RGBA 8 bits integers components)
/// Array size is GetWidth() x GetHeight() x 4
/// This pointer becomes invalid if you reload or resize the image
////////////////////////////////////////////////////////////
const Uint8* Image::GetPixelsPtr() const
{
    // First check if the array of pixels needs to be updated
    EnsureArrayUpdate();

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
    EnsureTextureUpdate();

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
void Image::SetSmooth(bool Smooth)
{
    if (Smooth != myIsSmooth)
    {
        myIsSmooth = Smooth;

        if (myTexture)
        {
            // Make sure we have a valid context
            priv::GraphicsContext Ctx;

            GLint PreviousTexture;
            GLCheck(glGetIntegerv(GL_TEXTURE_BINDING_2D, &PreviousTexture));

            GLCheck(glBindTexture(GL_TEXTURE_2D, myTexture));
            GLCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, myIsSmooth ? GL_LINEAR : GL_NEAREST));
            GLCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, myIsSmooth ? GL_LINEAR : GL_NEAREST));

            GLCheck(glBindTexture(GL_TEXTURE_2D, PreviousTexture));
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
FloatRect Image::GetTexCoords(const IntRect& Rect) const
{
    float Width  = static_cast<float>(myTextureWidth);
    float Height = static_cast<float>(myTextureHeight);

    return FloatRect(Rect.Left   / Width,
                     Rect.Top    / Height,
                     Rect.Right  / Width,
                     Rect.Bottom / Height);
}


////////////////////////////////////////////////////////////
/// Get a valid texture size according to hardware support
////////////////////////////////////////////////////////////
unsigned int Image::GetValidTextureSize(unsigned int Size)
{
    // Make sure we have a valid context
    priv::GraphicsContext Ctx;

    if (glewIsSupported("GL_ARB_texture_non_power_of_two") != 0)
    {
        // If hardware supports NPOT textures, then just return the unmodified size
        return Size;
    }
    else
    {
        // If hardware doesn't support NPOT textures, we calculate the nearest power of two
        unsigned int PowerOfTwo = 1;
        while (PowerOfTwo < Size)
            PowerOfTwo *= 2;

        return PowerOfTwo;
    }
}


////////////////////////////////////////////////////////////
/// Assignment operator
////////////////////////////////////////////////////////////
Image& Image::operator =(const Image& Other)
{
    Image Temp(Other);

    std::swap(myWidth,             Temp.myWidth);
    std::swap(myHeight,            Temp.myHeight);
    std::swap(myTextureWidth,      Temp.myTextureWidth);
    std::swap(myTextureHeight,     Temp.myTextureHeight);
    std::swap(myTexture,           Temp.myTexture);
    std::swap(myIsSmooth,          Temp.myIsSmooth);
    std::swap(myNeedArrayUpdate,   Temp.myNeedArrayUpdate);
    std::swap(myNeedTextureUpdate, Temp.myNeedTextureUpdate);
    myPixels.swap(Temp.myPixels);

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

    // Make sure we have a valid context
    priv::GraphicsContext Ctx;

    // Adjust internal texture dimensions depending on NPOT textures support
    unsigned int TextureWidth  = GetValidTextureSize(myWidth);
    unsigned int TextureHeight = GetValidTextureSize(myHeight);

    // Check the maximum texture size
    GLint MaxSize;
    GLCheck(glGetIntegerv(GL_MAX_TEXTURE_SIZE, &MaxSize));
    if ((TextureWidth > static_cast<unsigned int>(MaxSize)) || (TextureHeight > static_cast<unsigned int>(MaxSize)))
    {
        std::cerr << "Failed to create image, its internal size is too high (" << TextureWidth << "x" << TextureHeight << ")" << std::endl;
        return false;
    }

    // Destroy the previous OpenGL texture if it already exists with another size
    if ((TextureWidth != myTextureWidth) || (TextureHeight != myTextureHeight))
    {
        DestroyTexture();
        myTextureWidth  = TextureWidth;
        myTextureHeight = TextureHeight;
    }

    // Create the OpenGL texture
    if (!myTexture)
    {
        GLint PreviousTexture;
        GLCheck(glGetIntegerv(GL_TEXTURE_BINDING_2D, &PreviousTexture));

        GLuint Texture = 0;
        GLCheck(glGenTextures(1, &Texture));
        GLCheck(glBindTexture(GL_TEXTURE_2D, Texture));
        GLCheck(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, myTextureWidth, myTextureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL));
        GLCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP));
        GLCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP));
        GLCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, myIsSmooth ? GL_LINEAR : GL_NEAREST));
        GLCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, myIsSmooth ? GL_LINEAR : GL_NEAREST));
        myTexture = static_cast<unsigned int>(Texture);

        GLCheck(glBindTexture(GL_TEXTURE_2D, PreviousTexture));
    }

    myNeedTextureUpdate = true;

    return true;
}


////////////////////////////////////////////////////////////
/// Make sure the texture in video memory is updated with the
/// array of pixels
////////////////////////////////////////////////////////////
void Image::EnsureTextureUpdate() const
{
    if (myNeedTextureUpdate)
    {
        // Copy the pixels
        if (myTexture && !myPixels.empty())
        {
            GLint PreviousTexture;
            GLCheck(glGetIntegerv(GL_TEXTURE_BINDING_2D, &PreviousTexture));

            // Update the texture with the pixels array in RAM
            GLCheck(glBindTexture(GL_TEXTURE_2D, myTexture));
            GLCheck(glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, myWidth, myHeight, GL_RGBA, GL_UNSIGNED_BYTE, &myPixels[0]));

            GLCheck(glBindTexture(GL_TEXTURE_2D, PreviousTexture));
        }

        myNeedTextureUpdate = false;
    }
}


////////////////////////////////////////////////////////////
/// Make sure the array of pixels is updated with the
/// texture in video memory
////////////////////////////////////////////////////////////
void Image::EnsureArrayUpdate() const
{
    if (myNeedArrayUpdate)
    {
        // Save the previous texture
        GLint PreviousTexture;
        GLCheck(glGetIntegerv(GL_TEXTURE_BINDING_2D, &PreviousTexture));

        // Resize the destination array of pixels
        myPixels.resize(myWidth * myHeight);

        if ((myWidth == myTextureWidth) && (myHeight == myTextureHeight))
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
            std::vector<Color> AllPixels(myTextureWidth * myTextureHeight);
            GLCheck(glBindTexture(GL_TEXTURE_2D, myTexture));
            GLCheck(glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, &AllPixels[0]));

            // The we copy the useful pixels from the temporary array to the final one
            const Color* Src = &AllPixels[0];
            Color* Dst = &myPixels[0];
            for (unsigned int i = 0; i < myHeight; ++i)
            {
                std::copy(Src, Src + myWidth, Dst);
                Src += myTextureWidth;
                Dst += myWidth;
            }
        }

        // Restore the previous texture
        GLCheck(glBindTexture(GL_TEXTURE_2D, PreviousTexture));

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
        // Make sure we have a valid context
        priv::GraphicsContext Ctx;

        GLuint Texture = static_cast<GLuint>(myTexture);
        GLCheck(glDeleteTextures(1, &Texture));
        myTexture           = 0;
        myNeedTextureUpdate = false;
        myNeedArrayUpdate   = false;
    }
}

} // namespace sf
