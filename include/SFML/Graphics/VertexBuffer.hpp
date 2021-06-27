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

#ifndef SFML_VERTEXBUFFER_HPP
#define SFML_VERTEXBUFFER_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics/Export.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Window/GlResource.hpp>


namespace sf
{
class RenderTarget;
class Vertex;

////////////////////////////////////////////////////////////
/// \brief Vertex buffer storage for one or more 2D primitives
///
////////////////////////////////////////////////////////////
class SFML_GRAPHICS_API VertexBuffer : public Drawable, private GlResource
{
public:

    ////////////////////////////////////////////////////////////
    /// \brief Usage specifiers
    ///
    /// If data is going to be updated once or more every frame,
    /// set the usage to Stream. If data is going to be set once
    /// and used for a long time without being modified, set the
    /// usage to Static. For everything else Dynamic should be a
    /// good compromise.
    ///
    ////////////////////////////////////////////////////////////
    enum Usage
    {
        Stream,  //!< Constantly changing data
        Dynamic, //!< Occasionally changing data
        Static   //!< Rarely changing data
    };

    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    /// Creates an empty vertex buffer.
    ///
    ////////////////////////////////////////////////////////////
    VertexBuffer();

    ////////////////////////////////////////////////////////////
    /// \brief Construct a VertexBuffer with a specific PrimitiveType
    ///
    /// Creates an empty vertex buffer and sets its primitive type to \p type.
    ///
    /// \param type Type of primitive
    ///
    ////////////////////////////////////////////////////////////
    explicit VertexBuffer(PrimitiveType type);

    ////////////////////////////////////////////////////////////
    /// \brief Construct a VertexBuffer with a specific usage specifier
    ///
    /// Creates an empty vertex buffer and sets its usage to \p usage.
    ///
    /// \param usage Usage specifier
    ///
    ////////////////////////////////////////////////////////////
    explicit VertexBuffer(Usage usage);

    ////////////////////////////////////////////////////////////
    /// \brief Construct a VertexBuffer with a specific PrimitiveType and usage specifier
    ///
    /// Creates an empty vertex buffer and sets its primitive type
    /// to \p type and usage to \p usage.
    ///
    /// \param type  Type of primitive
    /// \param usage Usage specifier
    ///
    ////////////////////////////////////////////////////////////
    VertexBuffer(PrimitiveType type, Usage usage);

    ////////////////////////////////////////////////////////////
    /// \brief Copy constructor
    ///
    /// \param copy instance to copy
    ///
    ////////////////////////////////////////////////////////////
    VertexBuffer(const VertexBuffer& copy);

    ////////////////////////////////////////////////////////////
    /// \brief Destructor
    ///
    ////////////////////////////////////////////////////////////
    ~VertexBuffer();

    ////////////////////////////////////////////////////////////
    /// \brief Create the vertex buffer
    ///
    /// Creates the vertex buffer and allocates enough graphics
    /// memory to hold \p vertexCount vertices. Any previously
    /// allocated memory is freed in the process.
    ///
    /// In order to deallocate previously allocated memory pass 0
    /// as \p vertexCount. Don't forget to recreate with a non-zero
    /// value when graphics memory should be allocated again.
    ///
    /// \param vertexCount Number of vertices worth of memory to allocate
    ///
    /// \return True if creation was successful
    ///
    ////////////////////////////////////////////////////////////
    bool create(std::size_t vertexCount);

    ////////////////////////////////////////////////////////////
    /// \brief Return the vertex count
    ///
    /// \return Number of vertices in the vertex buffer
    ///
    ////////////////////////////////////////////////////////////
    std::size_t getVertexCount() const;

    ////////////////////////////////////////////////////////////
    /// \brief Update the whole buffer from an array of vertices
    ///
    /// The \a vertex array is assumed to have the same size as
    /// the \a created buffer.
    ///
    /// No additional check is performed on the size of the vertex
    /// array, passing invalid arguments will lead to undefined
    /// behavior.
    ///
    /// This function does nothing if \a vertices is null or if the
    /// buffer was not previously created.
    ///
    /// \param vertices Array of vertices to copy to the buffer
    ///
    /// \return True if the update was successful
    ///
    ////////////////////////////////////////////////////////////
    bool update(const Vertex* vertices);

