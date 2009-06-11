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
sfShape* sfShape_Create()
{
    return new sfShape;
}


////////////////////////////////////////////////////////////
/// Create a new shape made of a single line
////////////////////////////////////////////////////////////
sfShape* sfShape_CreateLine(float P1X, float P1Y, float P2X, float P2Y, float Thickness, sfColor Col, float Outline, sfColor OutlineCol)
{
    sf::Color Color(Col.r, Col.g, Col.b, Col.a);
    sf::Color OutlineColor(OutlineCol.r, OutlineCol.g, OutlineCol.b, OutlineCol.a);

    sfShape* Shape = new sfShape;
    Shape->This = sf::Shape::Line(P1X, P1Y, P2X, P2Y, Thickness, Color, Outline, OutlineColor);
    return Shape;
}


////////////////////////////////////////////////////////////
/// Create a new shape made of a single rectangle
////////////////////////////////////////////////////////////
sfShape* sfShape_CreateRectangle(float P1X, float P1Y, float P2X, float P2Y, sfColor Col, float Outline, sfColor OutlineCol)
{
    sf::Color Color(Col.r, Col.g, Col.b, Col.a);
    sf::Color OutlineColor(OutlineCol.r, OutlineCol.g, OutlineCol.b, OutlineCol.a);

    sfShape* Shape = new sfShape;
    Shape->This = sf::Shape::Rectangle(P1X, P1Y, P2X, P2Y, Color, Outline, OutlineColor);
    return Shape;
}


////////////////////////////////////////////////////////////
/// Create a new shape made of a single circle
////////////////////////////////////////////////////////////
sfShape* sfShape_CreateCircle(float X, float Y, float Radius, sfColor Col, float Outline, sfColor OutlineCol)
{
    sf::Color Color(Col.r, Col.g, Col.b, Col.a);
    sf::Color OutlineColor(OutlineCol.r, OutlineCol.g, OutlineCol.b, OutlineCol.a);

    sfShape* Shape = new sfShape;
    Shape->This = sf::Shape::Circle(X, Y, Radius, Color, Outline, OutlineColor);
    return Shape;
}


////////////////////////////////////////////////////////////
/// Destroy an existing shape
////////////////////////////////////////////////////////////
void sfShape_Destroy(sfShape* Shape)
{
    delete Shape;
}


////////////////////////////////////////////////////////////
/// Set the X position of a shape
////////////////////////////////////////////////////////////
void sfShape_SetX(sfShape* Shape, float X)
{
    CSFML_CALL(Shape, SetX(X))
}


////////////////////////////////////////////////////////////
/// Set the Y position of a shape
////////////////////////////////////////////////////////////
void sfShape_SetY(sfShape* Shape, float Y)
{
    CSFML_CALL(Shape, SetY(Y))
}


////////////////////////////////////////////////////////////
/// Set the position of a shape
////////////////////////////////////////////////////////////
void sfShape_SetPosition(sfShape* Shape, float X, float Y)
{
    CSFML_CALL(Shape, SetPosition(sf::Vector2f(X, Y)))
}


////////////////////////////////////////////////////////////
/// Set the horizontal scale of a shape
////////////////////////////////////////////////////////////
void sfShape_SetScaleX(sfShape* Shape, float Scale)
{
    CSFML_CALL(Shape, SetScaleX(Scale))
}


////////////////////////////////////////////////////////////
/// Set the vertical scale of a shape
////////////////////////////////////////////////////////////
void sfShape_SetScaleY(sfShape* Shape, float Scale)
{
    CSFML_CALL(Shape, SetScaleY(Scale))
}


////////////////////////////////////////////////////////////
/// Set the scale of a shape
////////////////////////////////////////////////////////////
void sfShape_SetScale(sfShape* Shape, float ScaleX, float ScaleY)
{
    CSFML_CALL(Shape, SetScale(sf::Vector2f(ScaleX, ScaleY)))
}


////////////////////////////////////////////////////////////
/// Set the orientation of a shape
////////////////////////////////////////////////////////////
void sfShape_SetRotation(sfShape* Shape, float Rotation)
{
    CSFML_CALL(Shape, SetRotation(Rotation))
}


////////////////////////////////////////////////////////////
/// Set the local origin of a shape, in coordinates
/// relative to its left-top corner
////////////////////////////////////////////////////////////
void sfShape_SetOrigin(sfShape* Shape, float X, float Y)
{
    CSFML_CALL(Shape, SetOrigin(sf::Vector2f(X, Y)))
}


////////////////////////////////////////////////////////////
/// Set the color of a shape
////////////////////////////////////////////////////////////
void sfShape_SetColor(sfShape* Shape, sfColor Color)
{
    CSFML_CALL(Shape, SetColor(sf::Color(Color.r, Color.g, Color.b, Color.a)))
}


