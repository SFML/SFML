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
#include <SFML/Graphics/GeometryRendererVBO.hpp>


namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
bool GeometryRendererVBO::IsSupported()
{
    priv::EnsureGlewInit();

    return GLEW_ARB_vertex_buffer_object != 0;
}


////////////////////////////////////////////////////////////
GeometryRendererVBO::GeometryRendererVBO() :
myVertexBufferSize(0),
myIndexBufferSize (0)
{
    priv::EnsureGlewInit();

    // Create the buffers
    GLCheck(glGenBuffersARB(1, &myVertexBuffer));
    GLCheck(glGenBuffersARB(1, &myIndexBuffer));
}


////////////////////////////////////////////////////////////
GeometryRendererVBO::~GeometryRendererVBO()
{
    // Free the buffers
    GLCheck(glDeleteBuffersARB(1, &myVertexBuffer));
    GLCheck(glDeleteBuffersARB(1, &myIndexBuffer));
}


////////////////////////////////////////////////////////////
void GeometryRendererVBO::Begin(const float* vertices, std::size_t verticesCount, const Uint32* indices, std::size_t indicesCount)
{
    // Update the vertex buffer data (make it grow if it is not large enough)
    GLCheck(glBindBufferARB(GL_ARRAY_BUFFER_ARB, myVertexBuffer));
    if (verticesCount > myVertexBufferSize)
    {
        GLCheck(glBufferDataARB(GL_ARRAY_BUFFER_ARB, verticesCount * sizeof(float), vertices, GL_DYNAMIC_DRAW_ARB));
        myVertexBufferSize = verticesCount;
    }
    else
    {
        GLCheck(glBufferSubDataARB(GL_ARRAY_BUFFER_ARB, 0, verticesCount * sizeof(float), vertices));
    }

    // Update the index buffer data (make it grow if it is not large enough)
    GLCheck(glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, myIndexBuffer));
    if (indicesCount > myIndexBufferSize)
    {
        GLCheck(glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB, indicesCount * sizeof(Uint32), indices, GL_DYNAMIC_DRAW_ARB));
        myIndexBufferSize = indicesCount;
    }
    else
    {
        GLCheck(glBufferSubDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB, 0, indicesCount * sizeof(Uint32), indices));
    }

    static const GLsizei stride  = 8 * sizeof(float);
    static const float*  pointer = NULL;

    // Setup positions
    GLCheck(glEnableClientState(GL_VERTEX_ARRAY));
    GLCheck(glVertexPointer(2, GL_FLOAT, stride, pointer));

    // Setup colors
    GLCheck(glEnableClientState(GL_COLOR_ARRAY));
    GLCheck(glColorPointer(4, GL_FLOAT, stride, pointer + 2));

    // Setup texture coordinates
    GLCheck(glClientActiveTextureARB(GL_TEXTURE0_ARB));
    GLCheck(glEnableClientState(GL_TEXTURE_COORD_ARRAY));
    GLCheck(glTexCoordPointer(2, GL_FLOAT, stride, pointer + 6));
}


////////////////////////////////////////////////////////////
void GeometryRendererVBO::End()
{
    // Nothing to do
}


////////////////////////////////////////////////////////////
void GeometryRendererVBO::RenderTriangles(Uint32 start, Uint32 count)
{
    static const UInt32* pointer = NULL;
    GLCheck(glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(count), GL_UNSIGNED_INT, pointer + start));
}

} // namespace priv

} // namespace sf
