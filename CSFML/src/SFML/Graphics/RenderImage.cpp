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
sfRenderImage* sfRenderImage_Create(unsigned int Width, unsigned int Height, sfBool DepthBuffer)
{
    sfRenderImage* RenderImage = new sfRenderImage;
    RenderImage->This.Create(Width, Height, DepthBuffer == sfTrue);
    RenderImage->Target      = new sfImage(const_cast<sf::Image*>(&RenderImage->This.GetImage()));
    RenderImage->DefaultView = new sfView(const_cast<sf::View*>(&RenderImage->This.GetDefaultView()));
    RenderImage->CurrentView = RenderImage->DefaultView;

    return RenderImage;
}


////////////////////////////////////////////////////////////
/// Destroy an existing renderimage
////////////////////////////////////////////////////////////
void sfRenderImage_Destroy(sfRenderImage* RenderImage)
{
    delete RenderImage->DefaultView;
    delete RenderImage->Target;
    delete RenderImage;
}


////////////////////////////////////////////////////////////
/// Get the width of the rendering region of a renderimage
////////////////////////////////////////////////////////////
unsigned int sfRenderImage_GetWidth(sfRenderImage* RenderImage)
{
    CSFML_CALL_RETURN(RenderImage, GetWidth(), 0);
}


////////////////////////////////////////////////////////////
/// Get the height of the rendering region of a renderimage
////////////////////////////////////////////////////////////
unsigned int sfRenderImage_GetHeight(sfRenderImage* RenderImage)
{
    CSFML_CALL_RETURN(RenderImage, GetHeight(), 0);
}


////////////////////////////////////////////////////////////
/// Set a renderimage as the current target for rendering
////////////////////////////////////////////////////////////
sfBool sfRenderImage_SetActive(sfRenderImage* RenderImage, sfBool Active)
{
    CSFML_CALL_RETURN(RenderImage, SetActive(Active == sfTrue), sfFalse)
}


////////////////////////////////////////////////////////////
/// Draw something on a renderimage
////////////////////////////////////////////////////////////
void sfRenderImage_DrawPostFX(sfRenderImage* RenderImage, sfPostFX* PostFX)
{
    CSFML_CHECK(PostFX);
    CSFML_CALL(RenderImage, Draw(PostFX->This));
}
void sfRenderImage_DrawShape(sfRenderImage* RenderImage, sfShape* Shape)
{
    CSFML_CHECK(Shape);
    CSFML_CALL(RenderImage, Draw(Shape->This));
}
void sfRenderImage_DrawSprite(sfRenderImage* RenderImage, sfSprite* Sprite)
{
    CSFML_CHECK(Sprite);
    CSFML_CALL(RenderImage, Draw(Sprite->This));
}
void sfRenderImage_DrawString(sfRenderImage* RenderImage, sfString* String)
{
    CSFML_CHECK(String);
    CSFML_CALL(RenderImage, Draw(String->This));
}


////////////////////////////////////////////////////////////
/// Clear the renderimage with the given color
////////////////////////////////////////////////////////////
void sfRenderImage_Clear(sfRenderImage* RenderImage, sfColor Color)
{
    sf::Color SFMLColor(Color.r, Color.g, Color.b, Color.a);

    CSFML_CALL(RenderImage, Clear(SFMLColor));
}


////////////////////////////////////////////////////////////
/// Change the current active view of a renderimage
////////////////////////////////////////////////////////////
void sfRenderImage_SetView(sfRenderImage* RenderImage, sfView* View)
{
    CSFML_CHECK(View);
    CSFML_CALL(RenderImage, SetView(*View->This));
    RenderImage->CurrentView = View;
}


////////////////////////////////////////////////////////////
/// Get the current active view of a renderimage
////////////////////////////////////////////////////////////
const sfView* sfRenderImage_GetView(sfRenderImage* RenderImage)
{
    CSFML_CHECK_RETURN(RenderImage, NULL);

    return RenderImage->CurrentView;
}


////////////////////////////////////////////////////////////
/// Get the default view of a renderimage
////////////////////////////////////////////////////////////
sfView* sfRenderImage_GetDefaultView(sfRenderImage* RenderImage)
{
    CSFML_CHECK_RETURN(RenderImage, NULL);

    return RenderImage->DefaultView;
}


////////////////////////////////////////////////////////////
/// Tell SFML to preserve external OpenGL states, at the expense of
/// more CPU charge. Use this function if you don't want SFML
/// to mess up your own OpenGL states (if any).
/// Don't enable state preservation if not needed, as it will allow
/// SFML to do internal optimizations and improve performances.
/// This parameter is false by default
////////////////////////////////////////////////////////////
void sfRenderImage_PreserveOpenGLStates(sfRenderImage* RenderImage, sfBool Preserve)
{
    CSFML_CALL(RenderImage, PreserveOpenGLStates(Preserve == sfTrue));
}


////////////////////////////////////////////////////////////
/// Get the target image
////////////////////////////////////////////////////////////
sfImage* sfRenderImage_GetImage(sfRenderImage* RenderImage)
{
    CSFML_CHECK_RETURN(RenderImage, NULL);

    return RenderImage->Target;
}


////////////////////////////////////////////////////////////
/// Check whether the system supports render images or not
////////////////////////////////////////////////////////////
sfBool sfRenderImage_CanUseRenderImage()
{
    return sf::RenderImage::CanUseRenderImage();
}
