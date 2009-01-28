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

#ifndef SFML_Shape_H
#define SFML_Shape_H

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Config.h>
#include <SFML/Graphics/BlendMode.h>
#include <SFML/Graphics/Color.h>


////////////////////////////////////////////////////////////
/// sfShape defines a drawable convex Shape ; it also defines
/// helper functions to draw simple Shapes like
/// lines, rectangles, circles, etc.
////////////////////////////////////////////////////////////
typedef struct sfShape sfShape;


////////////////////////////////////////////////////////////
/// Create a new shape
///
/// \return A new sfShape object, or NULL if it failed
///
////////////////////////////////////////////////////////////
CSFML_API sfShape* sfShape_Create();

////////////////////////////////////////////////////////////
/// Create a new shape made of a single line
///
/// \param P1X, P1Y :   Position of the first point
/// \param P2X, P2Y :   Position second point
/// \param Thickness :  Line thickness
/// \param Col :        Color used to draw the line
/// \param Outline :    Outline width
/// \param OutlineCol : Color used to draw the outline
///
////////////////////////////////////////////////////////////
CSFML_API sfShape* sfShape_CreateLine(float P1X, float P1Y, float P2X, float P2Y, float Thickness, sfColor Col, float Outline, sfColor OutlineCol);

////////////////////////////////////////////////////////////
/// Create a new shape made of a single rectangle
///
/// \param P1X, P1Y :   Position of the first point
/// \param P2X, P2Y :   Position second point
/// \param Col :        Color used to fill the rectangle
/// \param Outline :    Outline width
/// \param OutlineCol : Color used to draw the outline
///
////////////////////////////////////////////////////////////
CSFML_API sfShape* sfShape_CreateRectangle(float P1X, float P1Y, float P2X, float P2Y, sfColor Col, float Outline, sfColor OutlineCol);

////////////////////////////////////////////////////////////
/// Create a new shape made of a single circle
///
/// \param X, Y :       Position of the center
/// \param Radius :     Radius
/// \param Col :        Color used to fill the circle
/// \param Outline :    Outline width
/// \param OutlineCol : Color used to draw the outline
///
////////////////////////////////////////////////////////////
CSFML_API sfShape* sfShape_CreateCircle(float X, float Y, float Radius, sfColor Col, float Outline, sfColor OutlineCol);

////////////////////////////////////////////////////////////
/// Destroy an existing Shape
///
/// \param Shape : Shape to delete
///
////////////////////////////////////////////////////////////
CSFML_API void sfShape_Destroy(sfShape* Shape);

////////////////////////////////////////////////////////////
/// Set the X position of a shape
///
/// \param Shape : Shape to modify
/// \param X :     New X coordinate
///
////////////////////////////////////////////////////////////
CSFML_API void sfShape_SetX(sfShape* Shape, float X);

////////////////////////////////////////////////////////////
/// Set the Y position of a shape
///
/// \param Shape : Shape to modify
/// \param Y :     New Y coordinate
///
////////////////////////////////////////////////////////////
CSFML_API void sfShape_SetY(sfShape* Shape, float Y);

////////////////////////////////////////////////////////////
/// Set the position of a shape
///
/// \param Shape : Shape to modify
/// \param X :     New X coordinate
/// \param Y :     New Y coordinate
///
////////////////////////////////////////////////////////////
CSFML_API void sfShape_SetPosition(sfShape* Shape, float X, float Y);

////////////////////////////////////////////////////////////
/// Set the horizontal scale of a shape
///
/// \param Shape : Shape to modify
/// \param Scale : New scale (must be strictly positive)
///
////////////////////////////////////////////////////////////
CSFML_API void sfShape_SetScaleX(sfShape* Shape, float Scale);

////////////////////////////////////////////////////////////
/// Set the vertical scale of a shape
///
/// \param Shape : Shape to modify
/// \param Scale : New scale (must be strictly positive)
///
////////////////////////////////////////////////////////////
CSFML_API void sfShape_SetScaleY(sfShape* Shape, float Scale);

////////////////////////////////////////////////////////////
/// Set the scale of a shape
///
/// \param Shape :  Shape to modify
/// \param ScaleX : New horizontal scale (must be strictly positive)
/// \param ScaleY : New vertical scale (must be strictly positive)
///
////////////////////////////////////////////////////////////
CSFML_API void sfShape_SetScale(sfShape* Shape, float ScaleX, float ScaleY);

////////////////////////////////////////////////////////////
/// Set the orientation of a shape
///
/// \param Shape :    Shape to modify
/// \param Rotation : Angle of rotation, in degrees
///
////////////////////////////////////////////////////////////
CSFML_API void sfShape_SetRotation(sfShape* Shape, float Rotation);