////////////////////////////////////////////////////////////
/// Set the blending mode for a shape
////////////////////////////////////////////////////////////
void sfShape_SetBlendMode(sfShape* Shape, sfBlendMode Mode)
{
    CSFML_CALL(Shape, SetBlendMode(static_cast<sf::Blend::Mode>(Mode)))
}


////////////////////////////////////////////////////////////
/// Get the X position of a shape
////////////////////////////////////////////////////////////
float sfShape_GetX(sfShape* Shape)
{
    CSFML_CALL_RETURN(Shape, GetPosition().x, 0.f)
}


////////////////////////////////////////////////////////////
/// Get the Y position of a shape
////////////////////////////////////////////////////////////
float sfShape_GetY(sfShape* Shape)
{
    CSFML_CALL_RETURN(Shape, GetPosition().y, 0.f)
}


////////////////////////////////////////////////////////////
/// Get the horizontal scale of a shape
////////////////////////////////////////////////////////////
float sfShape_GetScaleX(sfShape* Shape)
{
    CSFML_CALL_RETURN(Shape, GetScale().x, 0.f)
}


////////////////////////////////////////////////////////////
/// Get the vertical scale of a shape
////////////////////////////////////////////////////////////
float sfShape_GetScaleY(sfShape* Shape)
{
    CSFML_CALL_RETURN(Shape, GetScale().y, 0.f)
}


////////////////////////////////////////////////////////////
/// Get the orientation of a shape
////////////////////////////////////////////////////////////
float sfShape_GetRotation(sfShape* Shape)
{
    CSFML_CALL_RETURN(Shape, GetRotation(), 0.f)
}


////////////////////////////////////////////////////////////
/// Get the X position of the origin a shape
////////////////////////////////////////////////////////////
float sfShape_GetOriginX(sfShape* Shape)
{
    CSFML_CALL_RETURN(Shape, GetOrigin().x, 0.f)
}


////////////////////////////////////////////////////////////
/// Get the Y position of the origin a shape
////////////////////////////////////////////////////////////
float sfShape_GetOriginY(sfShape* Shape)
{
    CSFML_CALL_RETURN(Shape, GetOrigin().y, 0.f)
}


////////////////////////////////////////////////////////////
/// Get the color of a shape
////////////////////////////////////////////////////////////
sfColor sfShape_GetColor(sfShape* Shape)
{
    sfColor Color = {0, 0, 0, 0};
    CSFML_CHECK_RETURN(Shape, Color)

    sf::Color SFMLColor = Shape->This.GetColor();
    return sfColor_FromRGBA(SFMLColor.r, SFMLColor.g, SFMLColor.b, SFMLColor.a);
}


////////////////////////////////////////////////////////////
/// Get the current blending mode of a shape
////////////////////////////////////////////////////////////
sfBlendMode sfShape_GetBlendMode(sfShape* Shape)
{
    CSFML_CHECK_RETURN(Shape, sfBlendNone)

    return static_cast<sfBlendMode>(Shape->This.GetBlendMode());
}


////////////////////////////////////////////////////////////
/// Move a shape
////////////////////////////////////////////////////////////
void sfShape_Move(sfShape* Shape, float OffsetX, float OffsetY)
{
    CSFML_CALL(Shape, Move(sf::Vector2f(OffsetX, OffsetY)))
}


////////////////////////////////////////////////////////////
/// Scale a shape
////////////////////////////////////////////////////////////
void sfShape_Scale(sfShape* Shape, float FactorX, float FactorY)
{
    CSFML_CALL(Shape, Scale(sf::Vector2f(FactorX, FactorY)))
}


////////////////////////////////////////////////////////////
/// Rotate a shape
////////////////////////////////////////////////////////////
void sfShape_Rotate(sfShape* Shape, float Angle)
{
    CSFML_CALL(Shape, Rotate(Angle))
}


////////////////////////////////////////////////////////////
/// Transform a point from global coordinates into the shape's local coordinates
/// (ie it applies the inverse of object's origin, translation, rotation and scale to the point)
////////////////////////////////////////////////////////////
void sfShape_TransformToLocal(sfShape* Shape, float PointX, float PointY, float* X, float* Y)
{
    CSFML_CHECK(Shape)

    sf::Vector2f Point = Shape->This.TransformToLocal(sf::Vector2f(PointX, PointY));
    if (X) *X = Point.x;
    if (Y) *Y = Point.y;
}


////////////////////////////////////////////////////////////
/// Transform a point from the shape's local coordinates into global coordinates
/// (ie it applies the object's origin, translation, rotation and scale to the point)
////////////////////////////////////////////////////////////
void sfShape_TransformToGlobal(sfShape* Shape, float PointX, float PointY, float* X, float* Y)
{
    CSFML_CHECK(Shape)

    sf::Vector2f Point = Shape->This.TransformToGlobal(sf::Vector2f(PointX, PointY));
    if (X) *X = Point.x;
    if (Y) *Y = Point.y;
}


