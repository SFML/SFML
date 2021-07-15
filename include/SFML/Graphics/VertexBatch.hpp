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

#ifndef SFML_VERTEXBATCH_HPP
#define SFML_VERTEXBATCH_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/VertexArray.hpp>


namespace sf
{
////////////////////////////////////////////////////////////
/// \brief RenderTarget wrapper that batches drawn shapes
///
/// Multiple shapes' vertices are batched together and drawn once.
/// This is a similar concept to the VertexArray which batches
/// indivial vertices. However, this can operate seamlessly on
/// any Drawable (excluding VertexBuffer, which is drawn normally.
///
////////////////////////////////////////////////////////////
class SFML_GRAPHICS_API VertexBatch : public RenderTarget, public VertexArray
{
public:

    ////////////////////////////////////////////////////////////
    /// \brief Construct a vertex batch which wraps a render target.
    ///
    /// \param target RenderTarget to wrap drawing with.
    /// \param type   Type of primitives, assumed to be triangles
    ///
    ////////////////////////////////////////////////////////////
    explicit VertexBatch(RenderTarget& target);

    ////////////////////////////////////////////////////////////
    /// \brief Draw a drawable object to the render target
    ///
    /// \param drawable Object to draw
    /// \param states   Render states to use for drawing
    ///
    ////////////////////////////////////////////////////////////
    inline void draw(const Drawable& drawable, const RenderStates& states = RenderStates::Default) {
        RenderTarget::draw(drawable, states);
    }

    ////////////////////////////////////////////////////////////
    /// \brief Draw primitives defined by an array of vertices
    /// Primitives that aren't triangles will be converted before batching.
    ///
    /// \param vertices    Pointer to the vertices
    /// \param vertexCount Number of vertices in the array
    /// \param type        Type of primitives to draw
    /// \param states      Render states to transform vertices with
    ///
    ////////////////////////////////////////////////////////////
    virtual void draw(const Vertex* vertices, std::size_t vertexCount,
                      PrimitiveType type, const RenderStates& states = RenderStates::Default);

    ////////////////////////////////////////////////////////////
    /// \brief Return the size of the wrapped render target
    ///
    /// \return Size in pixels
    ///
    ////////////////////////////////////////////////////////////
    virtual Vector2u getSize() const;

    ////////////////////////////////////////////////////////////
    /// \brief Does absolutely nothing
    ///
    /// This function does absolutely nothing.
    /// In the future it might be used with a mutex in a
    /// multithreaded context, but that's not going to happen.
    ///
    /// \param active Ignored.
    ///
    /// \return True
    ///
    ////////////////////////////////////////////////////////////
    virtual bool setActive(bool active);

    using VertexArray::clear;

private:

    ////////////////////////////////////////////////////////////
    /// \brief Append a vertex, transformed
    ///
    ////////////////////////////////////////////////////////////
    void batch(Vertex vert, const RenderStates& states);

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    RenderTarget &m_target; //!< Render target being wrapped over
};

} // namespace sf


#endif // SFML_VERTEXBATCH_HPP


////////////////////////////////////////////////////////////
/// \class sf::VertexBatch
/// \ingroup graphics
///
/// sf::VertexBatch is a wrapper around a sf::VertexArray
/// which is built by drawing other sf::Drawables to it.
///
/// Performance gains are similar to sf::VertexArray, but
/// apply to entire sf::Drawables rather than vertices alone.
///
/// Remember to clear() it before drawing another frame!
///
/// Example:
/// \code
/// sf::RenderWindow window;
/// sf::VertexBatch batch(window);
///
/// // Add a circle's vertices
/// sf::CircleShape circle(10, 20);
/// batch.draw(circle);
/// sf::Transform transform;
/// // Add its vertices again, but translated
/// transform.translate(200, 200);
/// batch.draw(circle, transform);
///
/// window.draw(batch);
/// \endcode
///
/// \see sf::VertexArray
///
////////////////////////////////////////////////////////////
