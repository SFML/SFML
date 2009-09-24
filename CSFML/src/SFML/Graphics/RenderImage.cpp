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
#include <SFML/Graphics/PostFXStruct.h>
#include <SFML/Graphics/SpriteStruct.h>
#include <SFML/Graphics/StringStruct.h>
#include <SFML/Internal.h>


////////////////////////////////////////////////////////////
/// Construct a new renderimage
////////////////////////////////////////////////////////////
sfRenderImage* sfRenderImage_Create(unsigned int width, unsigned int height, sfBool depthBuffer)
{
    sfRenderImage* renderImage = new sfRenderImage;
    renderImage->This.Create(width, height, depthBuffer == sfTrue);
    renderImage->Target      = new sfImage(const_cast<sf::Image*>(&renderImage->This.GetImage()));
    renderImage->DefaultView = new sfView(const_cast<sf::View*>(&renderImage->This.GetDefaultView()));
    renderImage->CurrentView = renderImage->DefaultView;

    return renderImage;
}


////////////////////////////////////////////////////////////
/// Destroy an existing renderimage
////////////////////////////////////////////////////////////
void sfRenderImage_Destroy(sfRenderImage* renderImage)
{
    delete renderImage->DefaultView;
    delete renderImage->Target;
    delete renderImage;
}


////////////////////////////////////////////////////////////
/// Get the width of the rendering region of a renderimage
////////////////////////////////////////////////////////////
unsigned int sfRenderImage_GetWidth(sfRenderImage* renderImage)
{
    CSFML_CALL_RETURN(renderImage, GetWidth(), 0);
}


////////////////////////////////////////////////////////////
/// Get the height of the rendering region of a renderimage
////////////////////////////////////////////////////////////
unsigned int sfRenderImage_GetHeight(sfRenderImage* renderImage)
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
/// Draw something on a renderimage
////////////////////////////////////////////////////////////
void sfRenderImage_DrawPostFX(sfRenderImage* renderImage, sfPostFX* postFX)
{
    CSFML_CHECK(postFX);
    CSFML_CALL(renderImage, Draw(postFX->This));
}
void sfRenderImage_DrawShape(sfRenderImage* renderImage, sfShape* shape)
{
    CSFML_CHECK(shape);
    CSFML_CALL(renderImage, Draw(shape->This));
}
void sfRenderImage_DrawSprite(sfRenderImage* renderImage, sfSprite* sprite)
{
    CSFML_CHECK(sprite);
    CSFML_CALL(renderImage, Draw(sprite->This));
}
void sfRenderImage_DrawString(sfRenderImage* renderImage, sfString* string)
{
    CSFML_CHECK(string);
    CSFML_CALL(renderImage, Draw(string->This));
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
void sfRenderImage_SetView(sfRenderImage* renderImage, sfView* view)
{
    CSFML_CHECK(view);
    CSFML_CALL(renderImage, SetView(*view->This));
    renderImage->CurrentView = view;
}


////////////////////////////////////////////////////////////
/// Get the current active view of a renderimage
////////////////////////////////////////////////////////////
const sfView* sfRenderImage_GetView(sfRenderImage* renderImage)
{
    CSFML_CHECK_RETURN(renderImage, NULL);

    return renderImage->CurrentView;
}


////////////////////////////////////////////////////////////
/// Get the default view of a renderimage
////////////////////////////////////////////////////////////
sfView* sfRenderImage_GetDefaultView(sfRenderImage* renderImage)
{
    CSFML_CHECK_RETURN(renderImage, NULL);

    return renderImage->DefaultView;
}


////////////////////////////////////////////////////////////
/// Get the viewport of a view applied to this target
////////////////////////////////////////////////////////////
sfIntRect sfRenderImage_GetViewport(sfRenderImage* renderImage, sfView* view)
{
    sfIntRect rect = {0, 0, 0, 0};
    CSFML_CHECK_RETURN(view, rect);
    CSFML_CHECK_RETURN(renderImage, rect);

    sf::IntRect SFMLrect = renderImage->This.GetViewport(*view->This);
    rect.Left   = SFMLrect.Left;
    rect.Top    = SFMLrect.Top;
    rect.Right  = SFMLrect.Right;
    rect.Bottom = SFMLrect.Bottom;

    return rect;
}


////////////////////////////////////////////////////////////
/// Convert a point in image coordinates into view coordinates
////////////////////////////////////////////////////////////
void sfRenderImage_ConvertCoords(sfRenderImage* renderImage, unsigned int imageX, unsigned int imageY, float* viewX, float* viewY, sfView* targetView)
{
    CSFML_CHECK(renderImage);

    sf::Vector2f point;
    if (targetView)
        point = renderImage->This.ConvertCoords(imageX, imageY, *targetView->This);
    else
        point = renderImage->This.ConvertCoords(imageX, imageY);

    if (viewX) *viewX = point.x;
    if (viewY) *viewY = point.y;
}


////////////////////////////////////////////////////////////
/// Get the target image
////////////////////////////////////////////////////////////
sfImage* sfRenderImage_GetImage(sfRenderImage* renderImage)
{
    CSFML_CHECK_RETURN(renderImage, NULL);

    return renderImage->Target;
}


////////////////////////////////////////////////////////////
/// Check whether the system supports render images or not
////////////////////////////////////////////////////////////
sfBool sfRenderImage_CanUseRenderImage()
{
    return sf::RenderImage::CanUseRenderImage();
}
