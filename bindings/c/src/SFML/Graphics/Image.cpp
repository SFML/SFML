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
#include <SFML/Graphics/Image.h>
#include <SFML/Graphics/ImageStruct.h>
#include <SFML/Graphics/RenderWindowStruct.h>
#include <SFML/Internal.h>


////////////////////////////////////////////////////////////
/// Create a new image filled with a color
////////////////////////////////////////////////////////////
sfImage* sfImage_CreateFromColor(unsigned int width, unsigned int height, sfColor color)
{
    sfImage* image = new sfImage;

    if (!image->This->Create(width, height, sf::Color(color.r, color.g, color.b, color.a)))
    {
        delete image;
        image = NULL;
    }

    return image;
}


////////////////////////////////////////////////////////////
/// Create a new image from an array of pixels in memory
////////////////////////////////////////////////////////////
sfImage* sfImage_CreateFromPixels(unsigned int width, unsigned int height, const sfUint8* data)
{
    sfImage* image = new sfImage;

    if (!image->This->LoadFromPixels(width, height, data))
    {
        delete image;
        image = NULL;
    }

    return image;
}


////////////////////////////////////////////////////////////
/// Create a new image from a file
////////////////////////////////////////////////////////////
sfImage* sfImage_CreateFromFile(const char* filename)
{
    sfImage* image = new sfImage;

    if (!image->This->LoadFromFile(filename))
    {
        delete image;
        image = NULL;
    }

    return image;
}


////////////////////////////////////////////////////////////
/// Create a new image from a file in memory
////////////////////////////////////////////////////////////
sfImage* sfImage_CreateFromMemory(const void* data, size_t sizeInBytes)
{
    sfImage* image = new sfImage;

    if (!image->This->LoadFromMemory(data, sizeInBytes))
    {
        delete image;
        image = NULL;
    }

    return image;
}


////////////////////////////////////////////////////////////
/// Copy an existing image
////////////////////////////////////////////////////////////
sfImage* sfImage_Copy(sfImage* image)
{
    CSFML_CHECK_RETURN(image, NULL);

    return new sfImage(*image);
}


////////////////////////////////////////////////////////////
/// Destroy an existing image
////////////////////////////////////////////////////////////
void sfImage_Destroy(sfImage* image)
{
    delete image;
}


////////////////////////////////////////////////////////////
/// Save the content of an image to a file
////////////////////////////////////////////////////////////
sfBool sfImage_SaveToFile(const sfImage* image, const char* filename)
{
    CSFML_CALL_PTR_RETURN(image, SaveToFile(filename), sfFalse);
}


////////////////////////////////////////////////////////////
/// Create a transparency mask for an image from a specified colorkey
////////////////////////////////////////////////////////////
void sfImage_CreateMaskFromColor(sfImage* image, sfColor colorKey, sfUint8 alpha)
{
    sf::Color SFMLColor(colorKey.r, colorKey.g, colorKey.b, colorKey.a);
    CSFML_CALL_PTR(image, CreateMaskFromColor(SFMLColor, alpha));
}


////////////////////////////////////////////////////////////
/// Copy pixels from another image onto this one.
/// This function does a slow pixel copy and should only
/// be used at initialization time
////////////////////////////////////////////////////////////
void sfImage_CopyImage(sfImage* image, const sfImage* source, unsigned int destX, unsigned int destY, sfIntRect sourceRect)
{
    CSFML_CHECK(source);
    sf::IntRect SFMLRect(sourceRect.Left, sourceRect.Top, sourceRect.Width, sourceRect.Height);
    CSFML_CALL_PTR(image, Copy(*source->This, destX, destY, SFMLRect));
}


////////////////////////////////////////////////////////////
/// Create the image from the current contents of the
/// given window
////////////////////////////////////////////////////////////
CSFML_API sfBool sfImage_CopyScreen(sfImage* image, sfRenderWindow* window, sfIntRect sourceRect)
{
    CSFML_CHECK_RETURN(window, sfFalse);
    sf::IntRect SFMLRect(sourceRect.Left, sourceRect.Top, sourceRect.Width, sourceRect.Height);
    CSFML_CALL_PTR_RETURN(image, CopyScreen(window->This, SFMLRect), sfFalse);
}


////////////////////////////////////////////////////////////
/// Change the color of a pixel of an image
/// Don't forget to call Update when you end modifying pixels
////////////////////////////////////////////////////////////
void sfImage_SetPixel(sfImage* image, unsigned int x, unsigned int y, sfColor color)
{
    sf::Color SFMLColor(color.r, color.g, color.b, color.a);
    CSFML_CALL_PTR(image, SetPixel(x, y, SFMLColor));
}


////////////////////////////////////////////////////////////
/// Get a pixel from an image
////////////////////////////////////////////////////////////
sfColor sfImage_GetPixel(const sfImage* image, unsigned int x, unsigned int y)
{
    sfColor color = {0, 0, 0, 0};
    CSFML_CHECK_RETURN(image, color);

    sf::Color SFMLColor = image->This->GetPixel(x, y);

    return sfColor_FromRGBA(SFMLColor.r, SFMLColor.g, SFMLColor.b, SFMLColor.a);
}


////////////////////////////////////////////////////////////
/// Get a read-only pointer to the array of pixels of an image (8 bits integers RGBA)
/// Array size is sfImage_GetWidth() x sfImage_GetHeight() x 4
/// This pointer becomes invalid if you reload or resize the image
////////////////////////////////////////////////////////////
const sfUint8* sfImage_GetPixelsPtr(const sfImage* image)
{
    CSFML_CALL_PTR_RETURN(image, GetPixelsPtr(), NULL);
}


////////////////////////////////////////////////////////////
/// Update a sub-rectangle of the image from an array of pixels
////////////////////////////////////////////////////////////
void sfImage_UpdatePixels(const sfImage* image, const sfUint8* pixels, sfIntRect rectangle)
{
    sf::IntRect rect(rectangle.Left, rectangle.Top, rectangle.Width, rectangle.Height);
    CSFML_CALL_PTR(image, UpdatePixels(pixels, rect));
}


////////////////////////////////////////////////////////////
/// Bind the image for rendering
////////////////////////////////////////////////////////////
void sfImage_Bind(const sfImage* image)
{
    CSFML_CALL_PTR(image, Bind());
}


////////////////////////////////////////////////////////////
/// Enable or disable image smooth filter
////////////////////////////////////////////////////////////
void sfImage_SetSmooth(sfImage* image, sfBool smooth)
{
    CSFML_CALL_PTR(image, SetSmooth(smooth == sfTrue));
}


////////////////////////////////////////////////////////////
/// Return the width of the image
////////////////////////////////////////////////////////////
unsigned int sfImage_GetWidth(const sfImage* image)
{
    CSFML_CALL_PTR_RETURN(image, GetWidth(), 0);
}


////////////////////////////////////////////////////////////
/// Return the height of the image
////////////////////////////////////////////////////////////
unsigned int sfImage_GetHeight(const sfImage* image)
{
    CSFML_CALL_PTR_RETURN(image, GetHeight(), 0);
}


////////////////////////////////////////////////////////////
/// Tells whether the smoothing filter is enabled or not on an image
////////////////////////////////////////////////////////////
sfBool sfImage_IsSmooth(const sfImage* image)
{
    CSFML_CALL_PTR_RETURN(image, IsSmooth(), sfFalse);
}
