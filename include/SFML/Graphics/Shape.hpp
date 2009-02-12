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

#ifndef SFML_SHAPE_HPP
#define SFML_SHAPE_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/System/Vector2.hpp>
#include <vector>


namespace sf
{
////////////////////////////////////////////////////////////
/// Shape defines a drawable convex shape ; it also defines
/// helper functions to draw simple shapes like
/// lines, rectangles, circles, etc.
////////////////////////////////////////////////////////////
class SFML_API Shape : public sf::Drawable
{
public :

    ////////////////////////////////////////////////////////////
    /// Default constructor
    ///
    ////////////////////////////////////////////////////////////
    Shape();

    ////////////////////////////////////////////////////////////
    /// Add a point to the shape
    ///
    /// \param X, Y :       Position of the point
    /// \param Col :        Color of the point (white by default)
    /// \param OutlineCol : Outline color of the point (black by default)
    ///
    ////////////////////////////////////////////////////////////
    void AddPoint(float X, float Y, const Color& Col = Color(255, 255, 255), const Color& OutlineCol = Color(0, 0, 0));

    ////////////////////////////////////////////////////////////
    /// Add a point to the shape
    ///
    /// \param Position :   Position of the point
    /// \param Col :        Color of the point (white by default)
    /// \param OutlineCol : Outline color of the point (black by default)
    ///
    ////////////////////////////////////////////////////////////
    void AddPoint(const Vector2f& Position, const Color& Col = Color(255, 255, 255), const Color& OutlineCol = Color(0, 0, 0));

    ////////////////////////////////////////////////////////////
    /// Get the number of points composing the shape
    ///
    /// \param Total number of points
    ///
    ////////////////////////////////////////////////////////////
    unsigned int GetNbPoints() const;

    ////////////////////////////////////////////////////////////
    /// Enable or disable filling the shape.
    /// Fill is enabled by default
    ///
    /// \param Enable : True to enable, false to disable
    ///
    ////////////////////////////////////////////////////////////
    void EnableFill(bool Enable);

    ////////////////////////////////////////////////////////////
    /// Enable or disable drawing the shape outline.
    /// Outline is enabled by default
    ///
    /// \param Enable : True to enable, false to disable
    ///
    ////////////////////////////////////////////////////////////
    void EnableOutline(bool Enable);

    ////////////////////////////////////////////////////////////
    /// Set the position of a point
    ///
    /// \param Index :    Index of the point, in range [0, GetNbPoints() - 1]
    /// \param Position : New position of the Index-th point
    ///
    ////////////////////////////////////////////////////////////
    void SetPointPosition(unsigned int Index, const Vector2f& Position);

    ////////////////////////////////////////////////////////////
    /// Set the position of a point
    ///
    /// \param Index : Index of the point, in range [0, GetNbPoints() - 1]
    /// \param X :     New X coordinate of the Index-th point
    /// \param Y :     New Y coordinate of the Index-th point
    ///
    ////////////////////////////////////////////////////////////
    void SetPointPosition(unsigned int Index, float X, float Y);

    ////////////////////////////////////////////////////////////
    /// Set the color of a point
    ///
    /// \param Index : Index of the point, in range [0, GetNbPoints() - 1]
    /// \param Col :   New color of the Index-th point
    ///
    ////////////////////////////////////////////////////////////
    void SetPointColor(unsigned int Index, const Color& Col);

    ////////////////////////////////////////////////////////////
    /// Set the outline color of a point
    ///
    /// \param Index :      Index of the point, in range [0, GetNbPoints() - 1]
    /// \param OutlineCol : New outline color of the Index-th point
    ///
    ////////////////////////////////////////////////////////////
    void SetPointOutlineColor(unsigned int Index, const Color& OutlineCol);

    ////////////////////////////////////////////////////////////
    /// Change the width of the shape outline
    ///
    /// \param Width : New width
    ///
    ////////////////////////////////////////////////////////////
    void SetOutlineWidth(float Width);

    ////////////////////////////////////////////////////////////
    /// Get the position of a point
    ///
    /// \param Index : Index of the point, in range [0, GetNbPoints() - 1]
    ///
    /// \return Position of the Index-th point
    ///
    ////////////////////////////////////////////////////////////
    const Vector2f& GetPointPosition(unsigned int Index) const;

    ////////////////////////////////////////////////////////////
    /// Get the color of a point
    ///
    /// \param Index : Index of the point, in range [0, GetNbPoints() - 1]
    ///
    /// \return Color of the Index-th point
    ///
    ////////////////////////////////////////////////////////////
    const Color& GetPointColor(unsigned int Index) const;

    ////////////////////////////////////////////////////////////
    /// Get the outline color of a point
    ///
    /// \param Index : Index of the point, in range [0, GetNbPoints() - 1]
    ///
    /// \return Outline color of the Index-th point
    ///
    ////////////////////////////////////////////////////////////
    const Color& GetPointOutlineColor(unsigned int Index) const;

    ////////////////////////////////////////////////////////////
    /// Get the width of the shape outline
    ///
    /// \return Current outline width
    ///
    ////////////////////////////////////////////////////////////
    float GetOutlineWidth() const;

