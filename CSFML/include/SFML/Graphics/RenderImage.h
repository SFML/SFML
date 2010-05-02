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
/// \param width :       Width of the renderimage
/// \param height :      Height of the renderimage
/// \param depthBuffer : Do you want a depth-buffer attached? (useful only if you're doing 3D OpenGL on the renderimage)
///
////////////////////////////////////////////////////////////
CSFML_API sfRenderImage* sfRenderImage_Create(unsigned int width, unsigned int height, sfBool depthBuffer);

////////////////////////////////////////////////////////////
/// Destroy an existing renderimage
///
/// \param renderImage : renderimage to destroy
///
////////////////////////////////////////////////////////////
CSFML_API void sfRenderImage_Destroy(sfRenderImage* renderImage);

////////////////////////////////////////////////////////////
/// Get the width of the rendering region of a renderimage
///
/// \param renderImage : Renderimage object
///
/// \return Width in pixels
///
////////////////////////////////////////////////////////////
CSFML_API unsigned int sfRenderImage_GetWidth(const sfRenderImage* renderImage);

////////////////////////////////////////////////////////////
/// Get the height of the rendering region of a renderimage
///
/// \param renderImage : Renderimage object
///
/// \return Height in pixels
///
////////////////////////////////////////////////////////////
CSFML_API unsigned int sfRenderImage_GetHeight(const sfRenderImage* renderImage);

////////////////////////////////////////////////////////////
/// Activate or deactivate a renderimage as the current target for rendering
///
/// \param renderImage : Renderimage object
/// \param active :      sfTrue to activate, sfFalse to deactivate
///
/// \return True if operation was successful, false otherwise
///
////////////////////////////////////////////////////////////
CSFML_API sfBool sfRenderImage_SetActive(sfRenderImage* renderImage, sfBool active);

////////////////////////////////////////////////////////////
/// Save the current OpenGL render states and matrices
///
/// \param renderWindow : Renderwindow object
///
////////////////////////////////////////////////////////////
CSFML_API void sfRenderImage_SaveGLStates(sfRenderImage* renderImage);

////////////////////////////////////////////////////////////
/// Restore the previously saved OpenGL render states and matrices
///
/// \param renderWindow : Renderwindow object
///
////////////////////////////////////////////////////////////
CSFML_API void sfRenderImage_RestoreGLStates(sfRenderImage* renderImage);

////////////////////////////////////////////////////////////
/// Update the contents of the target image
///
/// \param renderImage : Renderimage object
///
////////////////////////////////////////////////////////////
CSFML_API void sfRenderImage_Display(sfRenderImage* renderImage);

////////////////////////////////////////////////////////////
/// Draw something on a renderimage
///
/// \param renderImage :           Renderimage to draw in
/// \param sprite / text / shape : Object to draw
///
////////////////////////////////////////////////////////////
CSFML_API void sfRenderImage_DrawSprite(sfRenderImage* renderImage, const sfSprite* sprite);
CSFML_API void sfRenderImage_DrawShape (sfRenderImage* renderImage, const sfShape*  shape);
CSFML_API void sfRenderImage_DrawText  (sfRenderImage* renderImage, const sfText*   text);

////////////////////////////////////////////////////////////
/// Draw something on a renderimage with a shader
///
/// \param renderImage :           Renderimage to draw in
/// \param sprite / text / shape : Object to draw
/// \param shader :                Shader to use
///
////////////////////////////////////////////////////////////
CSFML_API void sfRenderImage_DrawSpriteWithShader(sfRenderImage* renderImage, const sfSprite* sprite, const sfShader* shader);
CSFML_API void sfRenderImage_DrawShapeWithShader (sfRenderImage* renderImage, const sfShape*  shape,  const sfShader* shader);
CSFML_API void sfRenderImage_DrawTextWithShader  (sfRenderImage* renderImage, const sfText*   text,   const sfShader* shader);

////////////////////////////////////////////////////////////
/// Clear the renderimage with the given color
///
/// \param renderImage : Renderimage to modify
/// \param color :       Fill color
///
////////////////////////////////////////////////////////////
CSFML_API void sfRenderImage_Clear(sfRenderImage* renderImage, sfColor color);

////////////////////////////////////////////////////////////
/// Change the current active view of a renderimage
///
/// \param renderImage : Renderimage to modify
/// \param view :        Pointer to the new view
///
////////////////////////////////////////////////////////////
CSFML_API void sfRenderImage_SetView(sfRenderImage* renderImage, const sfView* view);

////////////////////////////////////////////////////////////
/// Get the current active view of a renderimage
///
/// \param renderImage : Renderimage
///
/// \return Current active view
///
////////////////////////////////////////////////////////////
CSFML_API const sfView* sfRenderImage_GetView(const sfRenderImage* renderImage);

////////////////////////////////////////////////////////////
/// Get the default view of a renderimage
///
/// \param renderImage : Renderimage
///
/// \return Default view of the renderimage
///
////////////////////////////////////////////////////////////
CSFML_API const sfView* sfRenderImage_GetDefaultView(const sfRenderImage* renderImage);

////////////////////////////////////////////////////////////
/// Get the viewport of a view applied to this target
///
/// \param renderImage : Renderimage object
/// \param view :        Target view
///
/// \return Viewport rectangle, expressed in pixels in the current target
///
////////////////////////////////////////////////////////////
CSFML_API sfIntRect sfRenderImage_GetViewport(const sfRenderImage* renderImage, const sfView* view);

////////////////////////////////////////////////////////////
/// Convert a point in image coordinates into view coordinates
///
/// \param renderImage : Renderimage object
/// \param windowX :     X coordinate of the point to convert, relative to the image
/// \param windowY :     Y coordinate of the point to convert, relative to the image
/// \param viewX :       Pointer to fill with the X coordinate of the converted point
/// \param viewY :       Pointer to fill with the Y coordinate of the converted point
/// \param targetView :  Target view to convert the point to (pass NULL to use the current view)
///
////////////////////////////////////////////////////////////
CSFML_API void sfRenderImage_ConvertCoords(const sfRenderImage* renderImage, unsigned int windowX, unsigned int windowY, float* viewX, float* viewY, const sfView* targetView);

////////////////////////////////////////////////////////////
/// Get the target image
///
/// \param renderImage : Renderimage object
///
/// \return Pointer to the target image
///
////////////////////////////////////////////////////////////
CSFML_API const sfImage* sfRenderImage_GetImage(const sfRenderImage* renderImage);

////////////////////////////////////////////////////////////
/// Check whether the system supports render images or not
///
/// \return sfTrue if the RenderImage class can be used
///
////////////////////////////////////////////////////////////
CSFML_API sfBool sfRenderImage_IsAvailable(void);


#endif // SFML_RENDERIMAGE_H