////////////////////////////////////////////////////////////
/// Set the center of a shape, in coordinates relative to
/// its left-top corner
///
/// \param Shape : Shape to modify
/// \param X :     X coordinate of the center
/// \param Y :     Y coordinate of the center
///
////////////////////////////////////////////////////////////
CSFML_API void sfShape_SetCenter(sfShape* Shape, float X, float Y);

////////////////////////////////////////////////////////////
/// Set the color of a shape
///
/// \param Shape : Shape to modify
/// \param Color : New color
///
////////////////////////////////////////////////////////////
CSFML_API void sfShape_SetColor(sfShape* Shape, sfColor Color);

////////////////////////////////////////////////////////////
/// Set the blending mode for a shape
///
/// \param Shape : Shape to modify
/// \param Mode :  New blending mode
///
////////////////////////////////////////////////////////////
CSFML_API void sfShape_SetBlendMode(sfShape* Shape, sfBlendMode Mode);

////////////////////////////////////////////////////////////
/// Get the X position of a shape
///
/// \param Shape : Shape to read
///
/// \return Current X position
///
////////////////////////////////////////////////////////////
CSFML_API float sfShape_GetX(sfShape* Shape);

////////////////////////////////////////////////////////////
/// Get the Y position of a shape
///
/// \param Shape : Shape to read
///
/// \return Current Y position
///
////////////////////////////////////////////////////////////
CSFML_API float sfShape_GetY(sfShape* Shape);

////////////////////////////////////////////////////////////
/// Get the horizontal scale of a shape
///
/// \param Shape : Shape to read
///
/// \return Current X scale factor (always positive)
///
////////////////////////////////////////////////////////////
CSFML_API float sfShape_GetScaleX(sfShape* Shape);

////////////////////////////////////////////////////////////
/// Get the vertical scale of a shape
///
/// \param Shape : Shape to read
///
/// \return Current Y scale factor (always positive)
///
////////////////////////////////////////////////////////////
CSFML_API float sfShape_GetScaleY(sfShape* Shape);

////////////////////////////////////////////////////////////
/// Get the orientation of a shape
///
/// \param Shape : Shape to read
///
/// \return Current rotation, in degrees
///
////////////////////////////////////////////////////////////
CSFML_API float sfShape_GetRotation(sfShape* Shape);

////////////////////////////////////////////////////////////
/// Get the X position of the center a shape
///
/// \param Shape : Shape to read
///
/// \return Current X center
///
////////////////////////////////////////////////////////////
CSFML_API float sfShape_GetCenterX(sfShape* Shape);

////////////////////////////////////////////////////////////
/// Get the Y position of the center a shape
///
/// \param Shape : Shape to read
///
/// \return Current Y center
///
////////////////////////////////////////////////////////////
CSFML_API float sfShape_GetCenterY(sfShape* Shape);

////////////////////////////////////////////////////////////
/// Get the color of a shape
///
/// \param Shape : Shape to read
///
/// \return Current color
///
////////////////////////////////////////////////////////////
CSFML_API sfColor sfShape_GetColor(sfShape* Shape);

////////////////////////////////////////////////////////////
/// Get the current blending mode of a shape
///
/// \param Shape : Shape to read
///
/// \return Current blending mode
///
////////////////////////////////////////////////////////////
CSFML_API sfBlendMode sfShape_GetBlendMode(sfShape* Shape);

////////////////////////////////////////////////////////////
/// Move a shape
///
/// \param Shape :   Shape to modify
/// \param OffsetX : Offset on the X axis
/// \param OffsetY : Offset on the Y axis
///
////////////////////////////////////////////////////////////
CSFML_API void sfShape_Move(sfShape* Shape, float OffsetX, float OffsetY);

////////////////////////////////////////////////////////////
/// Scale a shape
///
/// \param Shape :   Shape to modify
/// \param FactorX : Horizontal scaling factor (must be strictly positive)
/// \param FactorY : Vertical scaling factor (must be strictly positive)
///
////////////////////////////////////////////////////////////
CSFML_API void sfShape_Scale(sfShape* Shape, float FactorX, float FactorY);

////////////////////////////////////////////////////////////
/// Rotate a shape
///
/// \param Shape : Shape to modify
/// \param Angle : Angle of rotation, in degrees
///
////////////////////////////////////////////////////////////
CSFML_API void sfShape_Rotate(sfShape* Shape, float Angle);

////////////////////////////////////////////////////////////
/// Transform a point from global coordinates into the shape's local coordinates
/// (ie it applies the inverse of object's center, translation, rotation and scale to the point)
///
/// \param Shape :  Shape object
/// \param PointX : X coordinate of the point to transform
/// \param PointY : Y coordinate of the point to transform
/// \param X :      Value to fill with the X coordinate of the converted point
/// \param Y :      Value to fill with the y coordinate of the converted point
///
////////////////////////////////////////////////////////////
CSFML_API void sfShape_TransformToLocal(sfShape* Shape, float PointX, float PointY, float* X, float* Y);