    ////////////////////////////////////////////////////////////
    /// \brief Update a part of the buffer from an array of vertices
    ///
    /// \p offset is specified as the number of vertices to skip
    /// from the beginning of the buffer.
    ///
    /// If \p offset is 0 and \p vertexCount is equal to the size of
    /// the currently created buffer, its whole contents are replaced.
    ///
    /// If \p offset is 0 and \p vertexCount is greater than the
    /// size of the currently created buffer, a new buffer is created
    /// containing the vertex data.
    ///
    /// If \p offset is 0 and \p vertexCount is less than the size of
    /// the currently created buffer, only the corresponding region
    /// is updated.
    ///
    /// If \p offset is not 0 and \p offset + \p vertexCount is greater
    /// than the size of the currently created buffer, the update fails.
    ///
    /// No additional check is performed on the size of the vertex
    /// array, passing invalid arguments will lead to undefined
    /// behavior.
    ///
    /// \param vertices    Array of vertices to copy to the buffer
    /// \param vertexCount Number of vertices to copy
    /// \param offset      Offset in the buffer to copy to
    ///
    /// \return True if the update was successful
    ///
    ////////////////////////////////////////////////////////////
    bool update(const Vertex* vertices, std::size_t vertexCount, unsigned int offset);

    ////////////////////////////////////////////////////////////
    /// \brief Copy the contents of another buffer into this buffer
    ///
    /// \param vertexBuffer Vertex buffer whose contents to copy into this vertex buffer
    ///
    /// \return True if the copy was successful
    ///
    ////////////////////////////////////////////////////////////
    bool update(const VertexBuffer& vertexBuffer);

    ////////////////////////////////////////////////////////////
    /// \brief Overload of assignment operator
    ///
    /// \param right Instance to assign
    ///
    /// \return Reference to self
    ///
    ////////////////////////////////////////////////////////////
    VertexBuffer& operator =(const VertexBuffer& right);

    ////////////////////////////////////////////////////////////
    /// \brief Swap the contents of this vertex buffer with those of another
    ///
    /// \param right Instance to swap with
    ///
    ////////////////////////////////////////////////////////////
    void swap(VertexBuffer& right);

    ////////////////////////////////////////////////////////////
    /// \brief Get the underlying OpenGL handle of the vertex buffer.
    ///
    /// You shouldn't need to use this function, unless you have
    /// very specific stuff to implement that SFML doesn't support,
    /// or implement a temporary workaround until a bug is fixed.
    ///
    /// \return OpenGL handle of the vertex buffer or 0 if not yet created
    ///
    ////////////////////////////////////////////////////////////
    unsigned int getNativeHandle() const;

    ////////////////////////////////////////////////////////////
    /// \brief Set the type of primitives to draw
    ///
    /// This function defines how the vertices must be interpreted
    /// when it's time to draw them.
    ///
    /// The default primitive type is sf::Points.
    ///
    /// \param type Type of primitive
    ///
    ////////////////////////////////////////////////////////////
    void setPrimitiveType(PrimitiveType type);

    ////////////////////////////////////////////////////////////
    /// \brief Get the type of primitives drawn by the vertex buffer
    ///
    /// \return Primitive type
    ///
    ////////////////////////////////////////////////////////////
    PrimitiveType getPrimitiveType() const;

    ////////////////////////////////////////////////////////////
    /// \brief Set the usage specifier of this vertex buffer
    ///
    /// This function provides a hint about how this vertex buffer is
    /// going to be used in terms of data update frequency.
    ///
    /// After changing the usage specifier, the vertex buffer has
    /// to be updated with new data for the usage specifier to
    /// take effect.
    ///
    /// The default primitive type is sf::VertexBuffer::Stream.
    ///
    /// \param usage Usage specifier
    ///
    ////////////////////////////////////////////////////////////
    void setUsage(Usage usage);

    ////////////////////////////////////////////////////////////
    /// \brief Get the usage specifier of this vertex buffer
    ///
    /// \return Usage specifier
    ///
    ////////////////////////////////////////////////////////////
    Usage getUsage() const;

