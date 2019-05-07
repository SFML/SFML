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

#ifndef SFML_VERTEXBUFFER_IMPL_HPP
#define SFML_VERTEXBUFFER_IMPL_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics/VertexBuffer.hpp>


namespace sf
{
class Vertex;

namespace priv
{
////////////////////////////////////////////////////////////
/// \brief Abstract base class for vertex buffer implementations
///
////////////////////////////////////////////////////////////
class VertexBufferImpl
{
public:

    ////////////////////////////////////////////////////////////
    /// \brief Destructor
    ///
    ////////////////////////////////////////////////////////////
    virtual ~VertexBufferImpl();

    ////////////////////////////////////////////////////////////
    /// \brief Create the vertex buffer
    ///
    /// \param vertexCount Number of vertices worth of memory to allocate
    ///
    /// \return True if creation was successful
    ///
    ////////////////////////////////////////////////////////////
    virtual bool create(std::size_t vertexCount) = 0;

    ////////////////////////////////////////////////////////////
    /// \brief Return the vertex count
    ///
    /// \return Number of vertices in the vertex buffer
    ///
    ////////////////////////////////////////////////////////////
    virtual std::size_t getVertexCount() const = 0;

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
    virtual bool update(const Vertex* vertices, std::size_t vertexCount, unsigned int offset) = 0;

    ////////////////////////////////////////////////////////////
    /// \brief Copy the contents of another buffer into this buffer
    ///
    /// \param vertexBuffer Vertex buffer whose contents to copy into this vertex buffer
    ///
    /// \return True if the copy was successful
    ///
    ////////////////////////////////////////////////////////////
    virtual bool update(const VertexBufferImpl& vertexBuffer) = 0;

    ////////////////////////////////////////////////////////////
    /// \brief Get the underlying OpenGL handle of the vertex buffer.
    ///
    /// \return OpenGL handle of the vertex buffer or 0 if not yet created
    ///
    ////////////////////////////////////////////////////////////
    virtual unsigned int getNativeHandle() const = 0;

    ////////////////////////////////////////////////////////////
    /// \brief Set the usage specifier of this vertex buffer
    ///
    /// \param usage Usage specifier
    ///
    ////////////////////////////////////////////////////////////
    virtual void setUsage(VertexBuffer::Usage usage) = 0;

    ////////////////////////////////////////////////////////////
    /// \brief Get the usage specifier of this vertex buffer
    ///
    /// \return Usage specifier
    ///
    ////////////////////////////////////////////////////////////
    virtual VertexBuffer::Usage getUsage() const = 0;
};

} // namespace priv

} // namespace sf


#endif // SFML_VERTEXBUFFER_IMPL_HPP
