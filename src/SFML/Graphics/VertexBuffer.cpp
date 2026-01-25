////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2025 Laurent Gomila (laurent@sfml-dev.org)
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
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/Graphics/VertexBuffer.hpp>
#include <SFML/Graphics/OpenGL/VertexBufferImplDefault.hpp>

#include <memory>
#include <utility>


namespace sf
{
////////////////////////////////////////////////////////////
VertexBuffer::VertexBuffer(PrimitiveType type) : m_primitiveType(type)
{
}


////////////////////////////////////////////////////////////
VertexBuffer::VertexBuffer() = default;


////////////////////////////////////////////////////////////
VertexBuffer::VertexBuffer(Usage usage) : m_usage(usage)
{
}


////////////////////////////////////////////////////////////
VertexBuffer::VertexBuffer(PrimitiveType type, Usage usage) : m_primitiveType(type), m_usage(usage)
{
}


////////////////////////////////////////////////////////////
VertexBuffer::VertexBuffer(const VertexBuffer& copy) :
    GlResource(copy),
    m_size(copy.m_size),
    m_primitiveType(copy.m_primitiveType),
    m_usage(copy.m_usage)
{
    if (copy.m_impl && copy.m_size)
    {
        if (!create(copy.m_size))
            return;

        if (!update(copy))
        {
            // Error already logged by update()
        }
    }
}


////////////////////////////////////////////////////////////
VertexBuffer::~VertexBuffer() = default;


////////////////////////////////////////////////////////////
bool VertexBuffer::create(std::size_t vertexCount)
{
    if (!m_impl)
        m_impl = std::make_unique<priv::VertexBufferImplDefault>();

    if (m_impl->create(m_usage, vertexCount))
    {
        m_size = vertexCount;
        return true;
    }

    return false;
}


////////////////////////////////////////////////////////////
std::size_t VertexBuffer::getVertexCount() const
{
    return m_size;
}


////////////////////////////////////////////////////////////
bool VertexBuffer::update(const Vertex* vertices)
{
    return update(vertices, m_size, 0);
}


////////////////////////////////////////////////////////////
bool VertexBuffer::update(const Vertex* vertices, std::size_t vertexCount, unsigned int offset)
{
    if (!m_impl)
        return false;

    return m_impl->update(m_usage, m_size, vertices, vertexCount, offset);
}


////////////////////////////////////////////////////////////
bool VertexBuffer::update(const VertexBuffer& vertexBuffer)
{
    if (!m_impl)
        return false;

    return m_impl->update(m_usage, m_size, vertexBuffer);
}


////////////////////////////////////////////////////////////
VertexBuffer& VertexBuffer::operator=(const VertexBuffer& right)
{
    VertexBuffer temp(right);

    swap(temp);

    return *this;
}


////////////////////////////////////////////////////////////
void VertexBuffer::swap(VertexBuffer& right) noexcept
{
    std::swap(m_impl, right.m_impl);
    std::swap(m_size, right.m_size);
    std::swap(m_primitiveType, right.m_primitiveType);
    std::swap(m_usage, right.m_usage);
}


////////////////////////////////////////////////////////////
unsigned int VertexBuffer::getNativeHandle() const
{
    return m_impl ? m_impl->getNativeHandle() : 0;
}


////////////////////////////////////////////////////////////
void VertexBuffer::bind(const VertexBuffer* vertexBuffer)
{
    priv::VertexBufferImpl::bind(vertexBuffer);
}


////////////////////////////////////////////////////////////
void VertexBuffer::setPrimitiveType(PrimitiveType type)
{
    m_primitiveType = type;
}


////////////////////////////////////////////////////////////
PrimitiveType VertexBuffer::getPrimitiveType() const
{
    return m_primitiveType;
}


////////////////////////////////////////////////////////////
void VertexBuffer::setUsage(Usage usage)
{
    m_usage = usage;
}


////////////////////////////////////////////////////////////
VertexBuffer::Usage VertexBuffer::getUsage() const
{
    return m_usage;
}


////////////////////////////////////////////////////////////
bool VertexBuffer::isAvailable()
{
    return priv::VertexBufferImpl::isAvailable();
}


////////////////////////////////////////////////////////////
void VertexBuffer::draw(RenderTarget& target, RenderStates states) const
{
    if (m_size)
    {
        if (m_impl)
            target.draw(*this, 0, m_size, states);
    }
}


////////////////////////////////////////////////////////////
void swap(VertexBuffer& left, VertexBuffer& right) noexcept
{
    left.swap(right);
}

} // namespace sf

