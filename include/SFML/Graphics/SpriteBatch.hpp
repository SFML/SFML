////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2022 Laurent Gomila (laurent@sfml-dev.org)
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
#include <SFML/Graphics/Export.hpp>

#include <SFML/Graphics/Batchable.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Vertex.hpp>

#include <vector>

namespace sf
{
class RenderTarget;

////////////////////////////////////////////////////////////
/// \brief Batching utility for drawables, meant for increasing
///        rendering performance
///
////////////////////////////////////////////////////////////
class SFML_GRAPHICS_API SpriteBatch : public Drawable
{
public:
    enum class BatchMode
    {
        ////////////////////////////////////////////////////////////
        /// \brief Creates a batch for each contiguous groups of drawables
        ///        that use the same texture.
        ///
        ////////////////////////////////////////////////////////////
        Deferred,

        ////////////////////////////////////////////////////////////
        /// \brief Organizes objects into a single layer, then reorders
        ///        them by texture before drawing.
        ///
        ////////////////////////////////////////////////////////////
        TextureSort,

        ////////////////////////////////////////////////////////////
        /// \brief Organizes objects into multiple layers. Groups objects
        ///        into layers based on depth, then reorders objects in
        ///        each layer by texture before drawing the layers from
        ///        furthest to closest.
        ///
        ////////////////////////////////////////////////////////////
        DepthSort,
    };

    ////////////////////////////////////////////////////////////
    /// \brief Sets the batch mode to use
    ///
    /// \param batchMode The new batch mode to use
    ///
    ////////////////////////////////////////////////////////////
    void setBatchMode(BatchMode batchMode);

    ////////////////////////////////////////////////////////////
    /// \brief Batches the given drawable
    ///
    /// \param drawable The drawable to be batched
    /// \param depth    The depth at which the drawable will be
    ///                 rendererd. This value is used only when
    ///                 BatchMode::DepthSort mode is used
    ///
    ////////////////////////////////////////////////////////////
    void batch(const Batchable& drawable, float depth = 0.f);

    ////////////////////////////////////////////////////////////
    /// \brief Batches an array of vertices
    ///
    /// \param vertices  The array of vertices to be batched
    /// \param count     How many vertices to batch
    /// \param type      The primitive formed by the vertices.
    ///                  Only triangle primitives are supported
    /// \param texture   The texture to use for rendering
    /// \param transform The transform to apply on the vertices
    /// \param depth     The depth at which the drawable will be
    ///                  rendererd. This value is used only when
    ///                  BatchMode::DepthSort mode is used.
    ///
    ////////////////////////////////////////////////////////////
    void batch(const Vertex*    vertices,
               std::size_t      count,
               PrimitiveType    type,
               const Texture*   texture,
               const Transform& transform = Transform::Identity,
               float            depth     = 0.f);

    ////////////////////////////////////////////////////////////
    /// \brief Renders the batch to the render target
    ///
    /// \param target The RenderTarget to draw to
    /// \param states The RenderStates to use. The texture is ignored
    ///
    ////////////////////////////////////////////////////////////
    void draw(RenderTarget& target, const RenderStates& states) const override;

    ////////////////////////////////////////////////////////////
    /// \brief Clears the batch, removing all drawables that were
    ///        added
    ///
    ////////////////////////////////////////////////////////////
    void clear();

private:
    ////////////////////////////////////////////////////////////
    /// \brief Pushes a triangle into internal storage
    ///
    /// \param a         First vertex
    /// \param b         Second vertex
    /// \param c         Third vertex
    /// \param transform Transform to apply before inserting
    /// \param texture   Texture to apply to triangle
    /// \param depth     The depth to use for depth sorting
    ///
    ////////////////////////////////////////////////////////////
    void pushTriangle(const Vertex&    a,
                      const Vertex&    b,
                      const Vertex&    c,
                      const Transform& transform,
                      const Texture*   texture,
                      float            depth);

    ////////////////////////////////////////////////////////////
    /// \brief Updates the batch, preparing its internal state for
    ///        efficient usage
    ///
    ////////////////////////////////////////////////////////////
    void updateBatch() const;

    ////////////////////////////////////////////////////////////
    /// \brief Holds information about a batched triangle
    ///
    ////////////////////////////////////////////////////////////
    struct TriangleInfo
    {
        const Texture* texture{}; //!< The triangle's texture
        float          depth{};   //!< The depth of the triangle

        TriangleInfo() = default;
        TriangleInfo(const Texture* theTexture, float theDepth) : texture(theTexture), depth(theDepth)
        {
        }
    };

    ////////////////////////////////////////////////////////////
    /// \brief Holds information for rendering a batch
    ///
    ////////////////////////////////////////////////////////////
    struct BatchInfo
    {
        const Texture* texture{};     //!< The texture used to render the batch
        std::size_t    vertexCount{}; //!< The number of contiguous vertices to render

        BatchInfo() = default;
        BatchInfo(const Texture* theTexture, std::size_t theVertexCount) :
        texture(theTexture),
        vertexCount(theVertexCount)
        {
        }
    };

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    // Batched Triangles
    std::vector<TriangleInfo>      m_triangles;        //!< Info about batched triangles
    std::vector<Vertex>            m_unsortedVertices; //!< Vertices currently batched
    mutable std::vector<BatchInfo> m_batches;          //!< Prepared batch information, ready for rendering
    mutable std::vector<Vertex>    m_vertices;         //!< Prepared vertices, ready for rendering

    // Batch Settings
    BatchMode    m_batchMode{BatchMode::Deferred}; //!< The current batch strategy
    mutable bool m_updateRequired{true};           //!< If true, batch must be sorted before rendering
};

} // namespace sf


////////////////////////////////////////////////////////////
/// \class sf::SpriteBatch
/// \ingroup graphics
///
/// sf::SpriteBatch is a batching utility for drawables. It
/// increases rendering performance by reordering and combining
/// drawables, such that they are rendered using fewer draw calls
/// and state switches.
///
/// sf::SpriteBatch supports multiple batching strategies. Each
/// comes with its benefits and drawbacks, so the optimal choice
/// depends on your specific needs.
///
/// \c sf::BatchMode::Deferred is best used when rendering contiguous
/// groups ofdrawables that use the same texture. This mode creates
/// batches for each such group, improving performance when many
/// drawables with few vertices are drawn (such as hundreds to
/// thousands of sf::Sprite drawables).
/// The render order of drawables is the same as the order they
/// were batched in. This is similar how rendering works with
/// sf::RenderTarget.
///
/// \c sf::BatchMode::TextureSort is best used when drawing objects
/// that are positioned on a "layer", and do not overlap each other.
/// This mode assumes that the draw order is not important within
/// a layer, and sorts all objects by texture before batching them.
/// This provides great performance improvements, especially when
/// rendering objects using different textures (since it will reorder
/// them back in contiguous groups).
/// Care should be taken when when drawing overlapping objects, since
/// it is not guaranteed that they will be ordered correctly.
///
/// \c sf::BatchMode::DepthSort is best used when drawing objects
/// that are positioned on multiple "layers" based on depth, and objects
/// within a layer do not overlap each other.
/// Within each layer, objects are sorted by texture before drawing,
/// similar to \c TextureSort. However, objects in a layer with a
/// higher "depth" value will be drawn behind objects with a lower
/// "depth" value.
/// You can also use this mode to render objects in an order that is
/// different from the order they were batched in, since objects
/// will be reordered by depth before rendering.
///
/// An example of how to use sf::SpriteBatch can be found in the
/// examples/sprite_batch project.
///
////////////////////////////////////////////////////////////
