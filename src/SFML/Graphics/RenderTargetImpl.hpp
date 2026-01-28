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
#include <SFML/Graphics/RenderTarget.hpp>

#include <cstddef>
#include <cstdint>


namespace sf
{
class Color;
class Shader;
class Texture;
class Transform;
struct BlendMode;
struct StencilMode;

namespace priv
{
////////////////////////////////////////////////////////////
/// \brief Abstract base class for render target implementations
///
////////////////////////////////////////////////////////////
class RenderTargetImpl
{
public:
    ////////////////////////////////////////////////////////////
    /// \brief Destructor
    ///
    ////////////////////////////////////////////////////////////
    virtual ~RenderTargetImpl() = default;

    ////////////////////////////////////////////////////////////
    /// \brief Performs the common initialization step after creation
    ///
    ////////////////////////////////////////////////////////////
    virtual void initialize() = 0;

    ////////////////////////////////////////////////////////////
    /// \brief Clear the entire target with a single color
    ///
    /// \param color Fill color to use to clear the render target
    ///
    ////////////////////////////////////////////////////////////
    virtual void clear(Color color) = 0;

    ////////////////////////////////////////////////////////////
    /// \brief Clear the stencil buffer to a specific value
    ///
    /// \param stencilValue Stencil value to clear to
    ///
    ////////////////////////////////////////////////////////////
    virtual void clearStencil(StencilValue stencilValue) = 0;

    ////////////////////////////////////////////////////////////
    /// \brief Clear the entire target with a single color and stencil value
    ///
    /// \param color        Fill color to use to clear the render target
    /// \param stencilValue Stencil value to clear to
    ///
    ////////////////////////////////////////////////////////////
    virtual void clear(Color color, StencilValue stencilValue) = 0;

    ////////////////////////////////////////////////////////////
    /// \brief Activate or deactivate the render target for rendering
    ///
    /// \param active `true` to activate, `false` to deactivate
    ///
    /// \return `true` if operation was successful, `false` otherwise
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] virtual bool setActive(bool active) = 0;

    ////////////////////////////////////////////////////////////
    /// \brief Save the current OpenGL render states and matrices
    ///
    ////////////////////////////////////////////////////////////
    virtual void pushGLStates() = 0;

    ////////////////////////////////////////////////////////////
    /// \brief Restore the previously saved OpenGL render states and matrices
    ///
    ////////////////////////////////////////////////////////////
    virtual void popGLStates() = 0;

    ////////////////////////////////////////////////////////////
    /// \brief Reset the internal OpenGL states so that the target is ready for drawing
    ///
    ////////////////////////////////////////////////////////////
    virtual void resetGLStates() = 0;

    ////////////////////////////////////////////////////////////
    /// \brief Mark the view as changed without applying it
    ///
    ////////////////////////////////////////////////////////////
    virtual void invalidateView() = 0;

    ////////////////////////////////////////////////////////////
    /// \brief Apply the current view
    ///
    ////////////////////////////////////////////////////////////
    virtual void applyCurrentView() = 0;

    ////////////////////////////////////////////////////////////
    /// \brief Apply a new blending mode
    ///
    /// \param mode Blending mode to apply
    ///
    ////////////////////////////////////////////////////////////
    virtual void applyBlendMode(const BlendMode& mode) = 0;

    ////////////////////////////////////////////////////////////
    /// \brief Apply a new stencil mode
    ///
    /// \param mode Stencil mode to apply
    ///
    ////////////////////////////////////////////////////////////
    virtual void applyStencilMode(const StencilMode& mode) = 0;

    ////////////////////////////////////////////////////////////
    /// \brief Apply a new transform
    ///
    /// \param transform Transform to apply
    ///
    ////////////////////////////////////////////////////////////
    virtual void applyTransform(const Transform& transform) = 0;

    ////////////////////////////////////////////////////////////
    /// \brief Apply a new texture
    ///
    /// \param texture        Texture to apply
    /// \param coordinateType The texture coordinate type to use
    ///
    ////////////////////////////////////////////////////////////
    virtual void applyTexture(const Texture* texture, CoordinateType coordinateType) = 0;

    ////////////////////////////////////////////////////////////
    /// \brief Apply a new shader
    ///
    /// \param shader Shader to apply
    ///
    ////////////////////////////////////////////////////////////
    virtual void applyShader(const Shader* shader) = 0;

    ////////////////////////////////////////////////////////////
    /// \brief Setup environment for drawing
    ///
    /// \param useVertexCache Are we going to use the vertex cache?
    /// \param states         Render states to use for drawing
    ///
    ////////////////////////////////////////////////////////////
    virtual void setupDraw(bool useVertexCache, const RenderStates& states) = 0;

    ////////////////////////////////////////////////////////////
    /// \brief Draw primitives defined by an array of vertices
    ///
    /// \param vertices    Pointer to the vertices
    /// \param vertexCount Number of vertices in the array
    /// \param type        Type of primitives to draw
    /// \param states      Render states to use for drawing
    ///
    ////////////////////////////////////////////////////////////
    virtual void drawVertices(const Vertex* vertices,
                              std::size_t vertexCount,
                              PrimitiveType type,
                              const RenderStates& states) = 0;

    ////////////////////////////////////////////////////////////
    /// \brief Draw the primitives
    ///
    /// \param type        Type of primitives to draw
    /// \param firstVertex Index of the first vertex to use when drawing
    /// \param vertexCount Number of vertices to use when drawing
    ///
    ////////////////////////////////////////////////////////////
    virtual void drawPrimitives(PrimitiveType type, std::size_t firstVertex, std::size_t vertexCount) = 0;

    virtual void setupVertexBufferDraw(const RenderStates& states) = 0;

    ////////////////////////////////////////////////////////////
    /// \brief Clean up environment after drawing
    ///
    /// \param states Render states used for drawing
    ///
    ////////////////////////////////////////////////////////////
    virtual void cleanupDraw(const RenderStates& states) = 0;
    ////////////////////////////////////////////////////////////
    /// rief Update the owner reference after a move
    ///
    /// \param newOwner The new owner of this implementation
    ///
    ////////////////////////////////////////////////////////////
    virtual void setOwner(RenderTarget& newOwner) = 0;};

} // namespace priv
} // namespace sf
