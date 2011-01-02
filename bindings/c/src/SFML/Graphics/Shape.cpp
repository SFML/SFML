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
#include <SFML/Graphics/Shape.h>
#include <SFML/Graphics/ShapeStruct.h>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Internal.h>


////////////////////////////////////////////////////////////
/// Create a new shape
////////////////////////////////////////////////////////////
sfShape* sfShape_Create(void)
{
    return new sfShape;
}


////////////////////////////////////////////////////////////
/// Create a new shape made of a single line
////////////////////////////////////////////////////////////
sfShape* sfShape_CreateLine(float p1x, float p1y, float p2x, float p2y, float thickness, sfColor color, float outline, sfColor outlineColor)
{
    sf::Color SFMLColor(color.r, color.g, color.b, color.a);
    sf::Color SFMLOutlineColor(outlineColor.r, outlineColor.g, outlineColor.b, outlineColor.a);

    sfShape* shape = new sfShape;
    shape->This = sf::Shape::Line(p1x, p1y, p2x, p2y, thickness, SFMLColor, outline, SFMLOutlineColor);
    return shape;
}


////////////////////////////////////////////////////////////
/// Create a new shape made of a single rectangle
////////////////////////////////////////////////////////////
sfShape* sfShape_CreateRectangle(float left, float top, float width, float height, sfColor color, float outline, sfColor outlineColor)
{
    sf::Color SFMLColor(color.r, color.g, color.b, color.a);
    sf::Color SFMLOutlineColor(outlineColor.r, outlineColor.g, outlineColor.b, outlineColor.a);

    sfShape* shape = new sfShape;
    shape->This = sf::Shape::Rectangle(left, top, width, height, SFMLColor, outline, SFMLOutlineColor);
    return shape;
}


////////////////////////////////////////////////////////////
/// Create a new shape made of a single circle
////////////////////////////////////////////////////////////
sfShape* sfShape_CreateCircle(float x, float y, float radius, sfColor color, float outline, sfColor outlineColor)
{
    sf::Color SFMLColor(color.r, color.g, color.b, color.a);
    sf::Color SFMLOutlineColor(outlineColor.r, outlineColor.g, outlineColor.b, outlineColor.a);

    sfShape* shape = new sfShape;
    shape->This = sf::Shape::Circle(x, y, radius, SFMLColor, outline, SFMLOutlineColor);
    return shape;
}


////////////////////////////////////////////////////////////
/// Copy an existing shape
////////////////////////////////////////////////////////////
sfShape* sfShape_Copy(sfShape* shape)
{
    CSFML_CHECK_RETURN(shape, NULL);

    return new sfShape(*shape);
}


////////////////////////////////////////////////////////////
/// Destroy an existing shape
////////////////////////////////////////////////////////////
void sfShape_Destroy(sfShape* shape)
{
    delete shape;
}


////////////////////////////////////////////////////////////
/// Set the X position of a shape
////////////////////////////////////////////////////////////
void sfShape_SetX(sfShape* shape, float x)
{
    CSFML_CALL(shape, SetX(x))
}


////////////////////////////////////////////////////////////
/// Set the Y position of a shape
////////////////////////////////////////////////////////////
void sfShape_SetY(sfShape* shape, float y)
{
    CSFML_CALL(shape, SetY(y))
}


////////////////////////////////////////////////////////////
/// Set the position of a shape
////////////////////////////////////////////////////////////
void sfShape_SetPosition(sfShape* shape, float x, float y)
{
    CSFML_CALL(shape, SetPosition(sf::Vector2f(x, y)))
}


////////////////////////////////////////////////////////////
/// Set the horizontal scale of a shape
////////////////////////////////////////////////////////////
void sfShape_SetScaleX(sfShape* shape, float scale)
{
    CSFML_CALL(shape, SetScaleX(scale))
}


////////////////////////////////////////////////////////////
/// Set the vertical scale of a shape
////////////////////////////////////////////////////////////
void sfShape_SetScaleY(sfShape* shape, float scale)
{
    CSFML_CALL(shape, SetScaleY(scale))
}


////////////////////////////////////////////////////////////
/// Set the scale of a shape
////////////////////////////////////////////////////////////
void sfShape_SetScale(sfShape* shape, float scaleX, float scaleY)
{
    CSFML_CALL(shape, SetScale(sf::Vector2f(scaleX, scaleY)))
}


////////////////////////////////////////////////////////////
/// Set the orientation of a shape
////////////////////////////////////////////////////////////
void sfShape_SetRotation(sfShape* shape, float rotation)
{
    CSFML_CALL(shape, SetRotation(rotation))
}


////////////////////////////////////////////////////////////
/// Set the local origin of a shape, in coordinates
/// relative to its left-top corner
////////////////////////////////////////////////////////////
void sfShape_SetOrigin(sfShape* shape, float x, float y)
{
    CSFML_CALL(shape, SetOrigin(sf::Vector2f(x, y)))
}


