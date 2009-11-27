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
#include <SFML/Graphics/GeometryRendererIM.hpp>
#include <SFML/Graphics/GLCheck.hpp>


namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
bool GeometryRendererIM::IsSupported()
{
    // Immediate mode is always supported
    return true;
}


////////////////////////////////////////////////////////////
GeometryRendererIM::GeometryRendererIM() :
myVertices(NULL),
myIndices (NULL)
{

}


////////////////////////////////////////////////////////////
void GeometryRendererIM::Begin(const float* vertices, std::size_t, const std::size_t* indices, std::size_t)
{
    // Store the geometry informations for later rendering
    myVertices = vertices;
    myIndices = indices;
}


////////////////////////////////////////////////////////////
void GeometryRendererIM::End()
{
    // Nothing to do
}


////////////////////////////////////////////////////////////
void GeometryRendererIM::RenderTriangles(std::size_t start, std::size_t count)
{
    // Caculate the bounds of the geometry range to render
    const std::size_t* begin = myIndices + start;
    const std::size_t* end = begin + count;

    // Begin rendering
    glBegin(GL_TRIANGLES);

    // Send the vertices one by one
    for (const std::size_t* index = begin; index != end; index++)
    {
        const float* vertex = myVertices + *index * 8;

        glColor4f(vertex[2], vertex[3], vertex[4], vertex[5]);
        glTexCoord2f(vertex[6], vertex[7]);
        glVertex2f(vertex[0], vertex[1]);
    }

    // End rendering
    glEnd();
}

} // namespace priv

} // namespace sf
