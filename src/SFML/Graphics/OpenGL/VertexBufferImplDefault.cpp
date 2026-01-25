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
#include <SFML/Graphics/OpenGL/VertexBufferImplDefault.hpp>
#include <SFML/Graphics/GLCheck.hpp>
#include <SFML/Graphics/GLExtensions.hpp>
#include <SFML/Graphics/Vertex.hpp>

#include <SFML/Window/GlResource.hpp>

#include <SFML/System/Err.hpp>

#include <ostream>
#include <cstring>


namespace
{
GLenum usageToGlEnum(sf::VertexBuffer::Usage usage)
{
    switch (usage)
    {
        case sf::VertexBuffer::Usage::Static:
            return GLEXT_GL_STATIC_DRAW;
        case sf::VertexBuffer::Usage::Dynamic:
            return GLEXT_GL_DYNAMIC_DRAW;
        default:
            return GLEXT_GL_STREAM_DRAW;
    }
}
} // namespace


namespace sf::priv
{
////////////////////////////////////////////////////////////
VertexBufferImplDefault::~VertexBufferImplDefault()
{
    if (m_buffer)
    {
        const GlResource::TransientContextLock contextLock;

        glCheck(GLEXT_glDeleteBuffers(1, &m_buffer));
    }
}


////////////////////////////////////////////////////////////
bool VertexBufferImplDefault::create(VertexBuffer::Usage usage, std::size_t vertexCount)
{
    if (!VertexBufferImpl::isAvailable())
        return false;

    const GlResource::TransientContextLock contextLock;

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
                               usageToGlEnum(usage)));
    glCheck(GLEXT_glBindBuffer(GLEXT_GL_ARRAY_BUFFER, 0));

    return true;
}


////////////////////////////////////////////////////////////
bool VertexBufferImplDefault::update(VertexBuffer::Usage usage, std::size_t& size, const Vertex* vertices, std::size_t vertexCount, unsigned int offset)
{
    // Sanity checks
    if (!m_buffer)
        return false;

    if (!vertices)
        return false;

    if (offset && (offset + vertexCount > size))
        return false;

    const GlResource::TransientContextLock contextLock;

    glCheck(GLEXT_glBindBuffer(GLEXT_GL_ARRAY_BUFFER, m_buffer));

    // Check if we need to resize or orphan the buffer
    if (vertexCount >= size)
    {
        glCheck(GLEXT_glBufferData(GLEXT_GL_ARRAY_BUFFER,
                                   static_cast<GLsizeiptrARB>(sizeof(Vertex) * vertexCount),
                                   nullptr,
                                   usageToGlEnum(usage)));

        size = vertexCount;
    }

    glCheck(GLEXT_glBufferSubData(GLEXT_GL_ARRAY_BUFFER,
                                  static_cast<GLintptrARB>(sizeof(Vertex) * offset),
                                  static_cast<GLsizeiptrARB>(sizeof(Vertex) * vertexCount),
                                  vertices));

    glCheck(GLEXT_glBindBuffer(GLEXT_GL_ARRAY_BUFFER, 0));

    return true;
}


////////////////////////////////////////////////////////////
bool VertexBufferImplDefault::update(VertexBuffer::Usage usage, std::size_t& size, const VertexBuffer& vertexBuffer)
{
#ifdef SFML_OPENGL_ES

    return false;

#else

    if (!m_buffer || !vertexBuffer.getNativeHandle())
        return false;

    const GlResource::TransientContextLock contextLock;

    // Make sure that extensions are initialized
    ensureExtensionsInit();

    if (GLEXT_copy_buffer)
    {
        glCheck(GLEXT_glBindBuffer(GLEXT_GL_COPY_READ_BUFFER, vertexBuffer.getNativeHandle()));
        glCheck(GLEXT_glBindBuffer(GLEXT_GL_COPY_WRITE_BUFFER, m_buffer));

        glCheck(GLEXT_glCopyBufferSubData(GLEXT_GL_COPY_READ_BUFFER,
                                          GLEXT_GL_COPY_WRITE_BUFFER,
                                          0,
                                          0,
                                          static_cast<GLsizeiptr>(sizeof(Vertex) * vertexBuffer.getVertexCount())));

        glCheck(GLEXT_glBindBuffer(GLEXT_GL_COPY_WRITE_BUFFER, 0));
        glCheck(GLEXT_glBindBuffer(GLEXT_GL_COPY_READ_BUFFER, 0));

        return true;
    }

    glCheck(GLEXT_glBindBuffer(GLEXT_GL_ARRAY_BUFFER, m_buffer));
    glCheck(GLEXT_glBufferData(GLEXT_GL_ARRAY_BUFFER,
                               static_cast<GLsizeiptrARB>(sizeof(Vertex) * vertexBuffer.getVertexCount()),
                               nullptr,
                               usageToGlEnum(usage)));

    void* const destination = glCheck(GLEXT_glMapBuffer(GLEXT_GL_ARRAY_BUFFER, GLEXT_GL_WRITE_ONLY));

    glCheck(GLEXT_glBindBuffer(GLEXT_GL_ARRAY_BUFFER, vertexBuffer.getNativeHandle()));

    const void* const source = glCheck(GLEXT_glMapBuffer(GLEXT_GL_ARRAY_BUFFER, GLEXT_GL_READ_ONLY));

    std::memcpy(destination, source, sizeof(Vertex) * vertexBuffer.getVertexCount());

    const GLboolean sourceResult = glCheck(GLEXT_glUnmapBuffer(GLEXT_GL_ARRAY_BUFFER));

    glCheck(GLEXT_glBindBuffer(GLEXT_GL_ARRAY_BUFFER, m_buffer));

    const GLboolean destinationResult = glCheck(GLEXT_glUnmapBuffer(GLEXT_GL_ARRAY_BUFFER));

    glCheck(GLEXT_glBindBuffer(GLEXT_GL_ARRAY_BUFFER, 0));

    size = vertexBuffer.getVertexCount();

    return (sourceResult == GL_TRUE) && (destinationResult == GL_TRUE);

#endif // SFML_OPENGL_ES
}


////////////////////////////////////////////////////////////
unsigned int VertexBufferImplDefault::getNativeHandle() const
{
    return m_buffer;
}


////////////////////////////////////////////////////////////
void VertexBufferImpl::bind(const VertexBuffer* vertexBuffer)
{
    if (!isAvailable())
        return;

    const GlResource::TransientContextLock lock;

    glCheck(GLEXT_glBindBuffer(GLEXT_GL_ARRAY_BUFFER, vertexBuffer ? vertexBuffer->getNativeHandle() : 0));
}


////////////////////////////////////////////////////////////
bool VertexBufferImpl::isAvailable()
{
    const GlResource::TransientContextLock lock;

    return GLEXT_vertex_buffer_object != 0;
}

} // namespace sf::priv
