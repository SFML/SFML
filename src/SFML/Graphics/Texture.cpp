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
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/GLCheck.hpp>
#include <SFML/Graphics/TextureSaver.hpp>
#include <SFML/Window/Window.hpp>
#include <SFML/System/Mutex.hpp>
#include <SFML/System/Lock.hpp>
#include <SFML/System/Err.hpp>
#include <cassert>
#include <cstring>


////////////////////////////////////////////////////////////
// Private data
////////////////////////////////////////////////////////////
namespace
{
    // Thread-safe unique identifier generator,
    // is used for states cache (see RenderTarget)
    sf::Uint64 GetUniqueId()
    {
        static sf::Uint64 id = 1; // start at 1, zero is "no texture"
        static sf::Mutex mutex;

        sf::Lock lock(mutex);
        return id++;
    }
}


namespace sf
{
////////////////////////////////////////////////////////////
Texture::Texture() :
myWidth        (0),
myHeight       (0),
myTextureWidth (0),
myTextureHeight(0),
myTexture      (0),
myIsSmooth     (false),
myIsRepeated   (false),
myPixelsFlipped(false),
myCacheId      (GetUniqueId())
{

}


////////////////////////////////////////////////////////////
Texture::Texture(const Texture& copy) :
myWidth        (0),
myHeight       (0),
myTextureWidth (0),
myTextureHeight(0),
myTexture      (0),
myIsSmooth     (copy.myIsSmooth),
myIsRepeated   (copy.myIsRepeated),
myPixelsFlipped(false),
myCacheId      (GetUniqueId())
{
    if (copy.myTexture)
        LoadFromImage(copy.CopyToImage());
}


////////////////////////////////////////////////////////////
Texture::~Texture()
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
bool Texture::Create(unsigned int width, unsigned int height)
{
    // Check if texture parameters are valid before creating it
    if (!width || !height)
    {
        Err() << "Failed to create texture, invalid size (" << width << "x" << height << ")" << std::endl;
        return false;
    }

    // Compute the internal texture dimensions depending on NPOT textures support
    unsigned int textureWidth  = GetValidSize(width);
    unsigned int textureHeight = GetValidSize(height);

    // Check the maximum texture size
    unsigned int maxSize = GetMaximumSize();
    if ((textureWidth > maxSize) || (textureHeight > maxSize))
    {
        Err() << "Failed to create texture, its internal size is too high "
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
    myPixelsFlipped = false;

    EnsureGlContext();

    // Create the OpenGL texture if it doesn't exist yet
    if (!myTexture)
    {
        GLuint texture;
        GLCheck(glGenTextures(1, &texture));
        myTexture = static_cast<unsigned int>(texture);
    }

    // Make sure that the current texture binding will be preserved
    priv::TextureSaver save;

    // Initialize the texture
    GLCheck(glBindTexture(GL_TEXTURE_2D, myTexture));
    GLCheck(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, myTextureWidth, myTextureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL));
    GLCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, myIsRepeated ? GL_REPEAT : GL_CLAMP_TO_EDGE));
    GLCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, myIsRepeated ? GL_REPEAT : GL_CLAMP_TO_EDGE));
    GLCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, myIsSmooth ? GL_LINEAR : GL_NEAREST));
    GLCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, myIsSmooth ? GL_LINEAR : GL_NEAREST));
    myCacheId = GetUniqueId();

    return true;
}


////////////////////////////////////////////////////////////
bool Texture::LoadFromFile(const std::string& filename, const IntRect& area)
{
    Image image;
    return image.LoadFromFile(filename) && LoadFromImage(image, area);
}


////////////////////////////////////////////////////////////
bool Texture::LoadFromMemory(const void* data, std::size_t size, const IntRect& area)
{
    Image image;
    return image.LoadFromMemory(data, size) && LoadFromImage(image, area);
}


////////////////////////////////////////////////////////////
bool Texture::LoadFromStream(InputStream& stream, const IntRect& area)
{
    Image image;
    return image.LoadFromStream(stream) && LoadFromImage(image, area);
}