////////////////////////////////////////////////////////////
/// Add a point to a shape
////////////////////////////////////////////////////////////
void sfShape_AddPoint(sfShape* Shape, float X, float Y, sfColor Col, sfColor OutlineCol)
{
    sf::Color Color(Col.r, Col.g, Col.b, Col.a);
    sf::Color OutlineColor(OutlineCol.r, OutlineCol.g, OutlineCol.b, OutlineCol.a);

    CSFML_CALL(Shape, AddPoint(X, Y, Color, OutlineColor))
}


////////////////////////////////////////////////////////////
/// Enable or disable filling a shape.
/// Fill is enabled by default
////////////////////////////////////////////////////////////
void sfShape_EnableFill(sfShape* Shape, sfBool Enable)
{
    CSFML_CALL(Shape, EnableFill(Enable == sfTrue))
}


////////////////////////////////////////////////////////////
/// Enable or disable drawing a shape outline.
/// Outline is enabled by default
////////////////////////////////////////////////////////////
void sfShape_EnableOutline(sfShape* Shape, sfBool Enable)
{
    CSFML_CALL(Shape, EnableOutline(Enable == sfTrue))
}


////////////////////////////////////////////////////////////
/// Change the width of a shape outline
////////////////////////////////////////////////////////////
void sfShape_SetOutlineWidth(sfShape* Shape, float Width)
{
    CSFML_CALL(Shape, SetOutlineWidth(Width))
}


////////////////////////////////////////////////////////////
/// Get the width of a shape outline
////////////////////////////////////////////////////////////
float sfShape_GetOutlineWidth(sfShape* Shape)
{
    CSFML_CALL_RETURN(Shape, GetOutlineWidth(), 0.f)
}


////////////////////////////////////////////////////////////
/// Get the number of points composing a shape
////////////////////////////////////////////////////////////
unsigned int sfShape_GetNbPoints(sfShape* Shape)
{
    CSFML_CALL_RETURN(Shape, GetNbPoints(), 0)
}


////////////////////////////////////////////////////////////
/// Get a point of a shape
////////////////////////////////////////////////////////////
void sfShape_GetPointPosition(sfShape* Shape, unsigned int Index, float* X, float* Y)
{
    CSFML_CHECK(Shape)

    sf::Vector2f Point = Shape->This.GetPointPosition(Index);
    if (X) *X = Point.x;
    if (Y) *Y = Point.y;
}


////////////////////////////////////////////////////////////
/// Get a the color of a shape's point
////////////////////////////////////////////////////////////
sfColor sfShape_GetPointColor(sfShape* Shape, unsigned int Index)
{
    sfColor Color = {255, 255, 255, 255};
    CSFML_CHECK_RETURN(Shape, Color)

    const sf::Color& SFMLColor = Shape->This.GetPointColor(Index);
    Color.r = SFMLColor.r;
    Color.g = SFMLColor.g;
    Color.b = SFMLColor.b;
    Color.a = SFMLColor.a;

    return Color;
}


////////////////////////////////////////////////////////////
/// Get a the outline color of a shape's point
////////////////////////////////////////////////////////////
sfColor sfShape_GetPointOutlineColor(sfShape* Shape, unsigned int Index)
{
    sfColor Color = {255, 255, 255, 255};
    CSFML_CHECK_RETURN(Shape, Color)

    const sf::Color& SFMLColor = Shape->This.GetPointOutlineColor(Index);
    Color.r = SFMLColor.r;
    Color.g = SFMLColor.g;
    Color.b = SFMLColor.b;
    Color.a = SFMLColor.a;

    return Color;
}

////////////////////////////////////////////////////////////
/// Set a the position of a shape's point
////////////////////////////////////////////////////////////
void sfShape_SetPointPosition(sfShape* Shape, unsigned int Index, float X, float Y)
{
    CSFML_CALL(Shape, SetPointPosition(Index, X, Y));
}


////////////////////////////////////////////////////////////
/// Set a the color of a shape's point
////////////////////////////////////////////////////////////
void sfShape_SetPointColor(sfShape* Shape, unsigned int Index, sfColor Color)
{
    CSFML_CALL(Shape, SetPointColor(Index, sf::Color(Color.r, Color.g, Color.b, Color.a)));
}


////////////////////////////////////////////////////////////
/// Set a the outline color of a shape's point
////////////////////////////////////////////////////////////
void sfShape_SetPointOutlineColor(sfShape* Shape, unsigned int Index, sfColor Color)
{
    CSFML_CALL(Shape, SetPointOutlineColor(Index, sf::Color(Color.r, Color.g, Color.b, Color.a)));
}
