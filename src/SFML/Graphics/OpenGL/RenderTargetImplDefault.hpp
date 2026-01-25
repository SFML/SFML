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
#include <SFML/Graphics/RenderTargetImpl.hpp>
#include <SFML/Graphics/BlendMode.hpp>
#include <SFML/Graphics/StencilMode.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/Graphics/View.hpp>

#include <array>
#include <cstdint>


namespace sf::priv
{
////////////////////////////////////////////////////////////
/// \brief OpenGL implementation of RenderTarget
///
////////////////////////////////////////////////////////////
class RenderTargetImplDefault : public RenderTargetImpl
{
public:
    ////////////////////////////////////////////////////////////
    /// \brief Constructor
    ///
    /// \param owner The RenderTarget that owns this implementation
    ///
    ////////////////////////////////////////////////////////////
    explicit RenderTargetImplDefault(RenderTarget& owner);

    ////////////////////////////////////////////////////////////
    /// \brief Destructor
    ///
    ////////////////////////////////////////////////////////////
    ~RenderTargetImplDefault() override = default;

    ////////////////////////////////////////////////////////////
    /// \brief Performs the common initialization step after creation
    ///
    ////////////////////////////////////////////////////////////
    void initialize() override;

    ////////////////////////////////////////////////////////////
    /// \brief Clear the entire target with a single color
    ///
    ////////////////////////////////////////////////////////////
    void clear(Color color) override;

    ////////////////////////////////////////////////////////////
    /// \brief Clear the stencil buffer to a specific value
    ///
    ////////////////////////////////////////////////////////////
    void clearStencil(StencilValue stencilValue) override;

    ////////////////////////////////////////////////////////////
    /// \brief Clear the entire target with a single color and stencil value
    ///
    ////////////////////////////////////////////////////////////
    void clear(Color color, StencilValue stencilValue) override;

    ////////////////////////////////////////////////////////////
    /// \brief Activate or deactivate the render target for rendering
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] bool setActive(bool active) override;

    ////////////////////////////////////////////////////////////
    /// \brief Save the current OpenGL render states and matrices
    ///
    ////////////////////////////////////////////////////////////
    void pushGLStates() override;

    ////////////////////////////////////////////////////////////
    /// \brief Restore the previously saved OpenGL render states and matrices
    ///
    ////////////////////////////////////////////////////////////
    void popGLStates() override;

    ////////////////////////////////////////////////////////////
    /// \brief Reset the internal OpenGL states so that the target is ready for drawing
    ///
    ////////////////////////////////////////////////////////////
    void resetGLStates() override;

    ////////////////////////////////////////////////////////////
    /// \brief Mark the view as changed without applying it
    ///
    ////////////////////////////////////////////////////////////
    void invalidateView() override;

    ////////////////////////////////////////////////////////////
    /// \brief Apply the current view
    ///
    ////////////////////////////////////////////////////////////
    void applyCurrentView() override;

    ////////////////////////////////////////////////////////////
    /// \brief Apply a new blending mode
    ///
    ////////////////////////////////////////////////////////////
    void applyBlendMode(const BlendMode& mode) override;

    ////////////////////////////////////////////////////////////
    /// \brief Apply a new stencil mode
    ///
    ////////////////////////////////////////////////////////////
    void applyStencilMode(const StencilMode& mode) override;

    ////////////////////////////////////////////////////////////
    /// \brief Apply a new transform
    ///
    ////////////////////////////////////////////////////////////
    void applyTransform(const Transform& transform) override;

    ////////////////////////////////////////////////////////////
    /// \brief Apply a new texture
    ///
    ////////////////////////////////////////////////////////////
    void applyTexture(const Texture* texture, CoordinateType coordinateType) override;

    ////////////////////////////////////////////////////////////
    /// \brief Apply a new shader
    ///
    ////////////////////////////////////////////////////////////
    void applyShader(const Shader* shader) override;

    ////////////////////////////////////////////////////////////
    /// \brief Setup environment for drawing
    ///
    ////////////////////////////////////////////////////////////
    void setupDraw(bool useVertexCache, const RenderStates& states) override;

    ////////////////////////////////////////////////////////////
    /// \brief Draw primitives defined by an array of vertices
    ///
    ////////////////////////////////////////////////////////////
    void drawVertices(const Vertex* vertices,
                      std::size_t vertexCount,
                      PrimitiveType type,
                      const RenderStates& states) override;

    ////////////////////////////////////////////////////////////
    /// \brief Draw the primitives
    ///
    ////////////////////////////////////////////////////////////
    void drawPrimitives(PrimitiveType type, std::size_t firstVertex, std::size_t vertexCount) override;

    ////////////////////////////////////////////////////////////
    /// \brief Clean up environment after drawing
    ///
    ////////////////////////////////////////////////////////////
    void cleanupDraw(const RenderStates& states) override;

private:
    ////////////////////////////////////////////////////////////
    /// \brief Render states cache
    ///
    ////////////////////////////////////////////////////////////
    struct StatesCache
    {
        bool                  enable{};                //!< Is the cache enabled?
        bool                  glStatesSet{};           //!< Are our internal GL states set yet?
        bool                  viewChanged{};           //!< Has the current view changed since last draw?
        bool                  scissorEnabled{};        //!< Is scissor testing enabled?
        bool                  stencilEnabled{};        //!< Is stencil testing enabled?
        BlendMode             lastBlendMode;           //!< Cached blending mode
        StencilMode           lastStencilMode;         //!< Cached stencil
        std::uint64_t         lastTextureId{};         //!< Cached texture
        CoordinateType        lastCoordinateType{};    //!< Texture coordinate type
        bool                  texCoordsArrayEnabled{}; //!< Is `GL_TEXTURE_COORD_ARRAY` client state enabled?
        bool                  useVertexCache{};        //!< Did we previously use the vertex cache?
        std::array<Vertex, 4> vertexCache{};           //!< Pre-transformed vertices cache
    };

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    RenderTarget& m_owner; //!< The RenderTarget that owns this implementation
    StatesCache   m_cache; //!< Render states cache
    std::uint64_t m_id;    //!< Unique number that identifies the RenderTarget
};

} // namespace sf::priv
