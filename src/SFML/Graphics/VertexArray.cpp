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
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/RenderTarget.hpp>


namespace sf
{
////////////////////////////////////////////////////////////
VertexArray::VertexArray() :
myVertices     (),
myPrimitiveType(Points)
{
}


////////////////////////////////////////////////////////////
VertexArray::VertexArray(PrimitiveType type, unsigned int vertexCount) :
myVertices     (vertexCount),
myPrimitiveType(type)
{
}


////////////////////////////////////////////////////////////
unsigned int VertexArray::GetVertexCount() const
{
    return myVertices.size();
}


////////////////////////////////////////////////////////////
Vertex& VertexArray::operator [](unsigned int index)
{
    return myVertices[index];
}


////////////////////////////////////////////////////////////
const Vertex& VertexArray::operator [](unsigned int index) const
{
    return myVertices[index];
}


////////////////////////////////////////////////////////////
void VertexArray::Clear()
{
    myVertices.clear();
}


////////////////////////////////////////////////////////////
void VertexArray::Resize(unsigned int vertexCount)
{
    myVertices.resize(vertexCount);
}


////////////////////////////////////////////////////////////
void VertexArray::Append(const Vertex& vertex)
{
    myVertices.push_back(vertex);
}


////////////////////////////////////////////////////////////
void VertexArray::SetPrimitiveType(PrimitiveType type)
{
    myPrimitiveType = type;
}


////////////////////////////////////////////////////////////
PrimitiveType VertexArray::GetPrimitiveType() const
{
    return myPrimitiveType;
}


////////////////////////////////////////////////////////////
FloatRect VertexArray::GetBounds() const
{
    if (!myVertices.empty())
    {
        float left   = myVertices[0].Position.x;
        float top    = myVertices[0].Position.y;
        float right  = myVertices[0].Position.x;
        float bottom = myVertices[0].Position.y;

        for (std::size_t i = 0; i < myVertices.size(); ++i)
        {
            Vector2f position = myVertices[i].Position;

            // Update left and right
            if (position.x < left)
                left = position.x;
            else if (position.x > right)
                right = position.x;

            // Update top and bottom
            if (position.y < top)
                top = position.y;
            else if (position.y > bottom)
                bottom = position.y;
        }

        return FloatRect(left, top, right - left, bottom - top);
    }
    else
    {
        // Array is empty
        return FloatRect();
    }
}


////////////////////////////////////////////////////////////
void VertexArray::Draw(RenderTarget& target, RenderStates states) const
{
    if (!myVertices.empty())
        target.Draw(&myVertices[0], myVertices.size(), myPrimitiveType, states);
}

} // namespace sf