////////////////////////////////////////////////////////////
/// Transform a point from the shape's local coordinates into global coordinates
/// (ie it applies the object's center, translation, rotation and scale to the point)
///
/// \param Shape :  Shape object
/// \param PointX : X coordinate of the point to transform
/// \param PointY : Y coordinate of the point to transform
/// \param X :      Value to fill with the X coordinate of the converted point
/// \param Y :      Value to fill with the y coordinate of the converted point
///
////////////////////////////////////////////////////////////
CSFML_API void sfShape_TransformToGlobal(sfShape* Shape, float PointX, float PointY, float* X, float* Y);

////////////////////////////////////////////////////////////
/// Add a point to a shape
///
/// \param Shape :      Shape to modify
/// \param X, Y :       Position of the point
/// \param Col :        Color of the point
/// \param OutlineCol : Outline color of the point
///
////////////////////////////////////////////////////////////
CSFML_API void sfShape_AddPoint(sfShape* Shape, float X, float Y, sfColor Col, sfColor OutlineCol);

////////////////////////////////////////////////////////////
/// Enable or disable filling a shape.
/// Fill is enabled by default
///
/// \param Enable : True to enable, false to disable
///
////////////////////////////////////////////////////////////
CSFML_API void sfShape_EnableFill(sfShape* Shape, sfBool Enable);

////////////////////////////////////////////////////////////
/// Enable or disable drawing a shape outline.
/// Outline is enabled by default
///
/// \param Shape :  Shape to modify
/// \param Enable : True to enable, false to disable
///
////////////////////////////////////////////////////////////
CSFML_API void sfShape_EnableOutline(sfShape* Shape, sfBool Enable);

////////////////////////////////////////////////////////////
/// Change the width of a shape outline
///
/// \param Shape : Shape to modify
/// \param Width : New width
///
////////////////////////////////////////////////////////////
CSFML_API void sfShape_SetOutlineWidth(sfShape* Shape, float Width);

////////////////////////////////////////////////////////////
/// Get the width of a shape outline
///
/// \param Shape : Shape to read
///
/// \param return Current outline width
///
////////////////////////////////////////////////////////////
CSFML_API float sfShape_GetOutlineWidth(sfShape* Shape);

////////////////////////////////////////////////////////////
/// Get the number of points composing a shape
///
/// \param Shape : Shape to read
///
/// \return Total number of points
///
////////////////////////////////////////////////////////////
CSFML_API unsigned int sfShape_GetNbPoints(sfShape* Shape);

////////////////////////////////////////////////////////////
/// Get a the position of a shape's point
///
/// \param Shape : Shape to read
/// \param Index : Index of the point to get
/// \param X :     Variable to fill with the X coordinate of the point
/// \param Y :     Variable to fill with the Y coordinate of the point
///
////////////////////////////////////////////////////////////
CSFML_API void sfShape_GetPointPosition(sfShape* Shape, unsigned int Index, float* X, float* Y);

////////////////////////////////////////////////////////////
/// Get a the color of a shape's point
///
/// \param Shape : Shape to read
/// \param Index : Index of the point to get
///
/// \return Color of the point
///
////////////////////////////////////////////////////////////
CSFML_API sfColor sfShape_GetPointColor(sfShape* Shape, unsigned int Index);

////////////////////////////////////////////////////////////
/// Get a the outline color of a shape's point
///
/// \param Shape : Shape to read
/// \param Index : Index of the point to get
///
/// \return Outline color of the point
///
////////////////////////////////////////////////////////////
CSFML_API sfColor sfShape_GetPointOutlineColor(sfShape* Shape, unsigned int Index);

////////////////////////////////////////////////////////////
/// Set a the position of a shape's point
///
/// \param Shape : Shape to read
/// \param Index : Index of the point to get
/// \param X :     X coordinate of the point
/// \param Y :     Y coordinate of the point
///
////////////////////////////////////////////////////////////
CSFML_API void sfShape_SetPointPosition(sfShape* Shape, unsigned int Index, float X, float Y);

////////////////////////////////////////////////////////////
/// Set a the color of a shape's point
///
/// \param Shape : Shape to read
/// \param Index : Index of the point to get
/// \param Color : Color of the point
///
////////////////////////////////////////////////////////////
CSFML_API void sfShape_SetPointColor(sfShape* Shape, unsigned int Index, sfColor Color);

////////////////////////////////////////////////////////////
/// Set a the outline color of a shape's point
///
/// \param Shape : Shape to read
/// \param Index : Index of the point to get
/// \param Color : Outline color of the point
///
////////////////////////////////////////////////////////////
CSFML_API void sfShape_SetPointOutlineColor(sfShape* Shape, unsigned int Index, sfColor Color);


#endif // SFML_Shape_H
