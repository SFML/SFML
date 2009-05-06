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
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/GraphicsContext.hpp>
#include <SFML/Window/Window.hpp>
#include <math.h>


namespace sf
{
////////////////////////////////////////////////////////////
/// Default constructor
////////////////////////////////////////////////////////////
Drawable::Drawable(const Vector2f& Position, const Vector2f& Scale, float Rotation, const Color& Col) :
myPosition     (Position),
myScale        (Scale),
myCenter       (0, 0),
myRotation     (Rotation),
myColor        (Col),
myBlendMode    (Blend::Alpha),
myNeedUpdate   (true),
myInvNeedUpdate(true)
{

}


////////////////////////////////////////////////////////////
/// Virtual destructor
////////////////////////////////////////////////////////////
Drawable::~Drawable()
{
    // Nothing to do
}


////////////////////////////////////////////////////////////
/// Set the position of the object (take 2 values)
////////////////////////////////////////////////////////////
void Drawable::SetPosition(float X, float Y)
{
    SetX(X);
    SetY(Y);
}


////////////////////////////////////////////////////////////
/// Set the position of the object (take a 2D vector)
////////////////////////////////////////////////////////////
void Drawable::SetPosition(const Vector2f& Position)
{
    SetX(Position.x);
    SetY(Position.y);
}


////////////////////////////////////////////////////////////
/// Set the X position of the object
////////////////////////////////////////////////////////////
void Drawable::SetX(float X)
{
    myPosition.x    = X;
    myNeedUpdate    = true;
    myInvNeedUpdate = true;
}


////////////////////////////////////////////////////////////
/// Set the Y position of the object
////////////////////////////////////////////////////////////
void Drawable::SetY(float Y)
{
    myPosition.y    = Y;
    myNeedUpdate    = true;
    myInvNeedUpdate = true;
}


////////////////////////////////////////////////////////////
/// Set the scale of the object (take 2 values)
////////////////////////////////////////////////////////////
void Drawable::SetScale(float ScaleX, float ScaleY)
{
    SetScaleX(ScaleX);
    SetScaleY(ScaleY);
}


////////////////////////////////////////////////////////////
/// Set the scale of the object (take a 2D vector)
////////////////////////////////////////////////////////////
void Drawable::SetScale(const Vector2f& Scale)
{
    SetScaleX(Scale.x);
    SetScaleY(Scale.y);
}


////////////////////////////////////////////////////////////
/// Set the X scale factor of the object
////////////////////////////////////////////////////////////
void Drawable::SetScaleX(float FactorX)
{
    if (FactorX > 0)
    {
        myScale.x       = FactorX;
        myNeedUpdate    = true;
        myInvNeedUpdate = true;
    }
}


////////////////////////////////////////////////////////////
/// Set the Y scale factor of the object
////////////////////////////////////////////////////////////
void Drawable::SetScaleY(float FactorY)
{
    if (FactorY > 0)
    {
        myScale.y       = FactorY;
        myNeedUpdate    = true;
        myInvNeedUpdate = true;
    }
}


////////////////////////////////////////////////////////////
/// Set the center of the object, in coordinates relative to the
/// top-left of the object (take 2 values).
/// The default center is (0, 0)
////////////////////////////////////////////////////////////
void Drawable::SetCenter(float CenterX, float CenterY)
{
    myCenter.x      = CenterX;
    myCenter.y      = CenterY;
    myNeedUpdate    = true;
    myInvNeedUpdate = true;
}


////////////////////////////////////////////////////////////
/// Set the center of the object, in coordinates relative to the
/// top-left of the object (take a 2D vector).
/// The default center is (0, 0)
////////////////////////////////////////////////////////////
void Drawable::SetCenter(const Vector2f& Center)
{
    SetCenter(Center.x, Center.y);
}


////////////////////////////////////////////////////////////
/// Set the orientation of the object
////////////////////////////////////////////////////////////
void Drawable::SetRotation(float Rotation)
{
    myRotation = static_cast<float>(fmod(Rotation, 360));
    if (myRotation < 0)
        myRotation += 360.f;
    myNeedUpdate    = true;
    myInvNeedUpdate = true;
}


////////////////////////////////////////////////////////////
/// Set the color of the object.
/// The default color is white
////////////////////////////////////////////////////////////
void Drawable::SetColor(const Color& Col)
{
    myColor = Col;
}


////////////////////////////////////////////////////////////
/// Set the blending mode for the object.
/// The default blend mode is Blend::Alpha
////////////////////////////////////////////////////////////
void Drawable::SetBlendMode(Blend::Mode Mode)
{
    myBlendMode = Mode;
}


////////////////////////////////////////////////////////////
/// Get the position of the object
////////////////////////////////////////////////////////////
const Vector2f& Drawable::GetPosition() const
{
    return myPosition;
}


////////////////////////////////////////////////////////////
/// Get the current scale of the object
////////////////////////////////////////////////////////////
const Vector2f& Drawable::GetScale() const
{
    return myScale;
}


////////////////////////////////////////////////////////////
/// Get the center of the object
////////////////////////////////////////////////////////////
const Vector2f& Drawable::GetCenter() const
{
    return myCenter;
}


////////////////////////////////////////////////////////////
/// Get the orientation of the object
////////////////////////////////////////////////////////////
float Drawable::GetRotation() const
{
    return myRotation;
}


////////////////////////////////////////////////////////////
/// Get the color of the object
////////////////////////////////////////////////////////////
const Color& Drawable::GetColor() const
{
    return myColor;
}


////////////////////////////////////////////////////////////
/// Get the current blending mode
////////////////////////////////////////////////////////////
Blend::Mode Drawable::GetBlendMode() const
{
    return myBlendMode;
}


////////////////////////////////////////////////////////////
/// Move the object of a given offset (take 2 values)
///
////////////////////////////////////////////////////////////
void Drawable::Move(float OffsetX, float OffsetY)
{
    SetX(myPosition.x + OffsetX);
    SetY(myPosition.y + OffsetY);
}


////////////////////////////////////////////////////////////
/// Move the object of a given offset (take a 2D vector)
////////////////////////////////////////////////////////////
void Drawable::Move(const Vector2f& Offset)
{
    Move(Offset.x, Offset.y);
}


////////////////////////////////////////////////////////////
/// Scale the object (take 2 values)
////////////////////////////////////////////////////////////
void Drawable::Scale(float FactorX, float FactorY)
{
    SetScaleX(myScale.x * FactorX);
    SetScaleY(myScale.y * FactorY);
}


////////////////////////////////////////////////////////////
/// Scale the object (take a 2D vector)
////////////////////////////////////////////////////////////
void Drawable::Scale(const Vector2f& Factor)
{
    Scale(Factor.x, Factor.y);
}


////////////////////////////////////////////////////////////
/// Rotate the object
////////////////////////////////////////////////////////////
void Drawable::Rotate(float Angle)
{
    SetRotation(myRotation + Angle);
}


////////////////////////////////////////////////////////////
/// Transform a point from global coordinates into local coordinates
/// (ie it applies the inverse of object's center, translation, rotation and scale to the point)
////////////////////////////////////////////////////////////
sf::Vector2f Drawable::TransformToLocal(const sf::Vector2f& Point) const
{
    return GetInverseMatrix().Transform(Point);
}

////////////////////////////////////////////////////////////
/// Transform a point from local coordinates into global coordinates
/// (ie it applies the object's center, translation, rotation and scale to the point)
////////////////////////////////////////////////////////////
sf::Vector2f Drawable::TransformToGlobal(const sf::Vector2f& Point) const
{
    return GetMatrix().Transform(Point);
}


////////////////////////////////////////////////////////////
/// Get the transform matrix of the drawable
////////////////////////////////////////////////////////////
const Matrix3& Drawable::GetMatrix() const
{
    // First recompute it if needed
    if (myNeedUpdate)
    {
        myMatrix.SetFromTransformations(myCenter, myPosition, myRotation, myScale);
        myNeedUpdate = false;
    }

    return myMatrix;
}


////////////////////////////////////////////////////////////
/// Get the inverse transform matrix of the drawable
////////////////////////////////////////////////////////////
const Matrix3& Drawable::GetInverseMatrix() const
{
    // First recompute it if needed
    if (myInvNeedUpdate)
    {
        myInvMatrix     = GetMatrix().GetInverse();
        myInvNeedUpdate = false;
    }

    return myInvMatrix;
}


////////////////////////////////////////////////////////////
/// Draw the object into the specified window
////////////////////////////////////////////////////////////
void Drawable::Draw(RenderTarget& Target) const
{
    // Save the current modelview matrix and set the new one
    GLCheck(glMatrixMode(GL_MODELVIEW));
    GLCheck(glPushMatrix());
    GLCheck(glMultMatrixf(GetMatrix().Get4x4Elements()));

    // Setup alpha-blending
    if (myBlendMode == Blend::None)
    {
        GLCheck(glDisable(GL_BLEND));
    }
    else
    {
        GLCheck(glEnable(GL_BLEND));

        switch (myBlendMode)
        {
            case Blend::Alpha :    GLCheck(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)); break;
            case Blend::Add :      GLCheck(glBlendFunc(GL_SRC_ALPHA, GL_ONE));                 break;
            case Blend::Multiply : GLCheck(glBlendFunc(GL_DST_COLOR, GL_ZERO));                break;
            default :                                                                          break;
        }
    }

    // Set color
    GLCheck(glColor4f(myColor.r / 255.f, myColor.g / 255.f, myColor.b / 255.f, myColor.a / 255.f));

    // Let the derived class render the object geometry
    Render(Target);

    // Restore the previous modelview matrix
    GLCheck(glMatrixMode(GL_MODELVIEW));
    GLCheck(glPopMatrix());
}

} // namespace sf
