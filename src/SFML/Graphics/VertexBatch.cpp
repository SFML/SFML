////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2021 Laurent Gomila (laurent@sfml-dev.org)
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
#include <SFML/Graphics/VertexBatch.hpp>


namespace sf
{
////////////////////////////////////////////////////////////
VertexBatch::VertexBatch(RenderTarget& target, PrimitiveType type) :
VertexArray(type),
m_target(target)
{
}


////////////////////////////////////////////////////////////
void VertexBatch::draw(const Vertex* vertices, std::size_t vertexCount,
                       PrimitiveType type, const RenderStates& states)
{
    // Nothing to draw?
    if (!vertices || (vertexCount == 0))
        return;

    // TODO: map vertices from type to gPT()

    // GL_QUADS is unavailable on OpenGL ES
    #ifdef SFML_OPENGL_ES
        if (getPrimitiveType() == Quads)
        {
            err() << "sf::Quads primitive type is not supported on OpenGL ES platforms, drawing skipped" << std::endl;
            return;
        }
    #endif

    // Append all the vertices, transformed by the CPU
    for (std::size_t i = 0; i < vertexCount; i++)
    {
        Vertex vert = vertices[i];
        vert.position = states.transform.transformPoint(vert.position.x, vert.position.y);
        append(vert);
    }
}


////////////////////////////////////////////////////////////
Vector2u VertexBatch::getSize() const
{
    return m_target.getSize();
}

////////////////////////////////////////////////////////////
bool VertexBatch::setActive(bool)
{
    // Since a vertex batch doesn't draw anything by itself, this doesnt matter
    return true;
}

} // namespace sf
