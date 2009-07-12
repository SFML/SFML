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
#include <SFML/Graphics/GLCheck.hpp>
#include <SFML/Window/Window.hpp>
#include <math.h>


namespace sf
{
////////////////////////////////////////////////////////////
/// Default constructor
////////////////////////////////////////////////////////////
Drawable::Drawable(const Vector2f& position, const Vector2f& scale, float rotation, const Color& color) :
myPosition     (position),
myScale        (scale),
myOrigin       (0, 0),
myRotation     (rotation),
myColor        (color),
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
void Drawable::SetPosition(float x, float y)
{
    SetX(x);
    SetY(y);
}


////////////////////////////////////////////////////////////
/// Set the position of the object (take a 2D vector)
////////////////////////////////////////////////////////////
void Drawable::SetPosition(const Vector2f& position)
{
    SetX(position.x);
    SetY(position.y);
}


////////////////////////////////////////////////////////////
/// Set the X position of the object
////////////////////////////////////////////////////////////
void Drawable::SetX(float x)
{
    myPosition.x    = x;
    myNeedUpdate    = true;
    myInvNeedUpdate = true;
}


////////////////////////////////////////////////////////////
/// Set the Y position of the object
////////////////////////////////////////////////////////////
void Drawable::SetY(float y)
{
    myPosition.y    = y;
    myNeedUpdate    = true;
    myInvNeedUpdate = true;
}


////////////////////////////////////////////////////////////
/// Set the scale of the object (take 2 values)
////////////////////////////////////////////////////////////
void Drawable::SetScale(float factorX, float factorY)
{
    SetScaleX(factorX);
    SetScaleY(factorY);
}


////////////////////////////////////////////////////////////
/// Set the scale of the object (take a 2D vector)
////////////////////////////////////////////////////////////
void Drawable::SetScale(const Vector2f& scale)
{
    SetScaleX(scale.x);
    SetScaleY(scale.y);
}


////////////////////////////////////////////////////////////
/// Set the X scale factor of the object
////////////////////////////////////////////////////////////
void Drawable::SetScaleX(float factor)
{
    if (factor > 0)
    {
        myScale.x       = factor;
        myNeedUpdate    = true;
        myInvNeedUpdate = true;
    }
}


////////////////////////////////////////////////////////////
/// Set the Y scale factor of the object
////////////////////////////////////////////////////////////
void Drawable::SetScaleY(float factor)
{
    if (factor > 0)
    {
        myScale.y       = factor;
        myNeedUpdate    = true;
        myInvNeedUpdate = true;
    }
}


////////////////////////////////////////////////////////////
/// Set the local origin of the object, in coordinates relative to the
/// top-left of the object (take 2 values).
/// The default origin is (0, 0)
////////////////////////////////////////////////////////////
void Drawable::SetOrigin(float x, float y)
{
    myOrigin.x      = x;
    myOrigin.y      = y;
    myNeedUpdate    = true;
    myInvNeedUpdate = true;
}


////////////////////////////////////////////////////////////
/// Set the local origin of the object, in coordinates relative to the
/// top-left of the object (take a 2D vector).
/// The default origin is (0, 0)
////////////////////////////////////////////////////////////
void Drawable::SetOrigin(const Vector2f& origin)
{
    SetOrigin(origin.x, origin.y);
}


////////////////////////////////////////////////////////////
/// Set the orientation of the object
////////////////////////////////////////////////////////////
void Drawable::SetRotation(float angle)
{
    myRotation = static_cast<float>(fmod(angle, 360));
    if (myRotation < 0)
        myRotation += 360.f;
    myNeedUpdate    = true;
    myInvNeedUpdate = true;
}


////////////////////////////////////////////////////////////
/// Set the color of the object.
/// The default color is white
////////////////////////////////////////////////////////////
void Drawable::SetColor(const Color& color)
{
    myColor = color;
}


////////////////////////////////////////////////////////////
/// Set the blending mode for the object.
/// The default blend mode is Blend::Alpha
////////////////////////////////////////////////////////////
void Drawable::SetBlendMode(Blend::Mode mode)
{
    myBlendMode = mode;
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
/// Get the origin of the object
////////////////////////////////////////////////////////////
const Vector2f& Drawable::GetOrigin() const
{
    return myOrigin;
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
void Drawable::Move(float offsetX, float offsetY)
{
    SetPosition(myPosition.x + offsetX, myPosition.y + offsetY);
}


////////////////////////////////////////////////////////////
/// Move the object of a given offset (take a 2D vector)
////////////////////////////////////////////////////////////
void Drawable::Move(const Vector2f& offset)
{
    SetPosition(myPosition + offset);
}


////////////////////////////////////////////////////////////
/// Scale the object (take 2 values)
////////////////////////////////////////////////////////////
void Drawable::Scale(float factorX, float factorY)
{
    SetScale(myScale.x * factorX, myScale.y * factorY);
}


////////////////////////////////////////////////////////////
/// Scale the object (take a 2D vector)
////////////////////////////////////////////////////////////
void Drawable::Scale(const Vector2f& factor)
{
    SetScale(myScale.x * factor.x, myScale.y * factor.y);
}


////////////////////////////////////////////////////////////
/// Rotate the object
////////////////////////////////////////////////////////////
void Drawable::Rotate(float angle)
{
    SetRotation(myRotation + angle);
}


////////////////////////////////////////////////////////////
/// Transform a point from global coordinates into local coordinates
/// (ie it applies the inverse of object's origin, translation, rotation and scale to the point)
////////////////////////////////////////////////////////////
sf::Vector2f Drawable::TransformToLocal(const sf::Vector2f& point) const
{
    return GetInverseMatrix().Transform(point);
}

////////////////////////////////////////////////////////////
/// Transform a point from local coordinates into global coordinates
/// (ie it applies the object's origin, translation, rotation and scale to the point)
////////////////////////////////////////////////////////////
sf::Vector2f Drawable::TransformToGlobal(const sf::Vector2f& point) const
{
    return GetMatrix().Transform(point);
}


////////////////////////////////////////////////////////////
/// Get the transform matrix of the drawable
////////////////////////////////////////////////////////////
const Matrix3& Drawable::GetMatrix() const
{
    // First recompute it if needed
    if (myNeedUpdate)
    {
        myMatrix.SetFromTransformations(myOrigin, myPosition, myRotation, myScale);
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
void Drawable::Draw(RenderTarget& target) const
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

        // @todo the resulting alpha may not be correct, which matters when target is a RenderImage.
        //       find a fix for this (glBlendFuncSeparate -- but not supported by every graphics card)
        switch (myBlendMode)
        {
            case Blend::Alpha :    GLCheck(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)); break;
            case Blend::Add :      GLCheck(glBlendFunc(GL_SRC_ALPHA, GL_ONE)); break;
            case Blend::Multiply : GLCheck(glBlendFunc(GL_DST_COLOR, GL_ZERO)); break;
            default : break;
        }
    }

    // Set color
    GLCheck(glColor4ub(myColor.r, myColor.g, myColor.b, myColor.a));

    // Let the derived class render the object geometry
    Render(target);

    // Restore the previous modelview matrix
    GLCheck(glMatrixMode(GL_MODELVIEW));
    GLCheck(glPopMatrix());
}

} // namespace sf
