////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2023 Laurent Gomila (laurent@sfml-dev.org)
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
#include <SFML/Graphics/GLCheck.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/Graphics/VertexBuffer.hpp>

#include <SFML/System/Err.hpp>

#include <ostream>
#include <utility>

#include <cstddef>
#include <cstring>

namespace
{
// A nested named namespace is used here to allow unity builds of SFML.
namespace VertexBufferImpl
{
GLenum usageToGlEnum(sf::VertexBuffer::Usage usage)
{
    switch (usage)
    {
        case sf::VertexBuffer::Static:
            return GLEXT_GL_STATIC_DRAW;
        case sf::VertexBuffer::Dynamic:
            return GLEXT_GL_DYNAMIC_DRAW;
        default:
            return GLEXT_GL_STREAM_DRAW;
    }
}
} // namespace VertexBufferImpl
} // namespace


namespace sf
{
////////////////////////////////////////////////////////////
VertexBuffer::VertexBuffer() = default;


////////////////////////////////////////////////////////////
VertexBuffer::VertexBuffer(PrimitiveType type) : m_primitiveType(type)
{
}


////////////////////////////////////////////////////////////
VertexBuffer::VertexBuffer(Usage usage) : m_usage(usage)
{
}


////////////////////////////////////////////////////////////
VertexBuffer::VertexBuffer(PrimitiveType type, Usage usage) : m_primitiveType(type), m_usage(usage)
{
}


////////////////////////////////////////////////////////////
VertexBuffer::VertexBuffer(const VertexBuffer& copy) : m_primitiveType(copy.m_primitiveType), m_usage(copy.m_usage)
{
    if (copy.m_buffer && copy.m_size)
    {
        if (!create(copy.m_size))
        {
            err() << "Could not create vertex buffer for copying" << std::endl;
            return;
        }

        if (!update(copy))
            err() << "Could not copy vertex buffer" << std::endl;
    }
}


////////////////////////////////////////////////////////////
VertexBuffer::~VertexBuffer()
{
    if (m_buffer)
    {
        TransientContextLock contextLock;

        glCheck(GLEXT_glDeleteBuffers(1, &m_buffer));
    }
}


////////////////////////////////////////////////////////////
bool VertexBuffer::create(std::size_t vertexCount)
{
    if (!isAvailable())
        return false;

    TransientContextLock contextLock;

    if (!m_buffer)
        glCheck(GLEXT_glGenBuffers(1, &m_buffer));

    if (!m_buffer)
    {
        err() << "Could not create vertex buffer, generation failed" << std::endl;
        return false;
    }

    glCheck(GLEXT_glBindBuffer(GLEXT_GL_ARRAY_BUFFER, m_buffer));
    glCheck(GLEXT_glBufferData(GLEXT_GL_ARRAY_BUFFER,
                               static_cast<GLsizeiptrARB>(sizeof(Vertex) * vertexCount),
                               nullptr,
                               VertexBufferImpl::usageToGlEnum(m_usage)));
    glCheck(GLEXT_glBindBuffer(GLEXT_GL_ARRAY_BUFFER, 0));

    m_size = vertexCount;

    return true;
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
    // Sanity checks
    if (!m_buffer)
        return false;

    if (!vertices)
        return false;

    if (offset && (offset + vertexCount > m_size))
        return false;

    TransientContextLock contextLock;

    glCheck(GLEXT_glBindBuffer(GLEXT_GL_ARRAY_BUFFER, m_buffer));

    // Check if we need to resize or orphan the buffer
    if (vertexCount >= m_size)
    {
        glCheck(GLEXT_glBufferData(GLEXT_GL_ARRAY_BUFFER,
                                   static_cast<GLsizeiptrARB>(sizeof(Vertex) * vertexCount),
                                   nullptr,
                                   VertexBufferImpl::usageToGlEnum(m_usage)));

        m_size = vertexCount;
    }

    glCheck(GLEXT_glBufferSubData(GLEXT_GL_ARRAY_BUFFER,
                                  static_cast<GLintptrARB>(sizeof(Vertex) * offset),
                                  static_cast<GLsizeiptrARB>(sizeof(Vertex) * vertexCount),
                                  vertices));

    glCheck(GLEXT_glBindBuffer(GLEXT_GL_ARRAY_BUFFER, 0));

    return true;
}


////////////////////////////////////////////////////////////
bool VertexBuffer::update([[maybe_unused]] const VertexBuffer& vertexBuffer)
{
#ifdef SFML_OPENGL_ES

    return false;

#else

    if (!m_buffer || !vertexBuffer.m_buffer)
        return false;

    TransientContextLock contextLock;

    // Make sure that extensions are initialized
    sf::priv::ensureExtensionsInit();

    if (GLEXT_copy_buffer)
    {
        glCheck(GLEXT_glBindBuffer(GLEXT_GL_COPY_READ_BUFFER, vertexBuffer.m_buffer));
        glCheck(GLEXT_glBindBuffer(GLEXT_GL_COPY_WRITE_BUFFER, m_buffer));

        glCheck(GLEXT_glCopyBufferSubData(GLEXT_GL_COPY_READ_BUFFER,
                                          GLEXT_GL_COPY_WRITE_BUFFER,
                                          0,
                                          0,
                                          static_cast<GLsizeiptr>(sizeof(Vertex) * vertexBuffer.m_size)));

        glCheck(GLEXT_glBindBuffer(GLEXT_GL_COPY_WRITE_BUFFER, 0));
        glCheck(GLEXT_glBindBuffer(GLEXT_GL_COPY_READ_BUFFER, 0));

        return true;
    }

    glCheck(GLEXT_glBindBuffer(GLEXT_GL_ARRAY_BUFFER, m_buffer));
    glCheck(GLEXT_glBufferData(GLEXT_GL_ARRAY_BUFFER,
                               static_cast<GLsizeiptrARB>(sizeof(Vertex) * vertexBuffer.m_size),
                               nullptr,
                               VertexBufferImpl::usageToGlEnum(m_usage)));

    void* destination = nullptr;
    glCheck(destination = GLEXT_glMapBuffer(GLEXT_GL_ARRAY_BUFFER, GLEXT_GL_WRITE_ONLY));

    glCheck(GLEXT_glBindBuffer(GLEXT_GL_ARRAY_BUFFER, vertexBuffer.m_buffer));

    void* source = nullptr;
    glCheck(source = GLEXT_glMapBuffer(GLEXT_GL_ARRAY_BUFFER, GLEXT_GL_READ_ONLY));

    std::memcpy(destination, source, sizeof(Vertex) * vertexBuffer.m_size);

    GLboolean sourceResult = GL_FALSE;
    glCheck(sourceResult = GLEXT_glUnmapBuffer(GLEXT_GL_ARRAY_BUFFER));

    glCheck(GLEXT_glBindBuffer(GLEXT_GL_ARRAY_BUFFER, m_buffer));

    GLboolean destinationResult = GL_FALSE;
    glCheck(destinationResult = GLEXT_glUnmapBuffer(GLEXT_GL_ARRAY_BUFFER));

    glCheck(GLEXT_glBindBuffer(GLEXT_GL_ARRAY_BUFFER, 0));

    return (sourceResult == GL_TRUE) && (destinationResult == GL_TRUE);

#endif // SFML_OPENGL_ES
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
    std::swap(m_size, right.m_size);
    std::swap(m_buffer, right.m_buffer);
    std::swap(m_primitiveType, right.m_primitiveType);
    std::swap(m_usage, right.m_usage);
}


////////////////////////////////////////////////////////////
unsigned int VertexBuffer::getNativeHandle() const
{
    return m_buffer;
}


////////////////////////////////////////////////////////////
void VertexBuffer::bind(const VertexBuffer* vertexBuffer)
{
    if (!isAvailable())
        return;

    TransientContextLock lock;

    glCheck(GLEXT_glBindBuffer(GLEXT_GL_ARRAY_BUFFER, vertexBuffer ? vertexBuffer->m_buffer : 0));
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
    static const bool available = []() -> bool
    {
        TransientContextLock contextLock;

        // Make sure that extensions are initialized
        sf::priv::ensureExtensionsInit();

        return GLEXT_vertex_buffer_object;
    }();

    return available;
}


////////////////////////////////////////////////////////////
void VertexBuffer::draw(RenderTarget& target, const RenderStates& states) const
{
    if (m_buffer && m_size)
        target.draw(*this, 0, m_size, states);
}


////////////////////////////////////////////////////////////
void swap(VertexBuffer& left, VertexBuffer& right) noexcept
{
    left.swap(right);
}

} // namespace sf
