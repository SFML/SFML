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

#ifndef SFML_VERTEXBUFFER_IMPL_DEFAULT_HPP
#define SFML_VERTEXBUFFER_IMPL_DEFAULT_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics/VertexBufferImpl.hpp>
#include <SFML/Window/GlResource.hpp>


namespace sf
{
class RenderTarget;
class Vertex;

namespace priv
{
////////////////////////////////////////////////////////////
/// \brief Default specialization of VertexBufferImpl,
///        using OpenGL VBOs
///
////////////////////////////////////////////////////////////
class VertexBufferImplDefault : public VertexBufferImpl, GlResource
{
public:

    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    /// Creates an empty vertex buffer.
    ///
    ////////////////////////////////////////////////////////////
    VertexBufferImplDefault();

    ////////////////////////////////////////////////////////////
    /// \brief Construct a VertexBuffer with a specific usage specifier
    ///
    /// Creates an empty vertex buffer and sets its usage to \p usage.
    ///
    /// \param usage Usage specifier
    ///
    ////////////////////////////////////////////////////////////
    explicit VertexBufferImplDefault(VertexBuffer::Usage usage);

    ////////////////////////////////////////////////////////////
    /// \brief Destructor
    ///
    ////////////////////////////////////////////////////////////
    ~VertexBufferImplDefault();

    ////////////////////////////////////////////////////////////
    /// \brief Bind a vertex buffer for rendering
    ///
    /// \param vertexBuffer Pointer to the vertex buffer to bind, can be null to use no vertex buffer
    ///
    ////////////////////////////////////////////////////////////
    static void bind(const VertexBufferImplDefault* vertexBuffer);

    ////////////////////////////////////////////////////////////
    /// \brief Tell whether or not the system supports vertex buffers
    ///
    /// \return True if vertex buffers are supported, false otherwise
    ///
    ////////////////////////////////////////////////////////////
    static bool isAvailable();

    ////////////////////////////////////////////////////////////
    /// \brief Create the vertex buffer
    ///
    /// \param vertexCount Number of vertices worth of memory to allocate
    ///
    /// \return True if creation was successful
    ///
    ////////////////////////////////////////////////////////////
    virtual bool create(std::size_t vertexCount);

    ////////////////////////////////////////////////////////////
    /// \brief Return the vertex count
    ///
    /// \return Number of vertices in the vertex buffer
    ///
    ////////////////////////////////////////////////////////////
    virtual std::size_t getVertexCount() const;

    ////////////////////////////////////////////////////////////
    /// \brief Update a part of the buffer from an array of vertices
    ///
    /// \param vertices    Array of vertices to copy to the buffer
    /// \param vertexCount Number of vertices to copy
    /// \param offset      Offset in the buffer to copy to
    ///
    /// \return True if the update was successful
    ///
    ////////////////////////////////////////////////////////////
    virtual bool update(const Vertex* vertices, std::size_t vertexCount, unsigned int offset);

    ////////////////////////////////////////////////////////////
    /// \brief Copy the contents of another buffer into this buffer
    ///
    /// \param vertexBuffer Vertex buffer whose contents to copy into this vertex buffer
    ///
    /// \return True if the copy was successful
    ///
    ////////////////////////////////////////////////////////////
    virtual bool update(const VertexBufferImpl& vertexBuffer);

    ////////////////////////////////////////////////////////////
    /// \brief Get the underlying OpenGL handle of the vertex buffer.
    ///
    /// \return OpenGL handle of the vertex buffer or 0 if not yet created
    ///
    ////////////////////////////////////////////////////////////
    virtual unsigned int getNativeHandle() const;

    ////////////////////////////////////////////////////////////
    /// \brief Set the usage specifier of this vertex buffer
    ///
    /// \param usage Usage specifier
    ///
    ////////////////////////////////////////////////////////////
    virtual void setUsage(VertexBuffer::Usage usage);

    ////////////////////////////////////////////////////////////
    /// \brief Get the usage specifier of this vertex buffer
    ///
    /// \return Usage specifier
    ///
    ////////////////////////////////////////////////////////////
    virtual VertexBuffer::Usage getUsage() const;

private:

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    unsigned int        m_buffer; ///< Internal buffer identifier
    std::size_t         m_size;   ///< Size in Vertexes of the currently allocated buffer
    VertexBuffer::Usage m_usage;  ///< How this vertex buffer is to be used
};

} // namespace priv

} // namespace sf


#endif // SFML_VERTEXBUFFER_IMPL_DEFAULT_HPP