////////////////////////////////////////////////////////////
/// Set the color of a shape
////////////////////////////////////////////////////////////
void sfShape_SetColor(sfShape* shape, sfColor color)
{
    CSFML_CALL(shape, SetColor(sf::Color(color.r, color.g, color.b, color.a)))
}


////////////////////////////////////////////////////////////
/// Set the blending mode for a shape
////////////////////////////////////////////////////////////
void sfShape_SetBlendMode(sfShape* shape, sfBlendMode mode)
{
    CSFML_CALL(shape, SetBlendMode(static_cast<sf::Blend::Mode>(mode)))
}


////////////////////////////////////////////////////////////
/// Get the X position of a shape
////////////////////////////////////////////////////////////
float sfShape_GetX(const sfShape* shape)
{
    CSFML_CALL_RETURN(shape, GetPosition().x, 0.f)
}


////////////////////////////////////////////////////////////
/// Get the Y position of a shape
////////////////////////////////////////////////////////////
float sfShape_GetY(const sfShape* shape)
{
    CSFML_CALL_RETURN(shape, GetPosition().y, 0.f)
}


////////////////////////////////////////////////////////////
/// Get the horizontal scale of a shape
////////////////////////////////////////////////////////////
float sfShape_GetScaleX(const sfShape* shape)
{
    CSFML_CALL_RETURN(shape, GetScale().x, 0.f)
}


////////////////////////////////////////////////////////////
/// Get the vertical scale of a shape
////////////////////////////////////////////////////////////
float sfShape_GetScaleY(const sfShape* shape)
{
    CSFML_CALL_RETURN(shape, GetScale().y, 0.f)
}


////////////////////////////////////////////////////////////
/// Get the orientation of a shape
////////////////////////////////////////////////////////////
float sfShape_GetRotation(const sfShape* shape)
{
    CSFML_CALL_RETURN(shape, GetRotation(), 0.f)
}


////////////////////////////////////////////////////////////
/// Get the X position of the origin a shape
////////////////////////////////////////////////////////////
float sfShape_GetOriginX(const sfShape* shape)
{
    CSFML_CALL_RETURN(shape, GetOrigin().x, 0.f)
}


////////////////////////////////////////////////////////////
/// Get the Y position of the origin a shape
////////////////////////////////////////////////////////////
float sfShape_GetOriginY(const sfShape* shape)
{
    CSFML_CALL_RETURN(shape, GetOrigin().y, 0.f)
}


////////////////////////////////////////////////////////////
/// Get the color of a shape
////////////////////////////////////////////////////////////
sfColor sfShape_GetColor(const sfShape* shape)
{
    sfColor color = {0, 0, 0, 0};
    CSFML_CHECK_RETURN(shape, color)

    sf::Color SFMLColor = shape->This.GetColor();
    return sfColor_FromRGBA(SFMLColor.r, SFMLColor.g, SFMLColor.b, SFMLColor.a);
}


////////////////////////////////////////////////////////////
/// Get the current blending mode of a shape
////////////////////////////////////////////////////////////
sfBlendMode sfShape_GetBlendMode(const sfShape* shape)
{
    CSFML_CHECK_RETURN(shape, sfBlendNone)

    return static_cast<sfBlendMode>(shape->This.GetBlendMode());
}


////////////////////////////////////////////////////////////
/// Move a shape
////////////////////////////////////////////////////////////
void sfShape_Move(sfShape* shape, float offsetX, float offsetY)
{
    CSFML_CALL(shape, Move(sf::Vector2f(offsetX, offsetY)))
}


////////////////////////////////////////////////////////////
/// Scale a shape
////////////////////////////////////////////////////////////
void sfShape_Scale(sfShape* shape, float factorX, float factorY)
{
    CSFML_CALL(shape, Scale(sf::Vector2f(factorX, factorY)))
}


////////////////////////////////////////////////////////////
/// Rotate a shape
////////////////////////////////////////////////////////////
void sfShape_Rotate(sfShape* shape, float angle)
{
    CSFML_CALL(shape, Rotate(angle))
}


////////////////////////////////////////////////////////////
/// Transform a point from global coordinates into the shape's local coordinates
/// (ie it applies the inverse of object's origin, translation, rotation and scale to the point)
////////////////////////////////////////////////////////////
void sfShape_TransformToLocal(const sfShape* shape, float pointX, float pointY, float* x, float* y)
{
    CSFML_CHECK(shape)

    sf::Vector2f point = shape->This.TransformToLocal(sf::Vector2f(pointX, pointY));
    if (x) *x = point.x;
    if (y) *y = point.y;
}


