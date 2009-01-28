////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2008 Laurent Gomila (laurent.gom@gmail.com)
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

#ifndef SFML_IMAGE_H
#define SFML_IMAGE_H

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Config.h>
#include <SFML/Graphics/Color.h>
#include <SFML/Graphics/Rect.h>


typedef struct sfRenderWindow sfRenderWindow;


////////////////////////////////////////////////////////////
/// sfImage is the low-level class for loading and
/// manipulating images
////////////////////////////////////////////////////////////
typedef struct sfImage sfImage;


////////////////////////////////////////////////////////////
/// Create a new empty image
///
/// \return A new sfImage object, or NULL if it failed
///
////////////////////////////////////////////////////////////
CSFML_API sfImage* sfImage_Create();

////////////////////////////////////////////////////////////
/// Create a new image filled with a color
///
/// \param Width :  Image width
/// \param Height : Image height
/// \param Col :    Image color
///
/// \return A new sfImage object, or NULL if it failed
///
////////////////////////////////////////////////////////////
CSFML_API sfImage* sfImage_CreateFromColor(unsigned int Width, unsigned int Height, sfColor Color);

////////////////////////////////////////////////////////////
/// Create a new image from an array of pixels in memory
///
/// \param Width :  Image width
/// \param Height : Image height
/// \param Data :   Pointer to the pixels in memory (assumed format is RGBA)
///
/// \return A new sfImage object, or NULL if it failed
///
////////////////////////////////////////////////////////////
CSFML_API sfImage* sfImage_CreateFromPixels(unsigned int Width, unsigned int Height, const sfUint8* Data);

////////////////////////////////////////////////////////////
/// Create a new image from a file
///
/// \param Filename : Path of the image file to load
///
/// \return A new sfImage object, or NULL if it failed
///
////////////////////////////////////////////////////////////
CSFML_API sfImage* sfImage_CreateFromFile(const char* Filename);

////////////////////////////////////////////////////////////
/// Create a new image from a file in memory
///
/// \param Data :        Pointer to the file data in memory
/// \param SizeInBytes : Size of the data to load, in bytes
///
/// \return A new sfImage object, or NULL if it failed
///
////////////////////////////////////////////////////////////
CSFML_API sfImage* sfImage_CreateFromMemory(const char* Data, size_t SizeInBytes);

////////////////////////////////////////////////////////////
/// Destroy an existing image
///
/// \param Image : Image to delete
///
////////////////////////////////////////////////////////////
CSFML_API void sfImage_Destroy(sfImage* Image);

////////////////////////////////////////////////////////////
/// Save the content of an image to a file
///
/// \param Image :    Image to save
/// \param Filename : Path of the file to save (overwritten if already exist)
///
/// \return sfTrue if saving was successful
///
////////////////////////////////////////////////////////////
CSFML_API sfBool sfImage_SaveToFile(sfImage* Image, const char* Filename);

////////////////////////////////////////////////////////////
/// Create a transparency mask for an image from a specified colorkey
///
/// \param Image :    Image to modify
/// \param ColorKey : Color to become transparent
/// \param Alpha :    Alpha value to use for transparent pixels
///
////////////////////////////////////////////////////////////
CSFML_API void sfImage_CreateMaskFromColor(sfImage* Image, sfColor ColorKey, sfUint8 Alpha);

////////////////////////////////////////////////////////////
/// Copy pixels from another image onto this one.
/// This function does a slow pixel copy and should only
/// be used at initialization time
///
/// \param Image :      Destination image
/// \param Source :     Source image to copy
/// \param DestX :      X coordinate of the destination position
/// \param DestY :      Y coordinate of the destination position
/// \param SourceRect : Sub-rectangle of the source image to copy
///
////////////////////////////////////////////////////////////
CSFML_API void sfImage_Copy(sfImage* Image, sfImage* Source, unsigned int DestX, unsigned int DestY, sfIntRect SourceRect);

////////////////////////////////////////////////////////////
/// Create the image from the current contents of the
/// given window
///
/// \param Image :      Destination image
/// \param Window :     Window to capture
/// \param SourceRect : Sub-rectangle of the screen to copy (empty by default - entire image)
///
/// \return True if creation was successful
///
////////////////////////////////////////////////////////////
CSFML_API sfBool sfImage_CopyScreen(sfImage* Image, sfRenderWindow* Window, sfIntRect SourceRect);

////////////////////////////////////////////////////////////
/// Change the color of a pixel of an image
/// Don't forget to call Update when you end modifying pixels
///
/// \param Image : Image to modify
/// \param X :     X coordinate of pixel in the image
/// \param Y :     Y coordinate of pixel in the image
/// \param Col :   New color for pixel (X, Y)
///
////////////////////////////////////////////////////////////
CSFML_API void sfImage_SetPixel(sfImage* Image, unsigned int X, unsigned int Y, sfColor Color);

////////////////////////////////////////////////////////////
/// Get a pixel from an image
///
/// \param Image : Image to read
/// \param X :     X coordinate of pixel in the image
/// \param Y :     Y coordinate of pixel in the image
///
/// \return Color of pixel (x, y)
///
////////////////////////////////////////////////////////////
CSFML_API sfColor sfImage_GetPixel(sfImage* Image, unsigned int X, unsigned int Y);

////////////////////////////////////////////////////////////
/// Get a read-only pointer to the array of pixels of an image (8 bits integers RGBA)
/// Array size is sfImage_GetWidth() x sfImage_GetHeight() x 4
/// This pointer becomes invalid if you reload or resize the image
///
/// \param Image : Image to read
///
/// \return Pointer to the array of pixels
///
////////////////////////////////////////////////////////////
CSFML_API const sfUint8* sfImage_GetPixelsPtr(sfImage* Image);

////////////////////////////////////////////////////////////
/// Bind the image for rendering
///
/// \param Image : Image to bind
///
////////////////////////////////////////////////////////////
CSFML_API void sfImage_Bind(sfImage* Image);

////////////////////////////////////////////////////////////
/// Enable or disable image smooth filter
///
/// \param Image :  Image to modify
/// \param Smooth : sfTrue to enable smoothing filter, false to disable it
///
////////////////////////////////////////////////////////////
CSFML_API void sfImage_SetSmooth(sfImage* Image, sfBool Smooth);

////////////////////////////////////////////////////////////
/// Return the width of the image
///
/// \param Image : Image to read
///
/// \return Width in pixels
///
////////////////////////////////////////////////////////////
CSFML_API unsigned int sfImage_GetWidth(sfImage* Image);

////////////////////////////////////////////////////////////
/// Return the height of the image
///
/// \param Image : Image to read
///
/// \return Height in pixels
///
////////////////////////////////////////////////////////////
CSFML_API unsigned int sfImage_GetHeight(sfImage* Image);

////////////////////////////////////////////////////////////
/// Tells whether the smoothing filter is enabled or not on an image
///
/// \param Image :  Image to read
///
/// \return sfTrue if the smoothing filter is enabled
///
////////////////////////////////////////////////////////////
CSFML_API sfBool sfImage_IsSmooth(sfImage* Image);


#endif // SFML_IMAGE_H
