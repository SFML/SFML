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

#pragma once

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics/VertexBuffer.hpp>

#include <SFML/Window/GlResource.hpp>

#include <cstddef>


namespace sf::priv
{
////////////////////////////////////////////////////////////
/// \brief Abstract base class for vertex buffer implementations
///
////////////////////////////////////////////////////////////
class VertexBufferImpl : protected GlResource
{
public:
    ////////////////////////////////////////////////////////////
    /// \brief Destructor
    ///
    ////////////////////////////////////////////////////////////
    virtual ~VertexBufferImpl() = default;

    ////////////////////////////////////////////////////////////
    /// \brief Create the vertex buffer
    ///
    /// \param usage       Usage specifier
    /// \param vertexCount Number of vertices worth of memory to allocate
    ///
    /// \return `true` if creation has been successful
    ///
    ////////////////////////////////////////////////////////////
    virtual bool create(VertexBuffer::Usage usage, std::size_t vertexCount) = 0;

    ////////////////////////////////////////////////////////////
    /// \brief Update the whole buffer from an array of vertices
    ///
    /// \param usage       Usage specifier
    /// \param size         Number of vertices in the buffer
    /// \param vertices    Array of vertices to copy to the buffer
    /// \param vertexCount Number of vertices to copy
    /// \param offset      Offset in the buffer to copy to
    ///
    /// \return `true` if the update was successful
    ///
    ////////////////////////////////////////////////////////////
    virtual bool update(VertexBuffer::Usage usage, std::size_t& size, const Vertex* vertices, std::size_t vertexCount, unsigned int offset) = 0;

    ////////////////////////////////////////////////////////////
    /// \brief Copy the contents of another buffer into this buffer
    ///
    /// \param usage        Usage specifier
    /// \param size         Number of vertices in the buffer
    /// \param vertexBuffer Vertex buffer whose contents to copy into this vertex buffer
    ///
    /// \return `true` if the copy was successful
    ///
    ////////////////////////////////////////////////////////////
    virtual bool update(VertexBuffer::Usage usage, std::size_t& size, const VertexBuffer& vertexBuffer) = 0;

    ////////////////////////////////////////////////////////////
    /// \brief Get the underlying native handle of the vertex buffer
    ///
    /// \return Native handle of the vertex buffer or 0 if not yet created
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] virtual unsigned int getNativeHandle() const = 0;

    ////////////////////////////////////////////////////////////
    /// \brief Bind a vertex buffer for rendering
    ///
    /// \param vertexBuffer Pointer to the vertex buffer to bind, can be null to use no vertex buffer
    ///
    ////////////////////////////////////////////////////////////
    static void bind(const VertexBuffer* vertexBuffer);

    ////////////////////////////////////////////////////////////
    /// \brief Tell whether or not the system supports vertex buffers
    ///
    /// \return `true` if vertex buffers are supported, `false` otherwise
    ///
    ////////////////////////////////////////////////////////////
    static bool isAvailable();
};

} // namespace sf::priv
