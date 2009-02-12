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
#include <SFML/Graphics/View.hpp>
#include <algorithm>


namespace sf
{
////////////////////////////////////////////////////////////
/// Construct the view from a rectangle
////////////////////////////////////////////////////////////
View::View(const FloatRect& ViewRect)
{
    SetFromRect(ViewRect);
}


////////////////////////////////////////////////////////////
/// Construct the view from its center and half-size
////////////////////////////////////////////////////////////
View::View(const sf::Vector2f& Center, const sf::Vector2f& HalfSize) :
myCenter    (Center),
myHalfSize  (HalfSize),
myNeedUpdate(true)
{

}


////////////////////////////////////////////////////////////
/// Change the center of the view (take 2 values)
////////////////////////////////////////////////////////////
void View::SetCenter(float X, float Y)
{
    myCenter.x   = X;
    myCenter.y   = Y;
    myNeedUpdate = true;
}


////////////////////////////////////////////////////////////
/// Change the center of the view (take a vector)
////////////////////////////////////////////////////////////
void View::SetCenter(const sf::Vector2f& Center)
{
    SetCenter(Center.x, Center.y);
}


////////////////////////////////////////////////////////////
/// Change the half-size of the view (take 2 values)
////////////////////////////////////////////////////////////
void View::SetHalfSize(float HalfWidth, float HalfHeight)
{
    myHalfSize.x = HalfWidth;
    myHalfSize.y = HalfHeight;
    myNeedUpdate = true;
}


////////////////////////////////////////////////////////////
/// Change the half-size of the view (take a vector)
////////////////////////////////////////////////////////////
void View::SetHalfSize(const sf::Vector2f& HalfSize)
{
    SetHalfSize(HalfSize.x, HalfSize.y);
}


////////////////////////////////////////////////////////////
/// Rebuild the view from a rectangle
////////////////////////////////////////////////////////////
void View::SetFromRect(const FloatRect& ViewRect)
{
    SetCenter(  (ViewRect.Right + ViewRect.Left) / 2, (ViewRect.Bottom + ViewRect.Top) / 2);
    SetHalfSize((ViewRect.Right - ViewRect.Left) / 2, (ViewRect.Bottom - ViewRect.Top) / 2);
}


////////////////////////////////////////////////////////////
/// Get the center of the view
////////////////////////////////////////////////////////////
const sf::Vector2f& View::GetCenter() const
{
    return myCenter;
}


////////////////////////////////////////////////////////////
/// Get the half-size of the view
////////////////////////////////////////////////////////////
const sf::Vector2f& View::GetHalfSize() const
{
    return myHalfSize;
}


////////////////////////////////////////////////////////////
/// Get the bounding rectangle of the view
////////////////////////////////////////////////////////////
const sf::FloatRect& View::GetRect() const
{
    // Recompute it if needed
    if (myNeedUpdate)
        const_cast<View*>(this)->RecomputeMatrix();

    return myRect;
}


////////////////////////////////////////////////////////////
/// Move the view (take 2 values)
////////////////////////////////////////////////////////////
void View::Move(float OffsetX, float OffsetY)
{
    myCenter.x  += OffsetX;
    myCenter.y  += OffsetY;
    myNeedUpdate = true;
}


////////////////////////////////////////////////////////////
/// Move the view (take a vector)
////////////////////////////////////////////////////////////
void View::Move(const sf::Vector2f& Offset)
{
    Move(Offset.x, Offset.y);
}


////////////////////////////////////////////////////////////
/// Resize the view rectangle to simulate a zoom / unzoom effect
////////////////////////////////////////////////////////////
void View::Zoom(float Factor)
{
    if (Factor != 0)
    {
        myHalfSize  /= Factor;
        myNeedUpdate = true;
    }
}


////////////////////////////////////////////////////////////
/// Get the projection matrix of the view
////////////////////////////////////////////////////////////
const Matrix3& View::GetMatrix() const
{
    // Recompute the matrix if needed
    if (myNeedUpdate)
        const_cast<View*>(this)->RecomputeMatrix();

    return myMatrix;
}


////////////////////////////////////////////////////////////
/// Recompute the view rectangle and the projection matrix
////////////////////////////////////////////////////////////
void View::RecomputeMatrix()
{
    // Compute the 4 corners of the view
    float Left   = myCenter.x - myHalfSize.x;
    float Top    = myCenter.y - myHalfSize.y;
    float Right  = myCenter.x + myHalfSize.x;
    float Bottom = myCenter.y + myHalfSize.y;

    // Update the view rectangle - be careful, reversed views are allowed !
    myRect.Left   = std::min(Left, Right);
    myRect.Top    = std::min(Top, Bottom);
    myRect.Right  = std::max(Left, Right);
    myRect.Bottom = std::max(Top, Bottom);

    // Update the projection matrix
    myMatrix(0, 0) = 2.f / (Right - Left);
    myMatrix(1, 1) = 2.f / (Top - Bottom);
    myMatrix(0, 2) = (Left + Right) / (Left - Right);
    myMatrix(1, 2) = (Bottom + Top) / (Bottom - Top);

    myNeedUpdate = false;
}

} // namespace sf
