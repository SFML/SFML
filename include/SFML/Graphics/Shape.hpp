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
class SFML_API Shape : public Drawable
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
    /// \param x, y :         Position of the point
    /// \param color :        Color of the point (white by default)
    /// \param outlineColor : Outline color of the point (black by default)
    ///
    ////////////////////////////////////////////////////////////
    void AddPoint(float x, float y, const Color& color = Color(255, 255, 255), const Color& outlineColor = Color(0, 0, 0));

    ////////////////////////////////////////////////////////////
    /// Add a point to the shape
    ///
    /// \param position :     Position of the point
    /// \param color :        Color of the point (white by default)
    /// \param outlineColor : Outline color of the point (black by default)
    ///
    ////////////////////////////////////////////////////////////
    void AddPoint(const Vector2f& position, const Color& color = Color(255, 255, 255), const Color& outlineColor = Color(0, 0, 0));

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
    /// \param enable : True to enable, false to disable
    ///
    ////////////////////////////////////////////////////////////
    void EnableFill(bool enable);

    ////////////////////////////////////////////////////////////
    /// Enable or disable drawing the shape outline.
    /// Outline is enabled by default
    ///
    /// \param enable : True to enable, false to disable
    ///
    ////////////////////////////////////////////////////////////
    void EnableOutline(bool enable);

    ////////////////////////////////////////////////////////////
    /// Set the position of a point
    ///
    /// \param index :    Index of the point, in range [0, GetNbPoints() - 1]
    /// \param position : New position of the index-th point
    ///
    ////////////////////////////////////////////////////////////
    void SetPointPosition(unsigned int index, const Vector2f& position);

    ////////////////////////////////////////////////////////////
    /// Set the position of a point
    ///
    /// \param index : Index of the point, in range [0, GetNbPoints() - 1]
    /// \param x :     New X coordinate of the index-th point
    /// \param y :     New Y coordinate of the index-th point
    ///
    ////////////////////////////////////////////////////////////
    void SetPointPosition(unsigned int index, float x, float y);

    ////////////////////////////////////////////////////////////
    /// Set the color of a point
    ///
    /// \param index : Index of the point, in range [0, GetNbPoints() - 1]
    /// \param color : New color of the index-th point
    ///
    ////////////////////////////////////////////////////////////
    void SetPointColor(unsigned int index, const Color& color);

    ////////////////////////////////////////////////////////////
    /// Set the outline color of a point
    ///
    /// \param index :       Index of the point, in range [0, GetNbPoints() - 1]
    /// \param outlineColor : New outline color of the index-th point
    ///
    ////////////////////////////////////////////////////////////
    void SetPointOutlineColor(unsigned int index, const Color& outlineColor);

    ////////////////////////////////////////////////////////////
    /// Change the width of the shape outline
    ///
    /// \param width : New width
    ///
    ////////////////////////////////////////////////////////////
    void SetOutlineWidth(float width);

    ////////////////////////////////////////////////////////////
    /// Get the position of a point
    ///
    /// \param index : Index of the point, in range [0, GetNbPoints() - 1]
    ///
    /// \return Position of the index-th point
    ///
    ////////////////////////////////////////////////////////////
    const Vector2f& GetPointPosition(unsigned int index) const;

    ////////////////////////////////////////////////////////////
    /// Get the color of a point
    ///
    /// \param Index : index of the point, in range [0, GetNbPoints() - 1]
    ///
    /// \return Color of the index-th point
    ///
    ////////////////////////////////////////////////////////////
    const Color& GetPointColor(unsigned int index) const;

    ////////////////////////////////////////////////////////////
    /// Get the outline color of a point
    ///
    /// \param index : Index of the point, in range [0, GetNbPoints() - 1]
    ///
    /// \return Outline color of the index-th point
    ///
    ////////////////////////////////////////////////////////////
    const Color& GetPointOutlineColor(unsigned int index) const;

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
    /// \param p1x, p1y :     Position of the first point
    /// \param p2x, p2y :     Position second point
    /// \param thickness :    Line thickness
    /// \param color :        Color used to draw the line
    /// \param outline :      Outline width (0 by default)
    /// \param outlineColor : Color used to draw the outline (black by default)
    ///
    ////////////////////////////////////////////////////////////
    static Shape Line(float p1x, float p1y, float p2x, float p2y, float thickness, const Color& color, float outline = 0.f, const Color& outlineColor = sf::Color(0, 0, 0));

    ////////////////////////////////////////////////////////////
    /// Create a shape made of a single line (use vectors)
    ///
    /// \param p1 :           Position of the first point
    /// \param p2 :           Position second point
    /// \param thickness :    Line thickness
    /// \param color :        Color used to draw the line
    /// \param outline :      Outline width (0 by default)
    /// \param outlineColor : Color used to draw the outline (black by default)
    ///
    ////////////////////////////////////////////////////////////
    static Shape Line(const Vector2f& p1, const Vector2f& p2, float thickness, const Color& color, float outline = 0.f, const Color& outlineColor = sf::Color(0, 0, 0));

    ////////////////////////////////////////////////////////////
    /// Create a shape made of a single rectangle (use floats)
    ///
    /// \param p1x, p1y :     Position of the first point
    /// \param p2x, p2y :     Position second point
    /// \param color :        Color used to fill the rectangle
    /// \param outline :      Outline width (0 by default)
    /// \param outlineColor : Color used to draw the outline (black by default)
    ///
    ////////////////////////////////////////////////////////////
    static Shape Rectangle(float p1x, float p1y, float p2x, float p2y, const Color& color, float outline = 0.f, const Color& outlineColor = sf::Color(0, 0, 0));

    ////////////////////////////////////////////////////////////
    /// Create a shape made of a single rectangle (use vectors)
    ///
    /// \param p1 :           Position of the first point
    /// \param p2 :           Position second point
    /// \param color :        Color used to fill the rectangle
    /// \param outline :      Outline width (0 by default)
    /// \param outlineColor : Color used to draw the outline (black by default)
    ///
    ////////////////////////////////////////////////////////////
    static Shape Rectangle(const Vector2f& p1, const Vector2f& p2, const Color& color, float outline = 0.f, const Color& outlineColor = sf::Color(0, 0, 0));

    ////////////////////////////////////////////////////////////
    /// Create a shape made of a single circle (use floats)
    ///
    /// \param x, y :         Position of the center
    /// \param radius :       Radius
    /// \param color :        Color used to fill the circle
    /// \param outline :      Outline width (0 by default)
    /// \param outlineColor : Color used to draw the outline (black by default)
    ///
    ////////////////////////////////////////////////////////////
    static Shape Circle(float x, float y, float radius, const Color& color, float outline = 0.f, const Color& outlineColor = sf::Color(0, 0, 0));

    ////////////////////////////////////////////////////////////
    /// Create a shape made of a single circle (use vectors)
    ///
    /// \param center :       Position of the center
    /// \param radius :       Radius
    /// \param color :        Color used to fill the circle
    /// \param outline :      Outline width (0 by default)
    /// \param outlineColor : Color used to draw the outline (black by default)
    ///
    ////////////////////////////////////////////////////////////
    static Shape Circle(const Vector2f& center, float radius, const Color& color, float outline = 0.f, const Color& outlineColor = sf::Color(0, 0, 0));

protected :

    ////////////////////////////////////////////////////////////
    /// /see Drawable::Render
    ///
    ////////////////////////////////////////////////////////////
    virtual void Render(RenderTarget& target, RenderQueue& queue) const;

private :

    ////////////////////////////////////////////////////////////
    /// Compile the shape : compute its center and its outline
    ///
    ////////////////////////////////////////////////////////////
    void Compile();

    ////////////////////////////////////////////////////////////
    /// Compute the normal of a given 2D segment
    ///
    /// \param p1 :     First point of the segment
    /// \param p2 :     Second point of the segment
    /// \param normal : Calculated normal
    ///
    /// \return False if the normal couldn't be calculated (segment is null)
    ///
    ////////////////////////////////////////////////////////////
    static bool ComputeNormal(const Vector2f& p1, const Vector2f& p2, Vector2f& normal);

    ////////////////////////////////////////////////////////////
    /// Defines a simple 2D point
    ////////////////////////////////////////////////////////////
    struct Point
    {
        Point(const Vector2f& position = Vector2f(0, 0), const Color& color = Color(255, 255, 255), const Color& outlineColor = Color(255, 255, 255));

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
