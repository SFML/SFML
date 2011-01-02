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
#include <SFML/Graphics/Renderer.hpp>
#include <cmath>


namespace sf
{
////////////////////////////////////////////////////////////
Shape::Shape() :
myOutline         (0.f),
myIsFillEnabled   (true),
myIsOutlineEnabled(true),
myIsCompiled      (false)
{
    // Put a placeholder for the center of the shape
    myPoints.push_back(Point());
}


////////////////////////////////////////////////////////////
void Shape::AddPoint(float x, float y, const Color& color, const Color& outlineColor)
{
    AddPoint(Vector2f(x, y), color, outlineColor);
}


////////////////////////////////////////////////////////////
void Shape::AddPoint(const Vector2f& position, const Color& color, const Color& outlineColor)
{
    myPoints.push_back(Point(position, color, outlineColor));
    myIsCompiled = false;
}


////////////////////////////////////////////////////////////
unsigned int Shape::GetPointsCount() const
{
    return static_cast<unsigned int>(myPoints.size() - 1);
}


////////////////////////////////////////////////////////////
void Shape::EnableFill(bool enable)
{
    myIsFillEnabled = enable;
}


////////////////////////////////////////////////////////////
void Shape::EnableOutline(bool enable)
{
    myIsOutlineEnabled = enable;
}


////////////////////////////////////////////////////////////
void Shape::SetPointPosition(unsigned int index, const Vector2f& position)
{
    myPoints[index + 1].Position = position;
    myIsCompiled = false;
}


////////////////////////////////////////////////////////////
void Shape::SetPointPosition(unsigned int index, float x, float y)
{
    SetPointPosition(index, Vector2f(x, y));
}


////////////////////////////////////////////////////////////
void Shape::SetPointColor(unsigned int index, const Color& color)
{
    myPoints[index + 1].Col = color;
    myIsCompiled = false;
}


////////////////////////////////////////////////////////////
void Shape::SetPointOutlineColor(unsigned int index, const Color& color)
{
    myPoints[index + 1].OutlineCol = color;
    myIsCompiled = false;
}


////////////////////////////////////////////////////////////
void Shape::SetOutlineThickness(float thickness)
{
    myOutline = thickness;
}


////////////////////////////////////////////////////////////
const Vector2f& Shape::GetPointPosition(unsigned int index) const
{
    return myPoints[index + 1].Position;
}


////////////////////////////////////////////////////////////
const Color& Shape::GetPointColor(unsigned int index) const
{
    return myPoints[index + 1].Col;
}


////////////////////////////////////////////////////////////
const Color& Shape::GetPointOutlineColor(unsigned int index) const
{
    return myPoints[index + 1].OutlineCol;
}


////////////////////////////////////////////////////////////
float Shape::GetOutlineThickness() const
{
    return myOutline;
}


////////////////////////////////////////////////////////////
Shape Shape::Line(float p1x, float p1y, float p2x, float p2y, float thickness, const Color& color, float outline, const Color& outlineColor)
{
    Vector2f p1(p1x, p1y);
    Vector2f p2(p2x, p2y);

    return Shape::Line(p1, p2, thickness, color, outline, outlineColor);
}


////////////////////////////////////////////////////////////
Shape Shape::Line(const Vector2f& p1, const Vector2f& p2, float thickness, const Color& color, float outline, const Color& outlineColor)
{
    // Compute the extrusion direction
    Vector2f normal;
    ComputeNormal(p1, p2, normal);
    normal *= thickness / 2;

    // Create the shape's points
    Shape shape;
    shape.AddPoint(p1 - normal, color, outlineColor);
    shape.AddPoint(p2 - normal, color, outlineColor);
    shape.AddPoint(p2 + normal, color, outlineColor);
    shape.AddPoint(p1 + normal, color, outlineColor);
    shape.SetOutlineThickness(outline);

    // Compile it
    shape.Compile();

    return shape;
}


////////////////////////////////////////////////////////////
Shape Shape::Rectangle(float left, float top, float width, float height, const Color& color, float outline, const Color& outlineColor)
{
    // Create the shape's points
    Shape shape;
    shape.AddPoint(Vector2f(left,         top),          color, outlineColor);
    shape.AddPoint(Vector2f(left + width, top),          color, outlineColor);
    shape.AddPoint(Vector2f(left + width, top + height), color, outlineColor);
    shape.AddPoint(Vector2f(left,         top + height), color, outlineColor);
    shape.SetOutlineThickness(outline);

    // Compile it
    shape.Compile();

    return shape;
}


////////////////////////////////////////////////////////////
Shape Shape::Rectangle(const FloatRect& rectangle, const Color& color, float outline, const Color& outlineColor)
{
    return Shape::Rectangle(rectangle.Left, rectangle.Top, rectangle.Width, rectangle.Height, color, outline, outlineColor);
}


////////////////////////////////////////////////////////////
Shape Shape::Circle(float x, float y, float radius, const Color& color, float outline, const Color& outlineColor)
{
    return Shape::Circle(Vector2f(x, y), radius, color, outline, outlineColor);
}


////////////////////////////////////////////////////////////
Shape Shape::Circle(const Vector2f& center, float radius, const Color& color, float outline, const Color& outlineColor)
{
    static const int nbSegments = 40;

    // Create the points set
    Shape shape;
    for (int i = 0; i < nbSegments; ++i)
    {
        float angle = i * 2 * 3.141592654f / nbSegments;
        Vector2f offset(std::cos(angle), std::sin(angle));

        shape.AddPoint(center + offset * radius, color, outlineColor);
    }

    // Compile it
    shape.SetOutlineThickness(outline);
    shape.Compile();

    return shape;
}


////////////////////////////////////////////////////////////
void Shape::Render(RenderTarget&, Renderer& renderer) const
{
    // Make sure the shape has at least 3 points (4 if we count the center)
    if (myPoints.size() < 4)
        return;

    // Make sure the shape is compiled
    if (!myIsCompiled)
        const_cast<Shape*>(this)->Compile();

    // Shapes only use color, no texture
    renderer.SetTexture(NULL);

    // Draw the shape
    if (myIsFillEnabled)
    {
        if (myPoints.size() == 4)
        {
            // Special case of a triangle
            renderer.Begin(Renderer::TriangleList);
                renderer.AddVertex(myPoints[1].Position.x, myPoints[1].Position.y, myPoints[1].Col);
                renderer.AddVertex(myPoints[2].Position.x, myPoints[2].Position.y, myPoints[2].Col);
                renderer.AddVertex(myPoints[3].Position.x, myPoints[3].Position.y, myPoints[3].Col);
            renderer.End();
        }
        else if (myPoints.size() == 5)
        {
            // Special case of a quad
            renderer.Begin(Renderer::TriangleStrip);
                renderer.AddVertex(myPoints[1].Position.x, myPoints[1].Position.y, myPoints[1].Col);
                renderer.AddVertex(myPoints[2].Position.x, myPoints[2].Position.y, myPoints[2].Col);
                renderer.AddVertex(myPoints[4].Position.x, myPoints[4].Position.y, myPoints[4].Col);
                renderer.AddVertex(myPoints[3].Position.x, myPoints[3].Position.y, myPoints[3].Col);
            renderer.End();
        }
        else
        {
            renderer.Begin(Renderer::TriangleFan);

            // General case of a convex polygon
            for (std::vector<Point>::const_iterator i = myPoints.begin(); i != myPoints.end(); ++i)
                renderer.AddVertex(i->Position.x, i->Position.y, i->Col);

            // Close the shape by duplicating the first point at the end
            const Point& first = myPoints[1];
            renderer.AddVertex(first.Position.x, first.Position.y, first.Col);

            renderer.End();
        }
    }

    // Draw the outline
    if (myIsOutlineEnabled && (myOutline != 0))
    {
        renderer.Begin(Renderer::TriangleStrip);

        for (std::vector<Point>::const_iterator i = myPoints.begin() + 1; i != myPoints.end(); ++i)
        {
            Vector2f point1 = i->Position;
            Vector2f point2 = i->Position + i->Normal * myOutline;
            renderer.AddVertex(point1.x, point1.y, i->OutlineCol);
            renderer.AddVertex(point2.x, point2.y, i->OutlineCol);
        }

        // Close the shape by duplicating the first point at the end
        const Point& first = myPoints[1];
        Vector2f point1 = first.Position;
        Vector2f point2 = first.Position + first.Normal * myOutline;
        renderer.AddVertex(point1.x, point1.y, first.OutlineCol);
        renderer.AddVertex(point2.x, point2.y, first.OutlineCol);

        renderer.End();
    }
}


////////////////////////////////////////////////////////////
void Shape::Compile()
{
    // Compute the center
    float nbPoints = static_cast<float>(myPoints.size() - 1);
    float r = 0, g = 0, b = 0, a = 0;
    Point center(Vector2f(0, 0), Color(0, 0, 0, 0));
    for (std::size_t i = 1; i < myPoints.size(); ++i)
    {
        center.Position += myPoints[i].Position;
        r += myPoints[i].Col.r;
        g += myPoints[i].Col.g;
        b += myPoints[i].Col.b;
        a += myPoints[i].Col.a;
    }
    center.Position /= nbPoints;
    center.Col.r = static_cast<Uint8>(r / nbPoints);
    center.Col.g = static_cast<Uint8>(g / nbPoints);
    center.Col.b = static_cast<Uint8>(b / nbPoints);
    center.Col.a = static_cast<Uint8>(a / nbPoints);
    myPoints[0] = center;

    // Compute the outline
    for (std::size_t i = 1; i < myPoints.size(); ++i)
    {
        // Get the two segments shared by the current point
        Point& p0 = (i == 1) ? myPoints[myPoints.size() - 1] : myPoints[i - 1];
        Point& p1 = myPoints[i];
        Point& p2 = (i == myPoints.size() - 1) ? myPoints[1] : myPoints[i + 1];

        // Compute their normal
        Vector2f normal1, normal2;
        if (!ComputeNormal(p0.Position, p1.Position, normal1) || !ComputeNormal(p1.Position, p2.Position, normal2))
            continue;

        // Add them to get the extrusion direction
        float factor = 1.f + (normal1.x * normal2.x + normal1.y * normal2.y);
        p1.Normal = (normal1 + normal2) / factor;

        // Make sure it points towards the outside of the shape
        float dot = (p1.Position.x - center.Position.x) * p1.Normal.x + (p1.Position.y - center.Position.y) * p1.Normal.y;
        if (dot < 0)
            p1.Normal = -p1.Normal;
    }

    myIsCompiled = true;
}


////////////////////////////////////////////////////////////
bool Shape::ComputeNormal(const Vector2f& p1, const Vector2f& p2, Vector2f& normal)
{
    normal.x = p1.y - p2.y;
    normal.y = p2.x - p1.x;

    float len = std::sqrt(normal.x * normal.x + normal.y * normal.y);
    if (len == 0.f)
        return false;

    normal.x /= len;
    normal.y /= len;

    return true;
}


////////////////////////////////////////////////////////////
Shape::Point::Point(const Vector2f& position, const Color& color, const Color& outlineColor) :
Position  (position),
Normal    (0.f, 0.f),
Col       (color),
OutlineCol(outlineColor)
{

}

} // namespace sf