    ////////////////////////////////////////////////////////////
    /// \brief Bind a vertex buffer for rendering
    ///
    /// This function is not part of the graphics API, it mustn't be
    /// used when drawing SFML entities. It must be used only if you
    /// mix sf::VertexBuffer with OpenGL code.
    ///
    /// \code
    /// sf::VertexBuffer vb1, vb2;
    /// ...
    /// sf::VertexBuffer::bind(&vb1);
    /// // draw OpenGL stuff that use vb1...
    /// sf::VertexBuffer::bind(&vb2);
    /// // draw OpenGL stuff that use vb2...
    /// sf::VertexBuffer::bind(NULL);
    /// // draw OpenGL stuff that use no vertex buffer...
    /// \endcode
    ///
    /// \param vertexBuffer Pointer to the vertex buffer to bind, can be null to use no vertex buffer
    ///
    ////////////////////////////////////////////////////////////
    static void bind(const VertexBuffer* vertexBuffer);

    ////////////////////////////////////////////////////////////
    /// \brief Tell whether or not the system supports vertex buffers
    ///
    /// This function should always be called before using
    /// the vertex buffer features. If it returns false, then
    /// any attempt to use sf::VertexBuffer will fail.
    ///
    /// \return True if vertex buffers are supported, false otherwise
    ///
    ////////////////////////////////////////////////////////////
    static bool isAvailable();

private:

    ////////////////////////////////////////////////////////////
    /// \brief Draw the vertex buffer to a render target
    ///
    /// \param target Render target to draw to
    /// \param states Current render states
    ///
    ////////////////////////////////////////////////////////////
    virtual void draw(RenderTarget& target, RenderStates states) const;

private:

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    unsigned int  m_buffer;        //!< Internal buffer identifier
    std::size_t   m_size;          //!< Size in Vertexes of the currently allocated buffer
    PrimitiveType m_primitiveType; //!< Type of primitives to draw
    Usage         m_usage;         //!< How this vertex buffer is to be used
};

} // namespace sf


#endif // SFML_VERTEXBUFFER_HPP


////////////////////////////////////////////////////////////
/// \class sf::VertexBuffer
/// \ingroup graphics
///
/// sf::VertexBuffer is a simple wrapper around a dynamic
/// buffer of vertices and a primitives type.
///
/// Unlike sf::VertexArray, the vertex data is stored in
/// graphics memory.
///
/// In situations where a large amount of vertex data would
/// have to be transferred from system memory to graphics memory
/// every frame, using sf::VertexBuffer can help. By using a
/// sf::VertexBuffer, data that has not been changed between frames
/// does not have to be re-transferred from system to graphics
/// memory as would be the case with sf::VertexArray. If data transfer
/// is a bottleneck, this can lead to performance gains.
///
/// Using sf::VertexBuffer, the user also has the ability to only modify
/// a portion of the buffer in graphics memory. This way, a large buffer
/// can be allocated at the start of the application and only the
/// applicable portions of it need to be updated during the course of
/// the application. This allows the user to take full control of data
/// transfers between system and graphics memory if they need to.
///
/// In special cases, the user can make use of multiple threads to update
/// vertex data in multiple distinct regions of the buffer simultaneously.
/// This might make sense when e.g. the position of multiple objects has to
/// be recalculated very frequently. The computation load can be spread
/// across multiple threads as long as there are no other data dependencies.
///
/// Simultaneous updates to the vertex buffer are not guaranteed to be
/// carried out by the driver in any specific order. Updating the same
/// region of the buffer from multiple threads will not cause undefined
/// behaviour, however the final state of the buffer will be unpredictable.
///
/// Simultaneous updates of distinct non-overlapping regions of the buffer
/// are also not guaranteed to complete in a specific order. However, in
/// this case the user can make sure to synchronize the writer threads at
/// well-defined points in their code. The driver will make sure that all
/// pending data transfers complete before the vertex buffer is sourced
/// by the rendering pipeline.
///
/// It inherits sf::Drawable, but unlike other drawables it
/// is not transformable.
///
/// Example:
/// \code
/// sf::Vertex vertices[15];
/// ...
/// sf::VertexBuffer triangles(sf::Triangles);
/// triangles.create(15);
/// triangles.update(vertices);
/// ...
/// window.draw(triangles);
/// \endcode
///
/// \see sf::Vertex, sf::VertexArray
///
////////////////////////////////////////////////////////////
