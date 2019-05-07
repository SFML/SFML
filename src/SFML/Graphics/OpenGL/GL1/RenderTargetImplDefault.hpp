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

#ifndef SFML_RENDERTARGET_IMPL_DEFAULT_HPP
#define SFML_RENDERTARGET_IMPL_DEFAULT_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics/RenderTargetImpl.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/Transform.hpp>
#include <SFML/Graphics/BlendMode.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/System/NonCopyable.hpp>


namespace sf
{
class Drawable;
class VertexBuffer;

namespace priv
{
////////////////////////////////////////////////////////////
/// \brief Base class for all render targets (window, texture, ...)
///
////////////////////////////////////////////////////////////
class RenderTargetImplDefault : public RenderTargetImpl
{
public:

    ////////////////////////////////////////////////////////////
    /// \brief Constructor
    ///
    ////////////////////////////////////////////////////////////
    RenderTargetImplDefault(RenderTarget* parent);

    ////////////////////////////////////////////////////////////
    /// \brief Destructor
    ///
    ////////////////////////////////////////////////////////////
    virtual ~RenderTargetImplDefault();

    ////////////////////////////////////////////////////////////
    /// \brief Clear the entire target with a single color
    ///
    /// \param color Fill color to use to clear the render target
    ///
    ////////////////////////////////////////////////////////////
    virtual void clear(const Color& color);

    ////////////////////////////////////////////////////////////
    /// \brief Change the current active view
    ///
    /// \param view New view to use
    ///
    /// \see getView, getDefaultView
    ///
    ////////////////////////////////////////////////////////////
    virtual void setView(const View& view);

    ////////////////////////////////////////////////////////////
    /// \brief Get the view currently in use in the render target
    ///
    /// \return The view object that is currently used
    ///
    /// \see setView, getDefaultView
    ///
    ////////////////////////////////////////////////////////////
    virtual const View& getView() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the default view of the render target
    ///
    /// \return The default view of the render target
    ///
    /// \see setView, getView
    ///
    ////////////////////////////////////////////////////////////
    virtual const View& getDefaultView() const;

    ////////////////////////////////////////////////////////////
    /// \brief Draw primitives defined by an array of vertices
    ///
    /// \param vertices    Pointer to the vertices
    /// \param vertexCount Number of vertices in the array
    /// \param type        Type of primitives to draw
    /// \param states      Render states to use for drawing
    ///
    ////////////////////////////////////////////////////////////
    virtual void draw(const Vertex* vertices, std::size_t vertexCount,
                      PrimitiveType type, const RenderStates& states);

    ////////////////////////////////////////////////////////////
    /// \brief Draw primitives defined by a vertex buffer
    ///
    /// \param vertexBuffer Vertex buffer
    /// \param firstVertex  Index of the first vertex to render
    /// \param vertexCount  Number of vertices to render
    /// \param states       Render states to use for drawing
    ///
    ////////////////////////////////////////////////////////////
    virtual void draw(const VertexBuffer& vertexBuffer, std::size_t firstVertex,
                      std::size_t vertexCount, const RenderStates& states);

    ////////////////////////////////////////////////////////////
    /// \brief Activate or deactivate the render target for rendering
    ///
    /// \param active True to activate, false to deactivate
    ///
    /// \return True if operation was successful, false otherwise
    ///
    ////////////////////////////////////////////////////////////
    virtual bool setActive(bool active);

    ////////////////////////////////////////////////////////////
    /// \brief Save the current OpenGL render states and matrices
    ///
    /// \see popGLStates
    ///
    ////////////////////////////////////////////////////////////
    virtual void pushGLStates();

    ////////////////////////////////////////////////////////////
    /// \brief Restore the previously saved OpenGL render states and matrices
    ///
    /// \see pushGLStates
    ///
    ////////////////////////////////////////////////////////////
    virtual void popGLStates();

    ////////////////////////////////////////////////////////////
    /// \brief Reset the internal OpenGL states so that the target is ready for drawing
    ///
    ////////////////////////////////////////////////////////////
    virtual void resetGLStates();

    ////////////////////////////////////////////////////////////
    /// \brief Performs the common initialization step after creation
    ///
    /// \param newSize New size of the RenderTarget
    ///
    ////////////////////////////////////////////////////////////
    virtual void initialize(const Vector2u& newSize);

private:

    ////////////////////////////////////////////////////////////
    /// \brief Apply the current view
    ///
    ////////////////////////////////////////////////////////////
    void applyCurrentView();

    ////////////////////////////////////////////////////////////
    /// \brief Apply a new blending mode
    ///
    /// \param mode Blending mode to apply
    ///
    ////////////////////////////////////////////////////////////
    void applyBlendMode(const BlendMode& mode);

    ////////////////////////////////////////////////////////////
    /// \brief Apply a new transform
    ///
    /// \param transform Transform to apply
    ///
    ////////////////////////////////////////////////////////////
    void applyTransform(const Transform& transform);

    ////////////////////////////////////////////////////////////
    /// \brief Apply a new texture
    ///
    /// \param texture Texture to apply
    ///
    ////////////////////////////////////////////////////////////
    void applyTexture(const Texture* texture);

    ////////////////////////////////////////////////////////////
    /// \brief Apply a new shader
    ///
    /// \param shader Shader to apply
    ///
    ////////////////////////////////////////////////////////////
    void applyShader(const Shader* shader);

    ////////////////////////////////////////////////////////////
    /// \brief Setup environment for drawing
    ///
    /// \param useVertexCache Are we going to use the vertex cache?
    /// \param states         Render states to use for drawing
    ///
    ////////////////////////////////////////////////////////////
    void setupDraw(bool useVertexCache, const RenderStates& states);

    ////////////////////////////////////////////////////////////
    /// \brief Draw the primitives
    ///
    /// \param type        Type of primitives to draw
    /// \param firstVertex Index of the first vertex to use when drawing
    /// \param vertexCount Number of vertices to use when drawing
    ///
    ////////////////////////////////////////////////////////////
    void drawPrimitives(PrimitiveType type, std::size_t firstVertex, std::size_t vertexCount);

    ////////////////////////////////////////////////////////////
    /// \brief Clean up environment after drawing
    ///
    /// \param states Render states used for drawing
    ///
    ////////////////////////////////////////////////////////////
    void cleanupDraw(const RenderStates& states);

    ////////////////////////////////////////////////////////////
    /// \brief Render states cache
    ///
    ////////////////////////////////////////////////////////////
    struct StatesCache
    {
        enum {VertexCacheSize = 4};

        bool      enable;         ///< Is the cache enabled?
        bool      glStatesSet;    ///< Are our internal GL states set yet?
        bool      viewChanged;    ///< Has the current view changed since last draw?
        BlendMode lastBlendMode;  ///< Cached blending mode
        Uint64    lastTextureId;  ///< Cached texture
        bool      texCoordsArrayEnabled; ///< Is GL_TEXTURE_COORD_ARRAY client state enabled?
        bool      useVertexCache; ///< Did we previously use the vertex cache?
        Vertex    vertexCache[VertexCacheSize]; ///< Pre-transformed vertices cache
    };

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    View        m_defaultView; ///< Default view
    View        m_view;        ///< Current view
    StatesCache m_cache;       ///< Render states cache
    Uint64      m_id;          ///< Unique number that identifies the RenderTarget
};

} // namespace priv

} // namespace sf


#endif // SFML_RENDERTARGET_IMPL_DEFAULT_HPP
