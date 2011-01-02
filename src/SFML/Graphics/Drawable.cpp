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
#include <SFML/Graphics/Renderer.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <cmath>


namespace sf
{
////////////////////////////////////////////////////////////
Drawable::Drawable(const Vector2f& position, const Vector2f& scale, float rotation, const Color& color) :
myPosition        (position),
myScale           (scale),
myOrigin          (0, 0),
myRotation        (rotation),
myColor           (color),
myBlendMode       (Blend::Alpha),
myMatrixUpdated   (false),
myInvMatrixUpdated(false)
{
}


////////////////////////////////////////////////////////////
Drawable::~Drawable()
{
    // Nothing to do
}


////////////////////////////////////////////////////////////
void Drawable::SetPosition(float x, float y)
{
    SetX(x);
    SetY(y);
}


////////////////////////////////////////////////////////////
void Drawable::SetPosition(const Vector2f& position)
{
    SetX(position.x);
    SetY(position.y);
}


////////////////////////////////////////////////////////////
void Drawable::SetX(float x)
{
    myPosition.x = x;

    myMatrixUpdated    = false;
    myInvMatrixUpdated = false;
}


////////////////////////////////////////////////////////////
void Drawable::SetY(float y)
{
    myPosition.y = y;

    myMatrixUpdated    = false;
    myInvMatrixUpdated = false;
}


////////////////////////////////////////////////////////////
void Drawable::SetScale(float factorX, float factorY)
{
    SetScaleX(factorX);
    SetScaleY(factorY);
}


////////////////////////////////////////////////////////////
void Drawable::SetScale(const Vector2f& factors)
{
    SetScaleX(factors.x);
    SetScaleY(factors.y);
}


////////////////////////////////////////////////////////////
void Drawable::SetScaleX(float factor)
{
    if (factor > 0)
    {
        myScale.x = factor;

        myMatrixUpdated    = false;
        myInvMatrixUpdated = false;
    }
}


////////////////////////////////////////////////////////////
void Drawable::SetScaleY(float factor)
{
    if (factor > 0)
    {
        myScale.y = factor;

        myMatrixUpdated    = false;
        myInvMatrixUpdated = false;
    }
}


////////////////////////////////////////////////////////////
void Drawable::SetOrigin(float x, float y)
{
    myOrigin.x = x;
    myOrigin.y = y;

    myMatrixUpdated    = false;
    myInvMatrixUpdated = false;
}


////////////////////////////////////////////////////////////
void Drawable::SetOrigin(const Vector2f& origin)
{
    SetOrigin(origin.x, origin.y);
}


////////////////////////////////////////////////////////////
void Drawable::SetRotation(float angle)
{
    myRotation = static_cast<float>(std::fmod(angle, 360));
    if (myRotation < 0)
        myRotation += 360.f;

    myMatrixUpdated    = false;
    myInvMatrixUpdated = false;
}


////////////////////////////////////////////////////////////
void Drawable::SetColor(const Color& color)
{
    myColor = color;
}


////////////////////////////////////////////////////////////
void Drawable::SetBlendMode(Blend::Mode mode)
{
    myBlendMode = mode;
}


////////////////////////////////////////////////////////////
const Vector2f& Drawable::GetPosition() const
{
    return myPosition;
}


////////////////////////////////////////////////////////////
const Vector2f& Drawable::GetScale() const
{
    return myScale;
}


////////////////////////////////////////////////////////////
const Vector2f& Drawable::GetOrigin() const
{
    return myOrigin;
}


////////////////////////////////////////////////////////////
float Drawable::GetRotation() const
{
    return myRotation;
}


////////////////////////////////////////////////////////////
const Color& Drawable::GetColor() const
{
    return myColor;
}


////////////////////////////////////////////////////////////
Blend::Mode Drawable::GetBlendMode() const
{
    return myBlendMode;
}


////////////////////////////////////////////////////////////
void Drawable::Move(float offsetX, float offsetY)
{
    SetPosition(myPosition.x + offsetX, myPosition.y + offsetY);
}


////////////////////////////////////////////////////////////
void Drawable::Move(const Vector2f& offset)
{
    SetPosition(myPosition + offset);
}


////////////////////////////////////////////////////////////
void Drawable::Scale(float factorX, float factorY)
{
    SetScale(myScale.x * factorX, myScale.y * factorY);
}


////////////////////////////////////////////////////////////
void Drawable::Scale(const Vector2f& factor)
{
    SetScale(myScale.x * factor.x, myScale.y * factor.y);
}


////////////////////////////////////////////////////////////
void Drawable::Rotate(float angle)
{
    SetRotation(myRotation + angle);
}


////////////////////////////////////////////////////////////
Vector2f Drawable::TransformToLocal(const Vector2f& point) const
{
    return GetInverseMatrix().Transform(point);
}


////////////////////////////////////////////////////////////
Vector2f Drawable::TransformToGlobal(const Vector2f& point) const
{
    return GetMatrix().Transform(point);
}


////////////////////////////////////////////////////////////
const Matrix3& Drawable::GetMatrix() const
{
    // First recompute it if needed
    if (!myMatrixUpdated)
    {
        myMatrix = Matrix3::Transformation(myOrigin, myPosition, myRotation, myScale);
        myMatrixUpdated = true;
    }

    return myMatrix;
}


////////////////////////////////////////////////////////////
const Matrix3& Drawable::GetInverseMatrix() const
{
    // First recompute it if needed
    if (!myInvMatrixUpdated)
    {
        myInvMatrix = GetMatrix().GetInverse();
        myInvMatrixUpdated = true;
    }

    return myInvMatrix;
}


////////////////////////////////////////////////////////////
void Drawable::Draw(RenderTarget& target, Renderer& renderer) const
{
    // Set the current model-view matrix
    renderer.ApplyModelView(GetMatrix());

    // Set the current global color
    renderer.ApplyColor(myColor);

    // Set the current alpha-blending mode
    renderer.SetBlendMode(myBlendMode);

    // Let the derived class render the object geometry
    Render(target, renderer);
}

} // namespace sf
