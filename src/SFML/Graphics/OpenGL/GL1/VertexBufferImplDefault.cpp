////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2019 Laurent Gomila (laurent@sfml-dev.org)
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
#include <SFML/Graphics/OpenGL/GL1/VertexBufferImplDefault.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/Graphics/OpenGL/GLCheck.hpp>
#include <SFML/System/Mutex.hpp>
#include <SFML/System/Lock.hpp>
#include <SFML/System/Err.hpp>
#include <cstring>

namespace
{
    sf::Mutex isAvailableMutex;

    GLenum usageToGlEnum(sf::VertexBuffer::Usage usage)
    {
        switch (usage)
        {
            case sf::VertexBuffer::Static:  return GLEXT_GL_STATIC_DRAW;
            case sf::VertexBuffer::Dynamic: return GLEXT_GL_DYNAMIC_DRAW;
            default:                        return GLEXT_GL_STREAM_DRAW;
        }
    }
}


namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
VertexBufferImplDefault::VertexBufferImplDefault() :
m_buffer(0),
m_size  (0),
m_usage (VertexBuffer::Stream)
{
}


////////////////////////////////////////////////////////////
VertexBufferImplDefault::VertexBufferImplDefault(VertexBuffer::Usage usage) :
m_buffer(0),
m_size  (0),
m_usage (usage)
{
}


////////////////////////////////////////////////////////////
VertexBufferImplDefault::~VertexBufferImplDefault()
{
    if (m_buffer)
    {
        TransientContextLock contextLock;

        glCheck(GLEXT_glDeleteBuffers(1, &m_buffer));
    }
}


////////////////////////////////////////////////////////////
bool VertexBufferImplDefault::create(std::size_t vertexCount)
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
    glCheck(GLEXT_glBufferData(GLEXT_GL_ARRAY_BUFFER, sizeof(Vertex) * vertexCount, 0, usageToGlEnum(m_usage)));
    glCheck(GLEXT_glBindBuffer(GLEXT_GL_ARRAY_BUFFER, 0));

    m_size = vertexCount;

    return true;
}


////////////////////////////////////////////////////////////
std::size_t VertexBufferImplDefault::getVertexCount() const
{
    return m_size;
}


////////////////////////////////////////////////////////////
bool VertexBufferImplDefault::update(const Vertex* vertices, std::size_t vertexCount, unsigned int offset)
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
        glCheck(GLEXT_glBufferData(GLEXT_GL_ARRAY_BUFFER, sizeof(Vertex) * vertexCount, 0, usageToGlEnum(m_usage)));

        m_size = vertexCount;
    }

    glCheck(GLEXT_glBufferSubData(GLEXT_GL_ARRAY_BUFFER, sizeof(Vertex) * offset, sizeof(Vertex) * vertexCount, vertices));

    glCheck(GLEXT_glBindBuffer(GLEXT_GL_ARRAY_BUFFER, 0));

    return true;
}


////////////////////////////////////////////////////////////
bool VertexBufferImplDefault::update(const VertexBufferImpl& vertexBuffer)
{
#ifdef SFML_OPENGL_ES

    return false;

#else

    const VertexBufferImplDefault& other = static_cast<const VertexBufferImplDefault&>(vertexBuffer);

    if (!m_buffer || !other.m_buffer)
        return false;

    TransientContextLock contextLock;

    // Make sure that extensions are initialized
    sf::priv::ensureExtensionsInit();

    if (GLEXT_copy_buffer)
    {
        glCheck(GLEXT_glBindBuffer(GLEXT_GL_COPY_READ_BUFFER, other.m_buffer));
        glCheck(GLEXT_glBindBuffer(GLEXT_GL_COPY_WRITE_BUFFER, m_buffer));

        glCheck(GLEXT_glCopyBufferSubData(GLEXT_GL_COPY_READ_BUFFER, GLEXT_GL_COPY_WRITE_BUFFER, 0, 0, sizeof(Vertex) * other.m_size));

        glCheck(GLEXT_glBindBuffer(GLEXT_GL_COPY_WRITE_BUFFER, 0));
        glCheck(GLEXT_glBindBuffer(GLEXT_GL_COPY_READ_BUFFER, 0));

        return true;
    }

    glCheck(GLEXT_glBindBuffer(GLEXT_GL_ARRAY_BUFFER, m_buffer));
    glCheck(GLEXT_glBufferData(GLEXT_GL_ARRAY_BUFFER, sizeof(Vertex) * other.m_size, 0, usageToGlEnum(m_usage)));

    void* destination = 0;
    glCheck(destination = GLEXT_glMapBuffer(GLEXT_GL_ARRAY_BUFFER, GLEXT_GL_WRITE_ONLY));

    glCheck(GLEXT_glBindBuffer(GLEXT_GL_ARRAY_BUFFER, other.m_buffer));

    void* source = 0;
    glCheck(source = GLEXT_glMapBuffer(GLEXT_GL_ARRAY_BUFFER, GLEXT_GL_READ_ONLY));

    std::memcpy(destination, source, sizeof(Vertex) * other.m_size);

    GLboolean sourceResult = GL_FALSE;
    glCheck(sourceResult = GLEXT_glUnmapBuffer(GLEXT_GL_ARRAY_BUFFER));

    glCheck(GLEXT_glBindBuffer(GLEXT_GL_ARRAY_BUFFER, m_buffer));

    GLboolean destinationResult = GL_FALSE;
    glCheck(destinationResult = GLEXT_glUnmapBuffer(GLEXT_GL_ARRAY_BUFFER));

    glCheck(GLEXT_glBindBuffer(GLEXT_GL_ARRAY_BUFFER, 0));

    if ((sourceResult == GL_FALSE) || (destinationResult == GL_FALSE))
        return false;

    return true;

#endif // SFML_OPENGL_ES
}


////////////////////////////////////////////////////////////
unsigned int VertexBufferImplDefault::getNativeHandle() const
{
    return m_buffer;
}


////////////////////////////////////////////////////////////
void VertexBufferImplDefault::bind(const VertexBufferImplDefault* vertexBuffer)
{
    if (!isAvailable())
        return;

    TransientContextLock lock;

    glCheck(GLEXT_glBindBuffer(GLEXT_GL_ARRAY_BUFFER, vertexBuffer ? vertexBuffer->m_buffer : 0));
}


////////////////////////////////////////////////////////////
void VertexBufferImplDefault::setUsage(VertexBuffer::Usage usage)
{
    m_usage = usage;
}


////////////////////////////////////////////////////////////
VertexBuffer::Usage VertexBufferImplDefault::getUsage() const
{
    return m_usage;
}


////////////////////////////////////////////////////////////
bool VertexBufferImplDefault::isAvailable()
{
    Lock lock(isAvailableMutex);

    static bool checked = false;
    static bool available = false;

    if (!checked)
    {
        checked = true;

        TransientContextLock contextLock;

        // Make sure that extensions are initialized
        sf::priv::ensureExtensionsInit();

        available = GLEXT_vertex_buffer_object;
    }

    return available;
}

} // namespace priv

} // namespace sf