    ////////////////////////////////////////////////////////////
    /// Create a shape made of a single line (use floats)
    ///
    /// \param P1X, P1Y :   Position of the first point
    /// \param P2X, P2Y :   Position second point
    /// \param Thickness :  Line thickness
    /// \param Col :        Color used to draw the line
    /// \param Outline :    Outline width (0 by default)
    /// \param OutlineCol : Color used to draw the outline (black by default)
    ///
    ////////////////////////////////////////////////////////////
    static Shape Line(float P1X, float P1Y, float P2X, float P2Y, float Thickness, const Color& Col, float Outline = 0.f, const Color& OutlineCol = sf::Color(0, 0, 0));

    ////////////////////////////////////////////////////////////
    /// Create a shape made of a single line (use vectors)
    ///
    /// \param P1X, P1Y :   Position of the first point
    /// \param P2X, P2Y :   Position second point
    /// \param Thickness :  Line thickness
    /// \param Col :        Color used to draw the line
    /// \param Outline :    Outline width (0 by default)
    /// \param OutlineCol : Color used to draw the outline (black by default)
    ///
    ////////////////////////////////////////////////////////////
    static Shape Line(const Vector2f& P1, const Vector2f& P2, float Thickness, const Color& Col, float Outline = 0.f, const Color& OutlineCol = sf::Color(0, 0, 0));

    ////////////////////////////////////////////////////////////
    /// Create a shape made of a single rectangle (use floats)
    ///
    /// \param P1X, P1Y :   Position of the first point
    /// \param P2X, P2Y :   Position second point
    /// \param Col :        Color used to fill the rectangle
    /// \param Outline :    Outline width (0 by default)
    /// \param OutlineCol : Color used to draw the outline (black by default)
    ///
    ////////////////////////////////////////////////////////////
    static Shape Rectangle(float P1X, float P1Y, float P2X, float P2Y, const Color& Col, float Outline = 0.f, const Color& OutlineCol = sf::Color(0, 0, 0));

    ////////////////////////////////////////////////////////////
    /// Create a shape made of a single rectangle (use vectors)
    ///
    /// \param P1 :         Position of the first point
    /// \param P2 :         Position second point
    /// \param Col :        Color used to fill the rectangle
    /// \param Outline :    Outline width (0 by default)
    /// \param OutlineCol : Color used to draw the outline (black by default)
    ///
    ////////////////////////////////////////////////////////////
    static Shape Rectangle(const Vector2f& P1, const Vector2f& P2, const Color& Col, float Outline = 0.f, const Color& OutlineCol = sf::Color(0, 0, 0));

    ////////////////////////////////////////////////////////////
    /// Create a shape made of a single circle (use floats)
    ///
    /// \param X, Y :       Position of the center
    /// \param Radius :     Radius
    /// \param Col :        Color used to fill the circle
    /// \param Outline :    Outline width (0 by default)
    /// \param OutlineCol : Color used to draw the outline (black by default)
    ///
    ////////////////////////////////////////////////////////////
    static Shape Circle(float X, float Y, float Radius, const Color& Col, float Outline = 0.f, const Color& OutlineCol = sf::Color(0, 0, 0));

    ////////////////////////////////////////////////////////////
    /// Create a shape made of a single circle (use vectors)
    ///
    /// \param Center :     Position of the center
    /// \param Radius :     Radius
    /// \param Col :        Color used to fill the circle
    /// \param Outline :    Outline width (0 by default)
    /// \param OutlineCol : Color used to draw the outline (black by default)
    ///
    ////////////////////////////////////////////////////////////
    static Shape Circle(const Vector2f& Center, float Radius, const Color& Col, float Outline = 0.f, const Color& OutlineCol = sf::Color(0, 0, 0));

protected :

    ////////////////////////////////////////////////////////////
    /// /see Drawable::Render
    ///
    ////////////////////////////////////////////////////////////
    virtual void Render(RenderTarget& Target) const;

private :

    ////////////////////////////////////////////////////////////
    /// Compile the shape : compute its center and its outline
    ///
    ////////////////////////////////////////////////////////////
    void Compile();

    ////////////////////////////////////////////////////////////
    /// Compute the normal of a given 2D segment
    ///
    /// \param P1 :     First point of the segment
    /// \param P2 :     Second point of the segment
    /// \param Normal : Calculated normal
    ///
    /// \return False if the normal couldn't be calculated (segment is null)
    ///
    ////////////////////////////////////////////////////////////
    static bool ComputeNormal(const Vector2f& P1, const Vector2f& P2, Vector2f& Normal);

    ////////////////////////////////////////////////////////////
    /// Defines a simple 2D point
    ////////////////////////////////////////////////////////////
    struct Point
    {
        Point(const Vector2f& Pos = Vector2f(0, 0), const Color& C = Color(255, 255, 255), const Color& OutlineC = Color(255, 255, 255));

        Vector2f Position;   ///< Position
        Vector2f Normal;     ///< Extruded normal
        Color    Col;        ///< Color of the point
        Color    OutlineCol; ///< Outline color of the point
    };

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    std::vector<Point> myPoints;           ///< Points composing the shape
    float              myOutline;          ///< Outline width
    bool               myIsFillEnabled;    ///< Should we draw the inside if the shape ?
    bool               myIsOutlineEnabled; ///< Should we draw the outline if the shape ?
    bool               myIsCompiled;       ///< Compiled state of the shape
};

} // namespace sf


#endif // SFML_SHAPE_HPP
