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

#ifndef SFML_RENDERTARGET_IMPL_HPP
#define SFML_RENDERTARGET_IMPL_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/System/Vector2.hpp>
#include <cstddef>


namespace sf
{
class Drawable;
class VertexBuffer;
class Vertex;
class View;
class RenderStates;
class Color;
class RenderTarget;
class Texture;

namespace priv
{
class TextureImpl;

////////////////////////////////////////////////////////////
/// \brief Base class for all render targets (window, texture, ...)
///
////////////////////////////////////////////////////////////
class RenderTargetImpl
{
public:

    ////////////////////////////////////////////////////////////
    /// \brief Constructor
    ///
    ////////////////////////////////////////////////////////////
    RenderTargetImpl(RenderTarget* parent);

    ////////////////////////////////////////////////////////////
    /// \brief Destructor
    ///
    ////////////////////////////////////////////////////////////
    virtual ~RenderTargetImpl();

    ////////////////////////////////////////////////////////////
    /// \brief Clear the entire target with a single color
    ///
    /// \param color Fill color to use to clear the render target
    ///
    ////////////////////////////////////////////////////////////
    virtual void clear(const Color& color) = 0;

    ////////////////////////////////////////////////////////////
    /// \brief Change the current active view
    ///
    /// \param view New view to use
    ///
    /// \see getView, getDefaultView
    ///
    ////////////////////////////////////////////////////////////
    virtual void setView(const View& view) = 0;

    ////////////////////////////////////////////////////////////
    /// \brief Get the view currently in use in the render target
    ///
    /// \return The view object that is currently used
    ///
    /// \see setView, getDefaultView
    ///
    ////////////////////////////////////////////////////////////
    virtual const View& getView() const = 0;

    ////////////////////////////////////////////////////////////
    /// \brief Get the default view of the render target
    ///
    /// \return The default view of the render target
    ///
    /// \see setView, getView
    ///
    ////////////////////////////////////////////////////////////
    virtual const View& getDefaultView() const = 0;

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
                      PrimitiveType type, const RenderStates& states) = 0;

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
                      std::size_t vertexCount, const RenderStates& states) = 0;

    ////////////////////////////////////////////////////////////
    /// \brief Activate or deactivate the render target for rendering
    ///
    /// \param active True to activate, false to deactivate
    ///
    /// \return True if operation was successful, false otherwise
    ///
    ////////////////////////////////////////////////////////////
    virtual bool setActive(bool active) = 0;

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
    virtual void initialize(const Vector2u& newSize) = 0;

protected:

    ////////////////////////////////////////////////////////////
    /// \brief Get the parent RenderTarget
    ///
    /// \return The parent RenderTarget
    ///
    ////////////////////////////////////////////////////////////
    RenderTarget* getParent();

    ////////////////////////////////////////////////////////////
    /// \brief Get the concrete implementation of a texture
    ///
    /// \param The texture
    ///
    /// \return The concrete implementation of a texture
    ///
    ////////////////////////////////////////////////////////////
    static const TextureImpl* getTextureImpl(const Texture& texture);

private:

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    RenderTarget* m_parent;

};

} // namespace priv

} // namespace sf


#endif // SFML_RENDERTARGET_IMPL_HPP
