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
#include <SFML/System/Err.hpp>


namespace sf
{
////////////////////////////////////////////////////////////
VertexBatch::VertexBatch(RenderTarget& target) :
VertexArray(Triangles),
m_target(target)
{
}


////////////////////////////////////////////////////////////
void VertexBatch::draw(const Vertex* vertices, std::size_t vertexCount,
                       PrimitiveType type, const RenderStates& states)
{
    // Nothing to batch?
    if (!vertices || (vertexCount == 0))
        return;

    // Transform all vertices on the CPU
    for (std::size_t i = 0; i < vertexCount; i++)
    {
        Vertex vert = vertices[i];
        vert.position = states.transform.transformPoint(vert.position.x, vert.position.y);
    }

	// Batch vertices mapped to triangles
	switch (type)
	{
		case Points:
		case Lines:
		case LineStrip:
			err() << "sf::Points/Lines/LineStrip cannot be converted to triangles when batching, skipped" << std::endl;
			return;

		case Triangles:
			// Batch triangles as-is
			for (std::size_t i = 0; i < vertexCount; i++)
			{
				append(vertices[i]);
			}
			break;
		case TriangleStrip:
			// Map strips 0/1/2/3/4 to 0/1/2 1/2/3 2/3/4 etc.
			for (std::size_t i = 0; i < vertexCount - 2; i++)
			{
				append(vertices[i]);
				append(vertices[i + 1]);
				append(vertices[i + 2]);
			}
			break;
		case TriangleFan:
		{
			// Map fan 0/1/2/3/4 to 0/1/2 0/2/3 0/3/4 etc.
			const Vertex& center = vertices[0];
			for (std::size_t i = 1; i < vertexCount - 1; i++)
			{
				append(center);
				append(vertices[i]);
				append(vertices[i + 1]);
			}
			break;
		}
		case Quads:
			// Map quad 0/1/2/3 to 0/1/2 1/2/3
			for (std::size_t i = 0; i < vertexCount; i += 4)
			{
				append(vertices[i]);
				append(vertices[i + 1]);
				append(vertices[i + 2]);

				append(vertices[i + 1]);
				append(vertices[i + 2]);
				append(vertices[i + 3]);
			}
			break;
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