////////////////////////////////////////////////////////////
bool Texture::LoadFromImage(const Image& image, const IntRect& area)
{
    // Retrieve the image size
    int width = static_cast<int>(image.GetWidth());
    int height = static_cast<int>(image.GetHeight());

    // Load the entire image if the source area is either empty or contains the whole image
    if (area.Width == 0 || (area.Height == 0) ||
       ((area.Left <= 0) && (area.Top <= 0) && (area.Width >= width) && (area.Height >= height)))
    {
        // Load the entire image
        if (Create(image.GetWidth(), image.GetHeight()))
        {
            Update(image);
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        // Load a sub-area of the image

        // Adjust the rectangle to the size of the image
        IntRect rectangle = area;
        if (rectangle.Left   < 0) rectangle.Left = 0;
        if (rectangle.Top    < 0) rectangle.Top  = 0;
        if (rectangle.Left + rectangle.Width > width)  rectangle.Width  = width - rectangle.Left;
        if (rectangle.Top + rectangle.Height > height) rectangle.Height = height - rectangle.Top;

        // Create the texture and upload the pixels
        if (Create(rectangle.Width, rectangle.Height))
        {
            // Make sure that the current texture binding will be preserved
            priv::TextureSaver save;

            // Copy the pixels to the texture, row by row
            const Uint8* pixels = image.GetPixelsPtr() + 4 * (rectangle.Left + (width * rectangle.Top));
            GLCheck(glBindTexture(GL_TEXTURE_2D, myTexture));
            for (int i = 0; i < rectangle.Height; ++i)
            {
                GLCheck(glTexSubImage2D(GL_TEXTURE_2D, 0, 0, i, myWidth, 1, GL_RGBA, GL_UNSIGNED_BYTE, pixels));
                pixels += 4 * width;
            }

            return true;
        }
        else
        {
            return false;
        }
    }
}


////////////////////////////////////////////////////////////
unsigned int Texture::GetWidth() const
{
    return myWidth;
}


////////////////////////////////////////////////////////////
unsigned int Texture::GetHeight() const
{
    return myHeight;
}


////////////////////////////////////////////////////////////
Image Texture::CopyToImage() const
{
    // Easy case: empty texture
    if (!myTexture)
        return Image();

    EnsureGlContext();

    // Make sure that the current texture binding will be preserved
    priv::TextureSaver save;

    // Create an array of pixels
    std::vector<Uint8> pixels(myWidth * myHeight * 4);

    if ((myWidth == myTextureWidth) && (myHeight == myTextureHeight) && !myPixelsFlipped)
    {
        // Texture is not padded nor flipped, we can use a direct copy
        GLCheck(glBindTexture(GL_TEXTURE_2D, myTexture));
        GLCheck(glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, &pixels[0]));
    }
    else
    {
        // Texture is either padded or flipped, we have to use a slower algorithm

        // All the pixels will first be copied to a temporary array
        std::vector<Uint8> allPixels(myTextureWidth * myTextureHeight * 4);
        GLCheck(glBindTexture(GL_TEXTURE_2D, myTexture));
        GLCheck(glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, &allPixels[0]));

        // Then we copy the useful pixels from the temporary array to the final one
        const Uint8* src = &allPixels[0];
        Uint8* dst = &pixels[0];
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

    // Create the image
    Image image;
    image.Create(myWidth, myHeight, &pixels[0]);

    return image;
}


////////////////////////////////////////////////////////////
void Texture::Update(const Uint8* pixels)
{
    // Update the whole texture
    Update(pixels, myWidth, myHeight, 0, 0);
}


////////////////////////////////////////////////////////////
void Texture::Update(const Uint8* pixels, unsigned int width, unsigned int height, unsigned int x, unsigned int y)
{
    assert(x + width <= myWidth);
    assert(y + height <= myHeight);

    if (pixels && myTexture)
    {
        EnsureGlContext();

        // Make sure that the current texture binding will be preserved
        priv::TextureSaver save;

        // Copy pixels from the given array to the texture
        GLCheck(glBindTexture(GL_TEXTURE_2D, myTexture));
        GLCheck(glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels));
        myPixelsFlipped = false;
        myCacheId = GetUniqueId();
    }
}


////////////////////////////////////////////////////////////
void Texture::Update(const Image& image)
{
    // Update the whole texture
    Update(image.GetPixelsPtr(), image.GetWidth(), image.GetHeight(), 0, 0);
}


////////////////////////////////////////////////////////////
void Texture::Update(const Image& image, unsigned int x, unsigned int y)
{
    Update(image.GetPixelsPtr(), image.GetWidth(), image.GetHeight(), x, y);
}


////////////////////////////////////////////////////////////
void Texture::Update(const Window& window)
{
    Update(window, 0, 0);
}


