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

#ifndef SFML_RENDERIMAGE_H
#define SFML_RENDERIMAGE_H

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Config.h>
#include <SFML/Graphics/Color.h>
#include <SFML/Graphics/Rect.h>
#include <SFML/Graphics/Types.h>


////////////////////////////////////////////////////////////
/// Construct a new renderimage
///
/// \param Width :       Width of the renderimage
/// \param Height :      Height of the renderimage
/// \param DepthBuffer : Do you want a depth-buffer attached? (useful only if you're doing 3D OpenGL on the renderimage)
///
////////////////////////////////////////////////////////////
CSFML_API sfRenderImage* sfRenderImage_Create(unsigned int Width, unsigned int Height, sfBool DepthBuffer);

////////////////////////////////////////////////////////////
/// Destroy an existing renderimage
///
/// \param RenderImage : renderimage to destroy
///
////////////////////////////////////////////////////////////
CSFML_API void sfRenderImage_Destroy(sfRenderImage* RenderImage);

////////////////////////////////////////////////////////////
/// Get the width of the rendering region of a renderimage
///
/// \param RenderImage : Renderimage object
///
/// \return Width in pixels
///
////////////////////////////////////////////////////////////
CSFML_API unsigned int sfRenderImage_GetWidth(sfRenderImage* RenderImage);

////////////////////////////////////////////////////////////
/// Get the height of the rendering region of a renderimage
///
/// \param RenderImage : Renderimage object
///
/// \return Height in pixels
///
////////////////////////////////////////////////////////////
CSFML_API unsigned int sfRenderImage_GetHeight(sfRenderImage* RenderImage);

////////////////////////////////////////////////////////////
/// Activate or deactivate a renderimage as the current target for rendering
///
/// \param RenderImage : Renderimage object
/// \param Active :      sfTrue to activate, sfFalse to deactivate
///
/// \return True if operation was successful, false otherwise
///
////////////////////////////////////////////////////////////
CSFML_API sfBool sfRenderImage_SetActive(sfRenderImage* RenderImage, sfBool Active);

////////////////////////////////////////////////////////////
/// Draw something on a renderimage
///
/// \param RenderImage :                      Renderimage to draw in
/// \param PostFX / Sprite / String / shape : Object to draw
///
////////////////////////////////////////////////////////////
CSFML_API void sfRenderImage_DrawPostFX(sfRenderImage* RenderImage, sfPostFX* PostFX);
CSFML_API void sfRenderImage_DrawSprite(sfRenderImage* RenderImage, sfSprite* Sprite);
CSFML_API void sfRenderImage_DrawShape (sfRenderImage* RenderImage, sfShape*  Shape);
CSFML_API void sfRenderImage_DrawString(sfRenderImage* RenderImage, sfString* String);

////////////////////////////////////////////////////////////
/// Clear the renderimage with the given color
///
/// \param RenderImage : Renderimage to modify
/// \param Color :       Fill color
///
////////////////////////////////////////////////////////////
CSFML_API void sfRenderImage_Clear(sfRenderImage* RenderImage, sfColor Color);

////////////////////////////////////////////////////////////
/// Change the current active view of a renderimage
///
/// \param RenderImage : Renderimage to modify
/// \param NewView :     Pointer to the new view
///
////////////////////////////////////////////////////////////
CSFML_API void sfRenderImage_SetView(sfRenderImage* RenderImage, sfView* View);

////////////////////////////////////////////////////////////
/// Get the current active view of a renderimage
///
/// \param RenderImage : Renderimage
///
/// \return Current active view
///
////////////////////////////////////////////////////////////
CSFML_API const sfView* sfRenderImage_GetView(sfRenderImage* RenderImage);

////////////////////////////////////////////////////////////
/// Get the default view of a renderimage
///
/// \param RenderImage : Renderimage
///
/// \return Default view of the renderimage
///
////////////////////////////////////////////////////////////
CSFML_API sfView* sfRenderImage_GetDefaultView(sfRenderImage* RenderImage);

////////////////////////////////////////////////////////////
/// Tell SFML to preserve external OpenGL states, at the expense of
/// more CPU charge. Use this function if you don't want SFML
/// to mess up your own OpenGL states (if any).
/// Don't enable state preservation if not needed, as it will allow
/// SFML to do internal optimizations and improve performances.
/// This parameter is false by default
///
/// \param RenderImage : Target renderimage
/// \param Preserve :    True to preserve OpenGL states, false to let SFML optimize
///
////////////////////////////////////////////////////////////
CSFML_API void sfRenderImage_PreserveOpenGLStates(sfRenderImage* RenderImage, sfBool Preserve);

////////////////////////////////////////////////////////////
/// Get the target image
///
/// \param RenderImage : Renderimage object
///
/// \return Pointer to the target image
///
////////////////////////////////////////////////////////////
CSFML_API sfImage* sfRenderImage_GetImage(sfRenderImage* RenderImage);

////////////////////////////////////////////////////////////
/// Check whether the system supports render images or not
///
/// \return sfTrue if the RenderImage class can be used
///
////////////////////////////////////////////////////////////
CSFML_API sfBool sfRenderImage_CanUseRenderImage();


#endif // SFML_RENDERIMAGE_H
