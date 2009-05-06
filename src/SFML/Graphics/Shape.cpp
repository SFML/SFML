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
#include <SFML/Graphics/GraphicsContext.hpp>
#include <math.h>


namespace sf
{
////////////////////////////////////////////////////////////
/// Default constructor
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
/// Add a point to the shape
////////////////////////////////////////////////////////////
void Shape::AddPoint(float X, float Y, const Color& Col, const Color& OutlineCol)
{
    AddPoint(Vector2f(X, Y), Col, OutlineCol);
}


////////////////////////////////////////////////////////////
/// Add a point to the shape
////////////////////////////////////////////////////////////
void Shape::AddPoint(const Vector2f& Position, const Color& Col, const Color& OutlineCol)
{
    myPoints.push_back(Point(Position, Col, OutlineCol));
    myIsCompiled = false;
}


////////////////////////////////////////////////////////////
/// Get the number of points composing the shape
////////////////////////////////////////////////////////////
unsigned int Shape::GetNbPoints() const
{
    return static_cast<unsigned int>(myPoints.size() - 1);
}


////////////////////////////////////////////////////////////
/// Enable or disable filling the shape.
/// Fill is enabled by default
////////////////////////////////////////////////////////////
void Shape::EnableFill(bool Enable)
{
    myIsFillEnabled = Enable;
}


////////////////////////////////////////////////////////////
/// Enable or disable drawing the shape outline.
/// Outline is enabled by default
////////////////////////////////////////////////////////////
void Shape::EnableOutline(bool Enable)
{
    myIsOutlineEnabled = Enable;
}


////////////////////////////////////////////////////////////
/// Set the position of a point
////////////////////////////////////////////////////////////
void Shape::SetPointPosition(unsigned int Index, const Vector2f& Position)
{
    myPoints[Index + 1].Position = Position;
    myIsCompiled = false;
}


////////////////////////////////////////////////////////////
/// Set the position of a point
////////////////////////////////////////////////////////////
void Shape::SetPointPosition(unsigned int Index, float X, float Y)
{
    SetPointPosition(Index, Vector2f(X, Y));
}


////////////////////////////////////////////////////////////
/// Set the color of a point
////////////////////////////////////////////////////////////
void Shape::SetPointColor(unsigned int Index, const Color& Col)
{
    myPoints[Index + 1].Col = Col;
    myIsCompiled = false;
}


////////////////////////////////////////////////////////////
/// Set the outline color of a point
////////////////////////////////////////////////////////////
void Shape::SetPointOutlineColor(unsigned int Index, const Color& OutlineCol)
{
    myPoints[Index + 1].OutlineCol = OutlineCol;
    myIsCompiled = false;
}


////////////////////////////////////////////////////////////
/// Change the width of the shape outline
////////////////////////////////////////////////////////////
void Shape::SetOutlineWidth(float Width)
{
    myOutline = Width;
}


////////////////////////////////////////////////////////////
/// Get the position of a point
////////////////////////////////////////////////////////////
const Vector2f& Shape::GetPointPosition(unsigned int Index) const
{
    return myPoints[Index + 1].Position;
}


////////////////////////////////////////////////////////////
/// Get the color of a point
////////////////////////////////////////////////////////////
const Color& Shape::GetPointColor(unsigned int Index) const
{
    return myPoints[Index + 1].Col;
}


////////////////////////////////////////////////////////////
/// Get the outline color of a point
////////////////////////////////////////////////////////////
const Color& Shape::GetPointOutlineColor(unsigned int Index) const
{
    return myPoints[Index + 1].OutlineCol;
}


////////////////////////////////////////////////////////////
/// Get the width of the shape outline
////////////////////////////////////////////////////////////
float Shape::GetOutlineWidth() const
{
    return myOutline;
}


////////////////////////////////////////////////////////////
/// Create a shape made of a single line
////////////////////////////////////////////////////////////
Shape Shape::Line(float P1X, float P1Y, float P2X, float P2Y, float Thickness, const Color& Col, float Outline, const Color& OutlineCol)
{
    Vector2f P1(P1X, P1Y);
    Vector2f P2(P2X, P2Y);

    // Compute the extrusion direction
    Vector2f Normal;
    ComputeNormal(P1, P2, Normal);
    Normal *= Thickness / 2;

    // Create the shape's points
    Shape S;
    S.AddPoint(P1 - Normal, Col, OutlineCol);
    S.AddPoint(P2 - Normal, Col, OutlineCol);
    S.AddPoint(P2 + Normal, Col, OutlineCol);
    S.AddPoint(P1 + Normal, Col, OutlineCol);
    S.SetOutlineWidth(Outline);

    // Compile it
    S.Compile();

    return S;
}


////////////////////////////////////////////////////////////
/// Create a shape made of a single line (use vectors)
////////////////////////////////////////////////////////////
Shape Shape::Line(const Vector2f& P1, const Vector2f& P2, float Thickness, const Color& Col, float Outline, const Color& OutlineCol)
{
    return Shape::Line(P1.x, P1.y, P2.x, P2.y, Thickness, Col, Outline, OutlineCol);
}


////////////////////////////////////////////////////////////
/// Create a shape made of a single rectangle
////////////////////////////////////////////////////////////
Shape Shape::Rectangle(float P1X, float P1Y, float P2X, float P2Y, const Color& Col, float Outline, const Color& OutlineCol)
{
    // Create the shape's points
    Shape S;
    S.AddPoint(Vector2f(P1X, P1Y), Col, OutlineCol);
    S.AddPoint(Vector2f(P2X, P1Y), Col, OutlineCol);
    S.AddPoint(Vector2f(P2X, P2Y), Col, OutlineCol);
    S.AddPoint(Vector2f(P1X, P2Y), Col, OutlineCol);
    S.SetOutlineWidth(Outline);

    // Compile it
    S.Compile();

    return S;
}


////////////////////////////////////////////////////////////
/// Create a shape made of a single rectangle (use vectors)
////////////////////////////////////////////////////////////
Shape Shape::Rectangle(const Vector2f& P1, const Vector2f& P2, const Color& Col, float Outline, const Color& OutlineCol)
{
    return Shape::Rectangle(P1.x, P1.y, P2.x, P2.y, Col, Outline, OutlineCol);
}


////////////////////////////////////////////////////////////
/// Create a shape made of a single circle
////////////////////////////////////////////////////////////
Shape Shape::Circle(float X, float Y, float Radius, const Color& Col, float Outline, const Color& OutlineCol)
{
    static const int NbSegments = 40;

    // Create the points set
    Shape S;
    Vector2f Center(X, Y);
    for (int i = 0; i < NbSegments; ++i)
    {
        float Angle = i * 2 * 3.141592654f / NbSegments;
        Vector2f Offset(cos(Angle), sin(Angle));

        S.AddPoint(Center + Offset * Radius, Col, OutlineCol);
    }

    // Compile it
    S.SetOutlineWidth(Outline);
    S.Compile();

    return S;
}


////////////////////////////////////////////////////////////
/// Create a shape made of a single circle (use vectors)
////////////////////////////////////////////////////////////
Shape Shape::Circle(const Vector2f& Center, float Radius, const Color& Col, float Outline, const Color& OutlineCol)
{
    return Shape::Circle(Center.x, Center.y, Radius, Col, Outline, OutlineCol);
}


////////////////////////////////////////////////////////////
/// /see Drawable::Render
////////////////////////////////////////////////////////////
void Shape::Render(RenderTarget&) const
{
    // Make sure the shape has at least 3 points (4 if we count the center)
    if (myPoints.size() < 4)
        return;

    // Make sure the shape is compiled
    if (!myIsCompiled)
        const_cast<Shape*>(this)->Compile();

    // Shapes only use color, no texture
    GLCheck(glDisable(GL_TEXTURE_2D));

    // Draw the shape
    if (myIsFillEnabled)
    {
        glBegin(GL_TRIANGLE_FAN);
        {
            for (std::vector<Point>::const_iterator i = myPoints.begin(); i != myPoints.end(); ++i)
            {
                Color PointColor = i->Col * GetColor();
                glColor4f(PointColor.r / 255.f, PointColor.g / 255.f, PointColor.b / 255.f, PointColor.a / 255.f);
                glVertex2f(i->Position.x, i->Position.y);
            }

            // Close the shape by duplicating the first point at the end
            Color PointColor = myPoints[1].Col * GetColor();
            glColor4f(PointColor.r / 255.f, PointColor.g / 255.f, PointColor.b / 255.f, PointColor.a / 255.f);
            glVertex2f(myPoints[1].Position.x, myPoints[1].Position.y);
        }
        glEnd();
    }

    // Draw the outline
    if (myIsOutlineEnabled)
    {
        glBegin(GL_TRIANGLE_STRIP);
        {
            for (std::size_t i = 1; i < myPoints.size(); ++i)
            {
                Color PointColor = myPoints[i].OutlineCol * GetColor();
                glColor4f(PointColor.r / 255.f, PointColor.g / 255.f, PointColor.b / 255.f, PointColor.a / 255.f);
                glVertex2f(myPoints[i].Position.x, myPoints[i].Position.y);
                glColor4f(PointColor.r / 255.f, PointColor.g / 255.f, PointColor.b / 255.f, PointColor.a / 255.f);
                glVertex2f(myPoints[i].Position.x + myPoints[i].Normal.x * myOutline, myPoints[i].Position.y + myPoints[i].Normal.y * myOutline);
            }

            // Close the shape by duplicating the first point at the end
            Color PointColor = myPoints[1].OutlineCol * GetColor();
            glColor4f(PointColor.r / 255.f, PointColor.g / 255.f, PointColor.b / 255.f, PointColor.a / 255.f);
            glVertex2f(myPoints[1].Position.x, myPoints[1].Position.y);
            glColor4f(PointColor.r / 255.f, PointColor.g / 255.f, PointColor.b / 255.f, PointColor.a / 255.f);
            glVertex2f(myPoints[1].Position.x + myPoints[1].Normal.x * myOutline, myPoints[1].Position.y + myPoints[1].Normal.y * myOutline);
        }
        glEnd();
    }
}


////////////////////////////////////////////////////////////
/// Compile the shape : compute its center and its outline
////////////////////////////////////////////////////////////
void Shape::Compile()
{
    // Compute the center
    float NbPoints = static_cast<float>(myPoints.size() - 1);
    float R = 0, G = 0, B = 0, A = 0;
    Point Center(Vector2f(0, 0), Color(0, 0, 0, 0));
    for (std::size_t i = 1; i < myPoints.size(); ++i)
    {
        Center.Position += myPoints[i].Position / NbPoints;
        R += myPoints[i].Col.r / NbPoints;
        G += myPoints[i].Col.g / NbPoints;
        B += myPoints[i].Col.b / NbPoints;
        A += myPoints[i].Col.a / NbPoints;
    }
    Center.Col.r = static_cast<Uint8>(R);
    Center.Col.g = static_cast<Uint8>(G);
    Center.Col.b = static_cast<Uint8>(B);
    Center.Col.a = static_cast<Uint8>(A);
    myPoints[0] = Center;

    // Compute the outline
    for (std::size_t i = 1; i < myPoints.size(); ++i)
    {
        // Get the two segments shared by the current point
        Point& P0 = (i == 1) ? myPoints[myPoints.size() - 1] : myPoints[i - 1];
        Point& P1 = myPoints[i];
        Point& P2 = (i == myPoints.size() - 1) ? myPoints[1] : myPoints[i + 1];

        // Compute their normal
        Vector2f Normal1, Normal2;
        if (!ComputeNormal(P0.Position, P1.Position, Normal1) || !ComputeNormal(P1.Position, P2.Position, Normal2))
            continue;

        // Add them to get the extrusion direction
        float Factor = 1.f + (Normal1.x * Normal2.x + Normal1.y * Normal2.y);
        P1.Normal = (Normal1 + Normal2) / Factor;

        // Make sure it points towards the outside of the shape
        float Dot = (P1.Position.x - Center.Position.x) * P1.Normal.x + (P1.Position.y - Center.Position.y) * P1.Normal.y;
        if (Dot < 0)
            P1.Normal = -P1.Normal;
    }

    myIsCompiled = true;
}


////////////////////////////////////////////////////////////
/// Compute the normal of a given 2D segment
////////////////////////////////////////////////////////////
bool Shape::ComputeNormal(const Vector2f& P1, const Vector2f& P2, Vector2f& Normal)
{
    Normal.x = P1.y - P2.y;
    Normal.y = P2.x - P1.x;

    float Len = sqrt(Normal.x * Normal.x + Normal.y * Normal.y);
    if (Len == 0.f)
        return false;

    Normal.x /= Len;
    Normal.y /= Len;

    return true;
}


////////////////////////////////////////////////////////////
/// Default constructor for Point
////////////////////////////////////////////////////////////
Shape::Point::Point(const Vector2f& Pos, const Color& C, const Color& OutlineC) :
Position  (Pos),
Normal    (0.f, 0.f),
Col       (C),
OutlineCol(OutlineC)
{

}

} // namespace sf