////////////////////////////////////////////////////////////
void Texture::Update(const Window& window, unsigned int x, unsigned int y)
{
    assert(x + window.GetWidth() <= myWidth);
    assert(y + window.GetHeight() <= myHeight);

    if (myTexture && window.SetActive(true))
    {
        // Make sure that the current texture binding will be preserved
        priv::TextureSaver save;

        // Copy pixels from the back-buffer to the texture
        GLCheck(glBindTexture(GL_TEXTURE_2D, myTexture));
        GLCheck(glCopyTexSubImage2D(GL_TEXTURE_2D, 0, x, y, 0, 0, window.GetWidth(), window.GetHeight()));
        myPixelsFlipped = true;
        myCacheId = GetUniqueId();
    }
}


////////////////////////////////////////////////////////////
void Texture::Bind(CoordinateType coordinateType) const
{
    // Bind the texture
    GLCheck(glBindTexture(GL_TEXTURE_2D, myTexture));

    // Check if we need to define a special texture matrix
    if ((coordinateType == Pixels) || myPixelsFlipped)
    {
        GLfloat matrix[16] = {1.f, 0.f, 0.f, 0.f,
                              0.f, 1.f, 0.f, 0.f,
                              0.f, 0.f, 1.f, 0.f,
                              0.f, 0.f, 0.f, 1.f};

        // If non-normalized coordinates (= pixels) are requested, we need to
        // setup scale factors that convert the range [0 .. size] to [0 .. 1]
        if (coordinateType == Pixels)
        {
            matrix[0] = 1.f / myTextureWidth;
            matrix[5] = 1.f / myTextureHeight;
        }

        // If pixels are flipped we must invert the Y axis
        if (myPixelsFlipped)
        {
            matrix[5] = -matrix[5];
            matrix[13] = static_cast<float>(myHeight / myTextureHeight);
        }

        // Load the matrix
        GLCheck(glMatrixMode(GL_TEXTURE));
        GLCheck(glLoadMatrixf(matrix));

        // Go back to model-view mode (sf::RenderTarget relies on it)
        GLCheck(glMatrixMode(GL_MODELVIEW));
    }
}


////////////////////////////////////////////////////////////
void Texture::SetSmooth(bool smooth)
{
    if (smooth != myIsSmooth)
    {
        myIsSmooth = smooth;

        if (myTexture)
        {
            EnsureGlContext();

            // Make sure that the current texture binding will be preserved
            priv::TextureSaver save;

            GLCheck(glBindTexture(GL_TEXTURE_2D, myTexture));
            GLCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, myIsSmooth ? GL_LINEAR : GL_NEAREST));
            GLCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, myIsSmooth ? GL_LINEAR : GL_NEAREST));
        }
    }
}


////////////////////////////////////////////////////////////
bool Texture::IsSmooth() const
{
    return myIsSmooth;
}


////////////////////////////////////////////////////////////
void Texture::SetRepeated(bool repeated)
{
    if (repeated != myIsRepeated)
    {
        myIsRepeated = repeated;

        if (myTexture)
        {
            EnsureGlContext();

            // Make sure that the current texture binding will be preserved
            priv::TextureSaver save;

            GLCheck(glBindTexture(GL_TEXTURE_2D, myTexture));
            GLCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, myIsRepeated ? GL_REPEAT : GL_CLAMP_TO_EDGE));
            GLCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, myIsRepeated ? GL_REPEAT : GL_CLAMP_TO_EDGE));
        }
    }
}


////////////////////////////////////////////////////////////
bool Texture::IsRepeated() const
{
    return myIsRepeated;
}


////////////////////////////////////////////////////////////
unsigned int Texture::GetMaximumSize()
{
    EnsureGlContext();

    GLint size;
    GLCheck(glGetIntegerv(GL_MAX_TEXTURE_SIZE, &size));

    return static_cast<unsigned int>(size);
}


////////////////////////////////////////////////////////////
Texture& Texture::operator =(const Texture& right)
{
    Texture temp(right);

    std::swap(myWidth,         temp.myWidth);
    std::swap(myHeight,        temp.myHeight);
    std::swap(myTextureWidth,  temp.myTextureWidth);
    std::swap(myTextureHeight, temp.myTextureHeight);
    std::swap(myTexture,       temp.myTexture);
    std::swap(myIsSmooth,      temp.myIsSmooth);
    std::swap(myIsRepeated,    temp.myIsRepeated);
    std::swap(myPixelsFlipped, temp.myPixelsFlipped);
    myCacheId = GetUniqueId();

    return *this;
}


////////////////////////////////////////////////////////////
unsigned int Texture::GetValidSize(unsigned int size)
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

} // namespace sf
