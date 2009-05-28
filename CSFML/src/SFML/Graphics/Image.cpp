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
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Input.hpp>
#include <SFML/Internal.h>


// WARNING : this structure must always be the SAME as in Window/Input.h
struct sfInput
{
    const sf::Input* This;
};
// WARNING : this structure must always be the SAME as in Graphics/View.h
struct sfView
{
    sf::View* This;
};
// WARNING : this structure must always be the SAME as in Graphics/RenderWindow.h
struct sfRenderWindow
{
    sf::RenderWindow This;
    sfInput          Input;
    sfView           DefaultView;
    sfView*          CurrentView;
};

struct sfImage
{
    sf::Image This;
};


////////////////////////////////////////////////////////////
/// Create a new empty image
////////////////////////////////////////////////////////////
sfImage* sfImage_Create()
{
    return new sfImage;
}


////////////////////////////////////////////////////////////
/// Create a new image filled with a color
////////////////////////////////////////////////////////////
sfImage* sfImage_CreateFromColor(unsigned int Width, unsigned int Height, sfColor Color)
{
    sfImage* Image = new sfImage;

    if (!Image->This.Create(Width, Height, sf::Color(Color.r, Color.g, Color.b, Color.a)))
    {
        delete Image;
        Image = NULL;
    }

    return Image;
}


////////////////////////////////////////////////////////////
/// Create a new image from an array of pixels in memory
////////////////////////////////////////////////////////////
sfImage* sfImage_CreateFromPixels(unsigned int Width, unsigned int Height, const sfUint8* Data)
{
    sfImage* Image = new sfImage;

    if (!Image->This.LoadFromPixels(Width, Height, Data))
    {
        delete Image;
        Image = NULL;
    }

    return Image;
}


////////////////////////////////////////////////////////////
/// Create a new image from a file
////////////////////////////////////////////////////////////
sfImage* sfImage_CreateFromFile(const char* Filename)
{
    sfImage* Image = new sfImage;

    if (!Image->This.LoadFromFile(Filename))
    {
        delete Image;
        Image = NULL;
    }

    return Image;
}


////////////////////////////////////////////////////////////
/// Create a new image from a file in memory
////////////////////////////////////////////////////////////
sfImage* sfImage_CreateFromMemory(const char* Data, size_t SizeInBytes)
{
    sfImage* Image = new sfImage;

    if (!Image->This.LoadFromMemory(Data, SizeInBytes))
    {
        delete Image;
        Image = NULL;
    }

    return Image;
}


////////////////////////////////////////////////////////////
/// Destroy an existing image
////////////////////////////////////////////////////////////
void sfImage_Destroy(sfImage* Image)
{
    delete Image;
}


////////////////////////////////////////////////////////////
/// Save the content of an image to a file
////////////////////////////////////////////////////////////
sfBool sfImage_SaveToFile(sfImage* Image, const char* Filename)
{
    CSFML_CALL_RETURN(Image, SaveToFile(Filename), sfFalse);
}


////////////////////////////////////////////////////////////
/// Create a transparency mask for an image from a specified colorkey
////////////////////////////////////////////////////////////
void sfImage_CreateMaskFromColor(sfImage* Image, sfColor ColorKey, sfUint8 Alpha)
{
    sf::Color SFMLColor(ColorKey.r, ColorKey.g, ColorKey.b, ColorKey.a);
    CSFML_CALL(Image, CreateMaskFromColor(SFMLColor, Alpha));
}


////////////////////////////////////////////////////////////
/// Copy pixels from another image onto this one.
/// This function does a slow pixel copy and should only
/// be used at initialization time
////////////////////////////////////////////////////////////
void sfImage_Copy(sfImage* Image, sfImage* Source, unsigned int DestX, unsigned int DestY, sfIntRect SourceRect)
{
    CSFML_CHECK(Source);
    sf::IntRect SFMLRect(SourceRect.Left, SourceRect.Top, SourceRect.Right, SourceRect.Bottom);
    CSFML_CALL(Image, Copy(Source->This, DestX, DestY, SFMLRect));
}


////////////////////////////////////////////////////////////
/// Create the image from the current contents of the
/// given window
////////////////////////////////////////////////////////////
CSFML_API sfBool sfImage_CopyScreen(sfImage* Image, sfRenderWindow* Window, sfIntRect SourceRect)
{
    CSFML_CHECK_RETURN(Window, sfFalse);
    sf::IntRect SFMLRect(SourceRect.Left, SourceRect.Top, SourceRect.Right, SourceRect.Bottom);
    CSFML_CALL_RETURN(Image, CopyScreen(Window->This, SFMLRect), sfFalse);
}


////////////////////////////////////////////////////////////
/// Change the color of a pixel of an image
/// Don't forget to call Update when you end modifying pixels
////////////////////////////////////////////////////////////
void sfImage_SetPixel(sfImage* Image, unsigned int X, unsigned int Y, sfColor Color)
{
    sf::Color SFMLColor(Color.r, Color.g, Color.b, Color.a);
    CSFML_CALL(Image, SetPixel(X, Y, SFMLColor));
}


////////////////////////////////////////////////////////////
/// Get a pixel from an image
////////////////////////////////////////////////////////////
sfColor sfImage_GetPixel(sfImage* Image, unsigned int X, unsigned int Y)
{
    sfColor Color = {0, 0, 0, 0};
    CSFML_CHECK_RETURN(Image, Color);

    sf::Color SFMLColor = Image->This.GetPixel(X, Y);

    return sfColor_FromRGBA(SFMLColor.r, SFMLColor.g, SFMLColor.b, SFMLColor.a);
}


////////////////////////////////////////////////////////////
/// Get a read-only pointer to the array of pixels of an image (8 bits integers RGBA)
/// Array size is sfImage_GetWidth() x sfImage_GetHeight() x 4
/// This pointer becomes invalid if you reload or resize the image
////////////////////////////////////////////////////////////
const sfUint8* sfImage_GetPixelsPtr(sfImage* Image)
{
    CSFML_CALL_RETURN(Image, GetPixelsPtr(), NULL);
}


////////////////////////////////////////////////////////////
/// Bind the image for rendering
////////////////////////////////////////////////////////////
void sfImage_Bind(sfImage* Image)
{
    CSFML_CALL(Image, Bind());
}


////////////////////////////////////////////////////////////
/// Enable or disable image smooth filter
////////////////////////////////////////////////////////////
void sfImage_SetSmooth(sfImage* Image, sfBool Smooth)
{
    CSFML_CALL(Image, SetSmooth(Smooth == sfTrue));
}


////////////////////////////////////////////////////////////
/// Return the width of the image
////////////////////////////////////////////////////////////
unsigned int sfImage_GetWidth(sfImage* Image)
{
    CSFML_CALL_RETURN(Image, GetWidth(), 0);
}


////////////////////////////////////////////////////////////
/// Return the height of the image
////////////////////////////////////////////////////////////
unsigned int sfImage_GetHeight(sfImage* Image)
{
    CSFML_CALL_RETURN(Image, GetHeight(), 0);
}


////////////////////////////////////////////////////////////
/// Tells whether the smoothing filter is enabled or not on an image
////////////////////////////////////////////////////////////
sfBool sfImage_IsSmooth(sfImage* Image)
{
    CSFML_CALL_RETURN(Image, IsSmooth(), sfFalse);
}
