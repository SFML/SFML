////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2024 Laurent Gomila (laurent@sfml-dev.org)
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

#pragma once

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics/Color.hpp>

#include <SFML/System/Vector2.hpp>


namespace sf
{
////////////////////////////////////////////////////////////
/// \brief Define a point with color and texture coordinates
///
/// By default, the vertex color is white and texture coordinates are (0, 0).
///
////////////////////////////////////////////////////////////
struct Vertex
{
    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    Vector2f position{};          //!< 2D position of the vertex
    Color    color{Color::White}; //!< Color of the vertex
    Vector2f texCoords{};         //!< Coordinates of the texture's pixel to map to the vertex
};

} // namespace sf


////////////////////////////////////////////////////////////
/// \struct sf::Vertex
/// \ingroup graphics
///
/// A vertex is an improved point. It has a position and other
/// extra attributes that will be used for drawing: in SFML,
/// vertices also have a color and a pair of texture coordinates.
///
/// The vertex is the building block of drawing. Everything which
/// is visible on screen is made of vertices. They are grouped
/// as 2D primitives (lines, triangles, ...), and these primitives
/// are grouped to create even more complex 2D entities such as
/// sprites, texts, etc.
///
/// If you use the graphical entities of SFML (sprite, text, shape)
/// you won't have to deal with vertices directly. But if you want
/// to define your own 2D entities, such as tiled maps or particle
/// systems, using vertices will allow you to get maximum performances.
///
/// Example:
/// \code
/// // define a 100x100 square, red, with a 10x10 texture mapped on it
/// sf::Vertex vertices[]
/// {
///     {{  0.0f,   0.0f}, sf::Color::Red, { 0.0f,  0.0f}},
///     {{  0.0f, 100.0f}, sf::Color::Red, { 0.0f, 10.0f}},
///     {{100.0f, 100.0f}, sf::Color::Red, {10.0f, 10.0f}},
///     {{  0.0f,   0.0f}, sf::Color::Red, { 0.0f,  0.0f}},
///     {{100.0f, 100.0f}, sf::Color::Red, {10.0f, 10.0f}},
///     {{100.0f,   0.0f}, sf::Color::Red, {10.0f,  0.0f}}
/// };
///
/// // draw it
/// window.draw(vertices, 6, sf::PrimitiveType::Triangles);
/// \endcode
///
///
/// It is recommended to use aggregate initialization to create vertex
/// objects, which initializes the members in order.
///
/// On a C++20-compliant compiler (or where supported as an extension)
/// it is possible to use "designated initializers" to only initialize
/// a subset of members, with the restriction of having to follow the
/// same order in which they are defined.
///
/// Example:
/// \code
/// // C++17 and above
/// sf::Vertex v0{{5.0f, 5.0f}};                               // explicit 'position', implicit 'color' and 'texCoords'
/// sf::Vertex v1{{5.0f, 5.0f}, sf::Color::Red};               // explicit 'position' and 'color', implicit 'texCoords'
/// sf::Vertex v2{{5.0f, 5.0f}, sf::Color::Red, {1.0f, 1.0f}}; // everything is explicitly specified
///
/// // C++20 and above (or compilers supporting "designated initializers" as an extension)
/// sf::Vertex v3{
///    .position{5.0f, 5.0f},
///    .texCoords{1.0f, 1.0f}
/// };
/// \endcode
///
///
/// Note: although texture coordinates are supposed to be an integer
/// amount of pixels, their type is float because of some buggy graphics
/// drivers that are not able to process integer coordinates correctly.
///
/// \see sf::VertexArray
///
////////////////////////////////////////////////////////////
