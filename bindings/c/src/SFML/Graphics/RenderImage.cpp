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
#include <SFML/Graphics/RenderImage.h>
#include <SFML/Graphics/RenderImageStruct.h>
#include <SFML/Graphics/ShapeStruct.h>
#include <SFML/Graphics/ShaderStruct.h>
#include <SFML/Graphics/SpriteStruct.h>
#include <SFML/Graphics/TextStruct.h>
#include <SFML/Internal.h>


////////////////////////////////////////////////////////////
/// Construct a new renderimage
////////////////////////////////////////////////////////////
sfRenderImage* sfRenderImage_Create(unsigned int width, unsigned int height, sfBool depthBuffer)
{
    sfRenderImage* renderImage = new sfRenderImage;
    renderImage->This.Create(width, height, depthBuffer == sfTrue);
    renderImage->Target = new sfImage(const_cast<sf::Image*>(&renderImage->This.GetImage()));
    renderImage->DefaultView.This = renderImage->This.GetDefaultView();
    renderImage->CurrentView.This = renderImage->This.GetView();

    return renderImage;
}


////////////////////////////////////////////////////////////
/// Destroy an existing renderimage
////////////////////////////////////////////////////////////
void sfRenderImage_Destroy(sfRenderImage* renderImage)
{
    delete renderImage->Target;
    delete renderImage;
}


////////////////////////////////////////////////////////////
/// Get the width of the rendering region of a renderimage
////////////////////////////////////////////////////////////
unsigned int sfRenderImage_GetWidth(const sfRenderImage* renderImage)
{
    CSFML_CALL_RETURN(renderImage, GetWidth(), 0);
}


////////////////////////////////////////////////////////////
/// Get the height of the rendering region of a renderimage
////////////////////////////////////////////////////////////
unsigned int sfRenderImage_GetHeight(const sfRenderImage* renderImage)
{
    CSFML_CALL_RETURN(renderImage, GetHeight(), 0);
}


////////////////////////////////////////////////////////////
/// Set a renderimage as the current target for rendering
////////////////////////////////////////////////////////////
sfBool sfRenderImage_SetActive(sfRenderImage* renderImage, sfBool active)
{
    CSFML_CALL_RETURN(renderImage, SetActive(active == sfTrue), sfFalse)
}


////////////////////////////////////////////////////////////
/// Save the current OpenGL render states and matrices
////////////////////////////////////////////////////////////
void sfRenderImage_SaveGLStates(sfRenderImage* renderImage)
{
    CSFML_CALL(renderImage, SaveGLStates());
}


////////////////////////////////////////////////////////////
/// Restore the previously saved OpenGL render states and matrices
////////////////////////////////////////////////////////////
void sfRenderImage_RestoreGLStates(sfRenderImage* renderImage)
{
    CSFML_CALL(renderImage, RestoreGLStates());
}


////////////////////////////////////////////////////////////
/// Update the contents of the target image
////////////////////////////////////////////////////////////
void sfRenderImage_Display(sfRenderImage* renderImage)
{
    CSFML_CALL(renderImage, Display())
}


////////////////////////////////////////////////////////////
/// Draw something on a renderimage
////////////////////////////////////////////////////////////
void sfRenderImage_DrawSprite(sfRenderImage* renderImage, const sfSprite* sprite)
{
    CSFML_CHECK(sprite);
    CSFML_CALL(renderImage, Draw(sprite->This));
}
void sfRenderImage_DrawShape(sfRenderImage* renderImage, const sfShape* shape)
{
    CSFML_CHECK(shape);
    CSFML_CALL(renderImage, Draw(shape->This));
}
void sfRenderImage_DrawText(sfRenderImage* renderImage, const sfText* text)
{
    CSFML_CHECK(text);
    CSFML_CALL(renderImage, Draw(text->This));
}


