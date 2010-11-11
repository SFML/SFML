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

#ifndef SFML_IMAGE_H
#define SFML_IMAGE_H

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Config.h>
#include <SFML/Graphics/Color.h>
#include <SFML/Graphics/Rect.h>
#include <SFML/Graphics/Types.h>


////////////////////////////////////////////////////////////
/// Create a new image filled with a color
///
/// \param width :  Image width
/// \param height : Image height
/// \param color :  Image color
///
/// \return A new sfImage object, or NULL if it failed
///
////////////////////////////////////////////////////////////
CSFML_API sfImage* sfImage_CreateFromColor(unsigned int width, unsigned int height, sfColor color);

////////////////////////////////////////////////////////////
/// Create a new image from an array of pixels in memory
///
/// \param width :  Image width
/// \param height : Image height
/// \param data :   Pointer to the pixels in memory (assumed format is RGBA)
///
/// \return A new sfImage object, or NULL if it failed
///
////////////////////////////////////////////////////////////
CSFML_API sfImage* sfImage_CreateFromPixels(unsigned int width, unsigned int height, const sfUint8* data);

////////////////////////////////////////////////////////////
/// Create a new image from a file
///
/// \param filename : Path of the image file to load
///
/// \return A new sfImage object, or NULL if it failed
///
////////////////////////////////////////////////////////////
CSFML_API sfImage* sfImage_CreateFromFile(const char* filename);

////////////////////////////////////////////////////////////
/// Create a new image from a file in memory
///
/// \param data :        Pointer to the file data in memory
/// \param sizeInBytes : Size of the data to load, in bytes
///
/// \return A new sfImage object, or NULL if it failed
///
////////////////////////////////////////////////////////////
CSFML_API sfImage* sfImage_CreateFromMemory(const void* data, size_t sizeInBytes);

////////////////////////////////////////////////////////////
/// Copy an existing image
///
/// \param image : Image to copy
///
/// \return Copied object
///
////////////////////////////////////////////////////////////
CSFML_API sfImage* sfImage_Copy(sfImage* image);

////////////////////////////////////////////////////////////
/// Destroy an existing image
///
/// \param image : Image to delete
///
////////////////////////////////////////////////////////////
CSFML_API void sfImage_Destroy(sfImage* image);

////////////////////////////////////////////////////////////
/// Save the content of an image to a file
///
/// \param image :    Image to save
/// \param filename : Path of the file to save (overwritten if already exist)
///
/// \return sfTrue if saving was successful
///
////////////////////////////////////////////////////////////
CSFML_API sfBool sfImage_SaveToFile(const sfImage* image, const char* filename);

////////////////////////////////////////////////////////////
/// Create a transparency mask for an image from a specified colorkey
///
/// \param image :    Image to modify
/// \param colorKey : Color to become transparent
/// \param alpha :    Alpha value to use for transparent pixels
///
////////////////////////////////////////////////////////////
CSFML_API void sfImage_CreateMaskFromColor(sfImage* image, sfColor colorKey, sfUint8 alpha);

////////////////////////////////////////////////////////////
/// Copy pixels from another image onto this one.
/// This function does a slow pixel copy and should only
/// be used at initialization time
///
/// \param image :      Destination image
/// \param source :     Source image to copy
/// \param destX :      X coordinate of the destination position
/// \param destY :      Y coordinate of the destination position
/// \param sourceRect : Sub-rectangle of the source image to copy
///
////////////////////////////////////////////////////////////
CSFML_API void sfImage_CopyImage(sfImage* image, const sfImage* source, unsigned int destX, unsigned int destY, sfIntRect sourceRect);

////////////////////////////////////////////////////////////
/// Create the image from the current contents of the
/// given window
///
/// \param image :      Destination image
/// \param window :     Window to capture
/// \param sourceRect : Sub-rectangle of the screen to copy (empty by default - entire image)
///
/// \return True if creation was successful
///
////////////////////////////////////////////////////////////
CSFML_API sfBool sfImage_CopyScreen(sfImage* image, sfRenderWindow* window, sfIntRect sourceRect);

////////////////////////////////////////////////////////////
/// Change the color of a pixel of an image
///
/// \param image : Image to modify
/// \param x :     X coordinate of pixel in the image
/// \param y :     Y coordinate of pixel in the image
/// \param color : New color for pixel (X, Y)
///
////////////////////////////////////////////////////////////
CSFML_API void sfImage_SetPixel(sfImage* image, unsigned int x, unsigned int y, sfColor color);

////////////////////////////////////////////////////////////
/// Get a pixel from an image
///
/// \param image : Image to read
/// \param x :     X coordinate of pixel in the image
/// \param y :     Y coordinate of pixel in the image
///
/// \return Color of pixel (x, y)
///
////////////////////////////////////////////////////////////
CSFML_API sfColor sfImage_GetPixel(const sfImage* image, unsigned int x, unsigned int y);

////////////////////////////////////////////////////////////
/// Get a read-only pointer to the array of pixels of an image (8 bits integers RGBA)
/// Array size is sfImage_GetWidth(img) x sfImage_GetHeight(img) x 4
/// This pointer becomes invalid if you reload or resize the image
///
/// \param image : Image to read
///
/// \return Pointer to the array of pixels
///
////////////////////////////////////////////////////////////
CSFML_API const sfUint8* sfImage_GetPixelsPtr(const sfImage* image);

////////////////////////////////////////////////////////////
/// Update a sub-rectangle of the image from an array of pixels
///
/// Warning: for performances reasons, this function doesn't
/// perform any check; thus you're responsible of ensuring that
/// \a rectangle does not exceed the image size, and that
/// \a pixels contains enough elements.
///
/// \param image :     Image to update
/// \param rectangle : Sub-rectangle of the image to update
/// \param pixels :    Array of pixels to write to the image
///
////////////////////////////////////////////////////////////
CSFML_API void sfImage_UpdatePixels(const sfImage* image, const sfUint8* pixels, sfIntRect rectangle);

////////////////////////////////////////////////////////////
/// Bind the image for rendering
///
/// \param image : Image to bind
///
////////////////////////////////////////////////////////////
CSFML_API void sfImage_Bind(const sfImage* image);

////////////////////////////////////////////////////////////
/// Enable or disable image smooth filter
///
/// \param image :  Image to modify
/// \param smooth : sfTrue to enable smoothing filter, false to disable it
///
////////////////////////////////////////////////////////////
CSFML_API void sfImage_SetSmooth(sfImage* image, sfBool smooth);

////////////////////////////////////////////////////////////
/// Return the width of the image
///
/// \param image : Image to read
///
/// \return Width in pixels
///
////////////////////////////////////////////////////////////
CSFML_API unsigned int sfImage_GetWidth(const sfImage* image);

////////////////////////////////////////////////////////////
/// Return the height of the image
///
/// \param image : Image to read
///
/// \return Height in pixels
///
////////////////////////////////////////////////////////////
CSFML_API unsigned int sfImage_GetHeight(const sfImage* image);

////////////////////////////////////////////////////////////
/// Tells whether the smoothing filter is enabled or not on an image
///
/// \param image :  Image to read
///
/// \return sfTrue if the smoothing filter is enabled
///
////////////////////////////////////////////////////////////
CSFML_API sfBool sfImage_IsSmooth(const sfImage* image);


#endif // SFML_IMAGE_H
