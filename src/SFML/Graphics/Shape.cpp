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
#include <SFML/Graphics/Shape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Err.hpp>
#include <cmath>


////////////////////////////////////////////////////////////
// Private data
////////////////////////////////////////////////////////////
namespace
{
    // Compute the normal of a segment
    sf::Vector2f ComputeNormal(const sf::Vector2f& p1, const sf::Vector2f& p2)
    {
        sf::Vector2f normal(p1.y - p2.y, p2.x - p1.x);
        float length = std::sqrt(normal.x * normal.x + normal.y * normal.y);
        if (length != 0.f)
            normal /= length;
        return normal;
    }
}


namespace sf
{
////////////////////////////////////////////////////////////
Shape::~Shape()
{
}


////////////////////////////////////////////////////////////
void Shape::SetTexture(const Texture* texture, bool resetRect)
{
    // Recompute the texture area if requested, or if there was no texture before
    if (texture && (resetRect || !myTexture))
        SetTextureRect(IntRect(0, 0, texture->GetWidth(), texture->GetHeight()));

    // Assign the new texture
    myTexture = texture;
}


////////////////////////////////////////////////////////////
const Texture* Shape::GetTexture() const
{
    return myTexture;
}


////////////////////////////////////////////////////////////
void Shape::SetTextureRect(const IntRect& rect)
{
    myTextureRect = rect;
    UpdateTexCoords();
}


////////////////////////////////////////////////////////////
const IntRect& Shape::GetTextureRect() const
{
    return myTextureRect;
}


////////////////////////////////////////////////////////////
void Shape::SetFillColor(const Color& color)
{
    myFillColor = color;
    UpdateFillColors();
}


////////////////////////////////////////////////////////////
const Color& Shape::GetFillColor() const
{
    return myFillColor;
}


////////////////////////////////////////////////////////////
void Shape::SetOutlineColor(const Color& color)
{
    myOutlineColor = color;
    UpdateOutlineColors();
}


////////////////////////////////////////////////////////////
const Color& Shape::GetOutlineColor() const
{
    return myOutlineColor;
}


////////////////////////////////////////////////////////////
void Shape::SetOutlineThickness(float thickness)
{
    myOutlineThickness = thickness;
    Update(); // recompute everything because the whole shape must be offset
}


////////////////////////////////////////////////////////////
float Shape::GetOutlineThickness() const
{
    return myOutlineThickness;
}


////////////////////////////////////////////////////////////
FloatRect Shape::GetLocalBounds() const
{
    return myBounds;
}


////////////////////////////////////////////////////////////
FloatRect Shape::GetGlobalBounds() const
{
    return GetTransform().TransformRect(GetLocalBounds());
}


////////////////////////////////////////////////////////////
Shape::Shape() :
myTexture         (NULL),
myTextureRect     (),
myFillColor       (255, 255, 255),
myOutlineColor    (255, 255, 255),
myOutlineThickness(0),
myVertices        (TrianglesFan),
myOutlineVertices (TrianglesStrip),
myInsideBounds    (),
myBounds          ()
{
}


////////////////////////////////////////////////////////////
void Shape::Update()
{
    // Get the total number of points of the shape
    unsigned int count = GetPointCount();
    if (count < 3)
    {
        myVertices.Resize(0);
        myOutlineVertices.Resize(0);
        return;
    }

    myVertices.Resize(count + 2); // + 2 for center and repeated first point

    // Position
    for (unsigned int i = 0; i < count; ++i)
        myVertices[i + 1].Position = GetPoint(i);
    myVertices[count + 1].Position = myVertices[1].Position;

    // Update the bounding rectangle
    myVertices[0] = myVertices[1]; // so that the result of GetBounds() is correct
    myInsideBounds = myVertices.GetBounds();

    // Compute the center and make it the first vertex
    myVertices[0].Position.x = myInsideBounds.Left + myInsideBounds.Width / 2;
    myVertices[0].Position.y = myInsideBounds.Top + myInsideBounds.Height / 2;

    // Color
    UpdateFillColors();

    // Texture coordinates
    UpdateTexCoords();

    // Outline
    UpdateOutline();
}


////////////////////////////////////////////////////////////
void Shape::Draw(RenderTarget& target, RenderStates states) const
{
    states.Transform *= GetTransform();

    // Render the inside
    if (myFillColor.a > 0)
    {
        states.Texture = myTexture;
        target.Draw(myVertices, states);
    }

    // Render the outline
    if ((myOutlineColor.a > 0) && (myOutlineThickness > 0))
    {
        states.Texture = NULL;
        target.Draw(myOutlineVertices, states);
    }
}


////////////////////////////////////////////////////////////
void Shape::UpdateFillColors()
{
    for (unsigned int i = 0; i < myVertices.GetVertexCount(); ++i)
        myVertices[i].Color = myFillColor;
}


////////////////////////////////////////////////////////////
void Shape::UpdateTexCoords()
{
    for (unsigned int i = 0; i < myVertices.GetVertexCount(); ++i)
    {
        float xratio = (myVertices[i].Position.x - myInsideBounds.Left) / myInsideBounds.Width;
        float yratio = (myVertices[i].Position.y - myInsideBounds.Top) / myInsideBounds.Height;
        myVertices[i].TexCoords.x = myTextureRect.Left + myTextureRect.Width * xratio;
        myVertices[i].TexCoords.y = myTextureRect.Top + myTextureRect.Height * yratio;
    }
}


////////////////////////////////////////////////////////////
void Shape::UpdateOutline()
{
    unsigned int count = myVertices.GetVertexCount() - 2;
    myOutlineVertices.Resize((count + 1) * 2);

    for (unsigned int i = 0; i < count; ++i)
    {
        unsigned int index = i + 1;

        // Get the two segments shared by the current point
        Vector2f p0 = (i == 0) ? myVertices[count].Position : myVertices[index - 1].Position;
        Vector2f p1 = myVertices[index].Position;
        Vector2f p2 = myVertices[index + 1].Position;

        // Compute their normal
        Vector2f n1 = ComputeNormal(p0, p1);
        Vector2f n2 = ComputeNormal(p1, p2);

        // Combine them to get the extrusion direction
        float factor = 1.f + (n1.x * n2.x + n1.y * n2.y);
        Vector2f normal = -(n1 + n2) / factor;

        // Update the outline points
        myOutlineVertices[i * 2 + 0].Position = p1;
        myOutlineVertices[i * 2 + 1].Position = p1 + normal * myOutlineThickness;
    }

    // Duplicate the first point at the end, to close the outline
    myOutlineVertices[count * 2 + 0].Position = myOutlineVertices[0].Position;
    myOutlineVertices[count * 2 + 1].Position = myOutlineVertices[1].Position;

    // Update outline colors
    UpdateOutlineColors();

    // Update the shape's bounds
    myBounds = myOutlineVertices.GetBounds();
}


////////////////////////////////////////////////////////////
void Shape::UpdateOutlineColors()
{
    for (unsigned int i = 0; i < myOutlineVertices.GetVertexCount(); ++i)
        myOutlineVertices[i].Color = myOutlineColor;
}

} // namespace sf