////////////////////////////////////////////////////////////
/// Draw something on a renderimage with a shader
////////////////////////////////////////////////////////////
void sfRenderImage_DrawSpriteWithShader(sfRenderImage* renderImage, const sfSprite* sprite, const sfShader* shader)
{
    CSFML_CHECK(sprite);
    CSFML_CHECK(shader);
    CSFML_CALL(renderImage, Draw(sprite->This, shader->This));
}
void sfRenderImage_DrawShapeWithShader(sfRenderImage* renderImage, const sfShape* shape, const sfShader* shader)
{
    CSFML_CHECK(shape);
    CSFML_CHECK(shader);
    CSFML_CALL(renderImage, Draw(shape->This, shader->This));
}
void sfRenderImage_DrawTextWithShader(sfRenderImage* renderImage, const sfText* text, const sfShader* shader)
{
    CSFML_CHECK(text);
    CSFML_CHECK(shader);
    CSFML_CALL(renderImage, Draw(text->This, shader->This));
}


////////////////////////////////////////////////////////////
/// Clear the renderimage with the given color
////////////////////////////////////////////////////////////
void sfRenderImage_Clear(sfRenderImage* renderImage, sfColor color)
{
    sf::Color SFMLColor(color.r, color.g, color.b, color.a);

    CSFML_CALL(renderImage, Clear(SFMLColor));
}


////////////////////////////////////////////////////////////
/// Change the current active view of a renderimage
////////////////////////////////////////////////////////////
void sfRenderImage_SetView(sfRenderImage* renderImage, const sfView* view)
{
    CSFML_CHECK(view);
    CSFML_CALL(renderImage, SetView(view->This));
    renderImage->CurrentView.This = view->This;
}


////////////////////////////////////////////////////////////
/// Get the current active view of a renderimage
////////////////////////////////////////////////////////////
const sfView* sfRenderImage_GetView(const sfRenderImage* renderImage)
{
    CSFML_CHECK_RETURN(renderImage, NULL);

    return &renderImage->CurrentView;
}


////////////////////////////////////////////////////////////
/// Get the default view of a renderimage
////////////////////////////////////////////////////////////
const sfView* sfRenderImage_GetDefaultView(const sfRenderImage* renderImage)
{
    CSFML_CHECK_RETURN(renderImage, NULL);

    return &renderImage->DefaultView;
}


////////////////////////////////////////////////////////////
/// Get the viewport of a view applied to this target
////////////////////////////////////////////////////////////
sfIntRect sfRenderImage_GetViewport(const sfRenderImage* renderImage, const sfView* view)
{
    sfIntRect rect = {0, 0, 0, 0};
    CSFML_CHECK_RETURN(view, rect);
    CSFML_CHECK_RETURN(renderImage, rect);

    sf::IntRect SFMLrect = renderImage->This.GetViewport(view->This);
    rect.Left   = SFMLrect.Left;
    rect.Top    = SFMLrect.Top;
    rect.Width  = SFMLrect.Width;
    rect.Height = SFMLrect.Height;

    return rect;
}


////////////////////////////////////////////////////////////
/// Convert a point in image coordinates into view coordinates
////////////////////////////////////////////////////////////
void sfRenderImage_ConvertCoords(const sfRenderImage* renderImage, unsigned int imageX, unsigned int imageY, float* viewX, float* viewY, const sfView* targetView)
{
    CSFML_CHECK(renderImage);

    sf::Vector2f point;
    if (targetView)
        point = renderImage->This.ConvertCoords(imageX, imageY, targetView->This);
    else
        point = renderImage->This.ConvertCoords(imageX, imageY);

    if (viewX) *viewX = point.x;
    if (viewY) *viewY = point.y;
}


////////////////////////////////////////////////////////////
/// Get the target image
////////////////////////////////////////////////////////////
const sfImage* sfRenderImage_GetImage(const sfRenderImage* renderImage)
{
    CSFML_CHECK_RETURN(renderImage, NULL);

    return renderImage->Target;
}


////////////////////////////////////////////////////////////
/// Check whether the system supports render images or not
////////////////////////////////////////////////////////////
sfBool sfRenderImage_IsAvailable(void)
{
    return sf::RenderImage::IsAvailable() ? sfTrue : sfFalse;
}
