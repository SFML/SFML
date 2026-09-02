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
#include <SFML/Graphics/VertexBufferImpl.hpp>

#include <SFML/Window/GlResource.hpp>


namespace sf::priv
{
////////////////////////////////////////////////////////////
/// \brief Default OpenGL implementation of vertex buffers
///
////////////////////////////////////////////////////////////
class VertexBufferImplDefault : public VertexBufferImpl
{
public:
    ////////////////////////////////////////////////////////////
    /// \brief Destructor
    ///
    ////////////////////////////////////////////////////////////
    ~VertexBufferImplDefault() override;

    ////////////////////////////////////////////////////////////
    /// \brief Create the vertex buffer
    ///
    /// \param usage       Usage specifier
    /// \param vertexCount Number of vertices worth of memory to allocate
    ///
    /// \return `true` if creation has been successful
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] bool create(VertexBuffer::Usage usage, std::size_t vertexCount) override;

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
    [[nodiscard]] bool update(VertexBuffer::Usage usage, std::size_t& size, const Vertex* vertices, std::size_t vertexCount, unsigned int offset) override;

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
    [[nodiscard]] bool update(VertexBuffer::Usage usage, std::size_t& size, const VertexBuffer& vertexBuffer) override;

    ////////////////////////////////////////////////////////////
    /// \brief Get the underlying native handle of the vertex buffer
    ///
    /// \return Native handle of the vertex buffer or 0 if not yet created
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] unsigned int getNativeHandle() const override;

private:
    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    unsigned int m_buffer{}; //!< Internal buffer identifier
};

} // namespace sf::priv