////////////////////////////////////////////////////////////
/// Transform a point from the shape's local coordinates into global coordinates
/// (ie it applies the object's origin, translation, rotation and scale to the point)
////////////////////////////////////////////////////////////
void sfShape_TransformToGlobal(const sfShape* shape, float pointX, float pointY, float* x, float* y)
{
    CSFML_CHECK(shape)

    sf::Vector2f point = shape->This.TransformToGlobal(sf::Vector2f(pointX, pointY));
    if (x) *x = point.x;
    if (y) *y = point.y;
}


////////////////////////////////////////////////////////////
/// Add a point to a shape
////////////////////////////////////////////////////////////
void sfShape_AddPoint(sfShape* shape, float x, float y, sfColor color, sfColor outlineColor)
{
    sf::Color SFMLColor(color.r, color.g, color.b, color.a);
    sf::Color SFMLOutlineColor(outlineColor.r, outlineColor.g, outlineColor.b, outlineColor.a);

    CSFML_CALL(shape, AddPoint(x, y, SFMLColor, SFMLOutlineColor))
}


////////////////////////////////////////////////////////////
/// Enable or disable filling a shape.
/// Fill is enabled by default
////////////////////////////////////////////////////////////
void sfShape_EnableFill(sfShape* shape, sfBool enable)
{
    CSFML_CALL(shape, EnableFill(enable == sfTrue))
}


////////////////////////////////////////////////////////////
/// Enable or disable drawing a shape outline.
/// Outline is enabled by default
////////////////////////////////////////////////////////////
void sfShape_EnableOutline(sfShape* shape, sfBool enable)
{
    CSFML_CALL(shape, EnableOutline(enable == sfTrue))
}


////////////////////////////////////////////////////////////
/// Change the thickness of a shape outline
////////////////////////////////////////////////////////////
void sfShape_SetOutlineThickness(sfShape* shape, float thickness)
{
    CSFML_CALL(shape, SetOutlineThickness(thickness))
}


////////////////////////////////////////////////////////////
/// Get the thickness of a shape outline
////////////////////////////////////////////////////////////
float sfShape_GetOutlineThickness(const sfShape* shape)
{
    CSFML_CALL_RETURN(shape, GetOutlineThickness(), 0.f)
}


////////////////////////////////////////////////////////////
/// Get the number of points composing a shape
////////////////////////////////////////////////////////////
unsigned int sfShape_GetPointsCount(const sfShape* shape)
{
    CSFML_CALL_RETURN(shape, GetPointsCount(), 0)
}


////////////////////////////////////////////////////////////
/// Get a point of a shape
////////////////////////////////////////////////////////////
void sfShape_GetPointPosition(const sfShape* shape, unsigned int index, float* x, float* y)
{
    CSFML_CHECK(shape)

    sf::Vector2f point = shape->This.GetPointPosition(index);
    if (x) *x = point.x;
    if (y) *y = point.y;
}


////////////////////////////////////////////////////////////
/// Get a the color of a shape's point
////////////////////////////////////////////////////////////
sfColor sfShape_GetPointColor(const sfShape* shape, unsigned int index)
{
    sfColor color = {255, 255, 255, 255};
    CSFML_CHECK_RETURN(shape, color)

    const sf::Color& SFMLColor = shape->This.GetPointColor(index);
    color.r = SFMLColor.r;
    color.g = SFMLColor.g;
    color.b = SFMLColor.b;
    color.a = SFMLColor.a;

    return color;
}


////////////////////////////////////////////////////////////
/// Get a the outline color of a shape's point
////////////////////////////////////////////////////////////
sfColor sfShape_GetPointOutlineColor(const sfShape* shape, unsigned int index)
{
    sfColor color = {255, 255, 255, 255};
    CSFML_CHECK_RETURN(shape, color)

    const sf::Color& SFMLColor = shape->This.GetPointOutlineColor(index);
    color.r = SFMLColor.r;
    color.g = SFMLColor.g;
    color.b = SFMLColor.b;
    color.a = SFMLColor.a;

    return color;
}

////////////////////////////////////////////////////////////
/// Set a the position of a shape's point
////////////////////////////////////////////////////////////
void sfShape_SetPointPosition(sfShape* shape, unsigned int index, float x, float y)
{
    CSFML_CALL(shape, SetPointPosition(index, x, y));
}


////////////////////////////////////////////////////////////
/// Set a the color of a shape's point
////////////////////////////////////////////////////////////
void sfShape_SetPointColor(sfShape* shape, unsigned int index, sfColor color)
{
    CSFML_CALL(shape, SetPointColor(index, sf::Color(color.r, color.g, color.b, color.a)));
}


////////////////////////////////////////////////////////////
/// Set a the outline color of a shape's point
////////////////////////////////////////////////////////////
void sfShape_SetPointOutlineColor(sfShape* shape, unsigned int index, sfColor color)
{
    CSFML_CALL(shape, SetPointOutlineColor(index, sf::Color(color.r, color.g, color.b, color.a)));
}
