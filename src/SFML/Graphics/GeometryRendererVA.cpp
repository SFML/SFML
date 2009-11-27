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
#include <SFML/Graphics/GeometryRendererVA.hpp>
#include <SFML/Graphics/GLCheck.hpp>


namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
bool GeometryRendererVA::IsSupported()
{
    priv::EnsureGlewInit();

    return GLEW_EXT_vertex_array != 0;
}


////////////////////////////////////////////////////////////
GeometryRendererVA::GeometryRendererVA() :
myIndices(NULL)
{
    priv::EnsureGlewInit();
}


////////////////////////////////////////////////////////////
void GeometryRendererVA::Begin(const float* vertices, std::size_t verticesCount, const std::size_t* indices, std::size_t)
{
    static const GLsizei stride = 8 * sizeof(float);

    // Setup positions
    GLCheck(glEnableClientState(GL_VERTEX_ARRAY));
    GLCheck(glVertexPointer(2, GL_FLOAT, stride, vertices));

    // Setup colors
    GLCheck(glEnableClientState(GL_COLOR_ARRAY));
    GLCheck(glColorPointer(4, GL_FLOAT, stride, vertices + 2));

    // Setup texture coordinates
    GLCheck(glClientActiveTextureARB(GL_TEXTURE0_ARB));
    GLCheck(glEnableClientState(GL_TEXTURE_COORD_ARRAY));
    GLCheck(glTexCoordPointer(2, GL_FLOAT, stride, vertices + 6));

    // Lock (compile) the vertex array if supported
    if (GLEW_EXT_compiled_vertex_array)
        GLCheck(glLockArraysEXT(0, static_cast<GLsizei>(verticesCount) / 8));

    // Store indices for later use
    myIndices = indices;
}


////////////////////////////////////////////////////////////
void GeometryRendererVA::End()
{
    // Unlock the vertex array if it was locked
    if (GLEW_EXT_compiled_vertex_array)
        GLCheck(glUnlockArraysEXT());
}


////////////////////////////////////////////////////////////
void GeometryRendererVA::RenderTriangles(std::size_t start, std::size_t count)
{
    GLCheck(glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(count), GL_UNSIGNED_INT, myIndices + start));
}

} // namespace priv

} // namespace sf
