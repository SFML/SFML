////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2009 Laurent Gomila (laurent.gom@gmail.com)
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

#ifndef SFML_RENDERER_HPP
#define SFML_RENDERER_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Config.hpp>
#include <SFML/System/NonCopyable.hpp>
#include <SFML/Window/GlResource.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Matrix3.hpp>


namespace sf
{
class Image;
class Shader;

////////////////////////////////////////////////////////////
/// \brief Handles the low-level rendering (states and geometry)
///
////////////////////////////////////////////////////////////
class SFML_API Renderer : GlResource, NonCopyable
{
public :

    ////////////////////////////////////////////////////////////
    /// \brief Types of primitives to be rendererd
    ///
    ////////////////////////////////////////////////////////////
    enum PrimitiveType
    {
        TriangleList,  ///< Simple list of triangles
        TriangleStrip, ///< Triangle strip (consecutive triangles always share two points)
        TriangleFan,   ///< Triangle fan (one center point + outline points)
        QuadList       ///< Simple list of quads
    };

public :

    ////////////////////////////////////////////////////////////
    /// \brief Construct the renderer with its owner render target
    ///
    /// \param target Owner render target
    ///
    ////////////////////////////////////////////////////////////
    Renderer(RenderTarget& target);

    ////////////////////////////////////////////////////////////
    /// \brief Initialize the renderer (set the default states, etc.)
    ///
    ////////////////////////////////////////////////////////////
    void Initialize();

    ////////////////////////////////////////////////////////////
    /// \brief Save the current OpenGL render states and matrices
    ///
    /// \see RestoreGLStates
    ///
    ////////////////////////////////////////////////////////////
    void SaveGLStates();

    ////////////////////////////////////////////////////////////
    /// \brief Restore the previously saved OpenGL render states and matrices
    ///
    /// \see SaveGLStates
    ///
    ////////////////////////////////////////////////////////////
    void RestoreGLStates();

    ////////////////////////////////////////////////////////////
    /// Clear the color buffer
    ///
    /// \param color Color to use to clear the color buffer
    ///
    ////////////////////////////////////////////////////////////
    void Clear(const Color& color);

    ////////////////////////////////////////////////////////////
    /// \brief Save the current render states
    ///
    /// \see PopStates
    ///
    ////////////////////////////////////////////////////////////
    void PushStates();

    ////////////////////////////////////////////////////////////
    /// \brief Restore the previously saved render states
    ///
    /// \see PushStates
    ///
    ////////////////////////////////////////////////////////////
    void PopStates();

    ////////////////////////////////////////////////////////////
    /// \brief Set a new model-view matrix
    ///
    /// Note: any call to this function after a call to BeginBatch
    /// will be ignored, and delayed until BeginBatch is called again.
    ///
    /// \param matrix New model-view matrix
    ///
    /// \see ApplyModelView
    ///
    ////////////////////////////////////////////////////////////
    void SetModelView(const Matrix3& matrix);

    ////////////////////////////////////////////////////////////
    /// \brief Combine a new model-view matrix with the current one
    ///
    /// Note: any call to this function after a call to BeginBatch
    /// will be ignored, and delayed until BeginBatch is called again.
    ///
    /// \param matrix Model-view matrix to combine
    ///
    /// \see SetModelView
    ///
    ////////////////////////////////////////////////////////////
    void ApplyModelView(const Matrix3& matrix);

    ////////////////////////////////////////////////////////////
    /// \brief Set a new projection matrix
    ///
    /// Note: any call to this function after a call to BeginBatch
    /// will be ignored, and delayed until BeginBatch is called again.
    ///
    /// \param matrix New projection matrix
    ///
    /// \see ApplyProjection
    ///
    ////////////////////////////////////////////////////////////
    void SetProjection(const Matrix3& matrix);

    ////////////////////////////////////////////////////////////
    /// \brief Set the current global color
    ///
    /// This color will be modulated with each vertex's color.
    /// Note: any call to this function after a call to BeginBatch
    /// will be ignored, and delayed until BeginBatch is called again.
    ///
    /// \param color New global color
    ///
    /// \see ApplyColor
    ///
    ////////////////////////////////////////////////////////////
    void SetColor(const Color& color);

    ////////////////////////////////////////////////////////////
    /// \brief Modulate the current global color with a new one
    ///
    /// This color will be modulated with each vertex's color.
    /// Note: any call to this function after a call to BeginBatch
    /// will be ignored, and delayed until BeginBatch is called again.
    ///
    /// \param color Color to modulate
    ///
    /// \see SetColor
    ///
    ////////////////////////////////////////////////////////////
    void ApplyColor(const Color& color);

    ////////////////////////////////////////////////////////////
    /// \brief Set the current viewport
    ///
    /// Note: any call to this function after a call to BeginBatch
    /// will be ignored, and delayed until BeginBatch is called again.
    ///
    /// \param viewport New viewport to apply
    ///
    ////////////////////////////////////////////////////////////
    void SetViewport(const IntRect& viewport);

    ////////////////////////////////////////////////////////////
    /// \brief Set the current alpha-blending mode
    ///
    /// Note: any call to this function after a call to BeginBatch
    /// will be ignored, and delayed until BeginBatch is called again.
    ///
    /// \param mode New blending mode
    ///
    ////////////////////////////////////////////////////////////
    void SetBlendMode(Blend::Mode mode);

    ////////////////////////////////////////////////////////////
    /// \brief Set the current texture
    ///
    /// Note: any call to this function after a call to BeginBatch
    /// will be ignored, and delayed until BeginBatch is called again.
    ///
    /// \param texture New texture
    ///
    ////////////////////////////////////////////////////////////
    void SetTexture(const Image* texture);

    ////////////////////////////////////////////////////////////
    /// \brief Set the current shader
    ///
    /// Note: any call to this function after a call to BeginBatch
    /// will be ignored, and delayed until BeginBatch is called again.
    ///
    /// \param shader New Shader
    ///
    ////////////////////////////////////////////////////////////
    void SetShader(const Shader* shader);

    ////////////////////////////////////////////////////////////
    /// \brief Begin rendering a new geometry batch
    ///
    /// You need to call End() to complete the batch and trigger
    /// the actual rendering of the geometry that you passed
    /// between Begin() and End().
    ///
    /// Usage:
    /// \code
    /// renderer.Begin(Renderer::TriangleList);
    /// renderer.AddVertex(...);
    /// renderer.AddVertex(...);
    /// renderer.AddVertex(...);
    /// renderer.End();
    /// \endcode
    ///
    /// \param type Type of the primitives that are going to be rendered
    ///
    /// \see End
    ///
    ////////////////////////////////////////////////////////////
    void Begin(PrimitiveType type);

    ////////////////////////////////////////////////////////////
    /// \brief End the current geometry batch and render it
    ///
    /// \see Begin
    ///
    ////////////////////////////////////////////////////////////
    void End();

    ////////////////////////////////////////////////////////////
    /// \brief Add a new vertex (position only)
    ///
    /// This function adds a new vertex to the current batch.
    /// This is equivalent to calling AddVertex(x, y, 0, 0, Color::White).
    ///
    /// \param x X coordinate of the vertex
    /// \param y Y coordinate of the vertex
    ///
    ////////////////////////////////////////////////////////////
    void AddVertex(float x, float y);

    ////////////////////////////////////////////////////////////
    /// \brief Add a new vertex (position + texture coordinates)
    ///
    /// This function adds a new vertex to the current batch.
    /// This is equivalent to calling AddVertex(x, y, u, v, Color::White).
    ///
    /// \param x X coordinate of the vertex
    /// \param y Y coordinate of the vertex
    /// \param u X texture coordinate of the vertex
    /// \param v Y texture coordinate of the vertex
    ///
    ////////////////////////////////////////////////////////////
    void AddVertex(float x, float y, float u, float v);

    ////////////////////////////////////////////////////////////
    /// \brief Add a new vertex (position + color)
    ///
    /// This function adds a new vertex to the current batch.
    /// This is equivalent to calling AddVertex(x, y, 0, 0, color).
    ///
    /// \param x X coordinate of the vertex
    /// \param y Y coordinate of the vertex
    /// \param color Color of the vertex
    ///
    ////////////////////////////////////////////////////////////
    void AddVertex(float x, float y, const Color& color);

    ////////////////////////////////////////////////////////////
    /// \brief Add a new vertex (position + texture coordinates + color)
    ///
    /// This function adds a new vertex to the current batch.
    ///
    /// \param x X coordinate of the vertex
    /// \param y Y coordinate of the vertex
    /// \param u X texture coordinate of the vertex
    /// \param v Y texture coordinate of the vertex
    /// \param color Color of the vertex
    ///
    ////////////////////////////////////////////////////////////
    void AddVertex(float x, float y, float u, float v, const Color& color);

private :

    ////////////////////////////////////////////////////////////
    /// \brief Process a new vertex
    ///
    /// This function is called by all the public overloads of AddVertex,
    /// it processes a new vertex to be rendered.
    ///
    /// \param x X coordinate of the vertex
    /// \param y Y coordinate of the vertex
    /// \param u X texture coordinate of the vertex
    /// \param v Y texture coordinate of the vertex
    /// \param r Red component of the vertex color (normalized)
    /// \param g Green component of the vertex color (normalized)
    /// \param b Blue component of the vertex color (normalized)
    /// \param a Alpha component of the vertex color (normalized)
    ///
    ////////////////////////////////////////////////////////////
    void ProcessVertex(float x, float y, float u, float v, float r, float g, float b, float a);

    ////////////////////////////////////////////////////////////
    // Structure holding the render states that can be stacked
    ////////////////////////////////////////////////////////////
    struct States
    {
        States() : r(1.f), g(1.f), b(1.f), a(1.f) {}

        Matrix3 modelView;  ///< Model-view matrix
        float   r, g, b, a; ///< Vertex color (normalized components for faster operations)
    };

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    RenderTarget& myTarget;           ///< Reference to the render target owning this renderer
    States        myStatesStack[64];  ///< Stack of render states
    States*       myStates;           ///< Current set of render states
    const Image*  myTexture;          ///< Current texture
    unsigned int  myTextureId;        ///< Current texture identifier (the sf::Image instance may be the same, but not the internal OpenGL texture)
    const Shader* myShader;           ///< Current pixel shader
    Blend::Mode   myBlendMode;        ///< Current blending mode
    IntRect       myViewport;         ///< Current target viewport
    bool          myTextureIsValid;   ///< Is the cached texture valid? (if not, the cached value is ignored)
    bool          myShaderIsValid;    ///< Is the cached shader valid? (if not, the cached value is ignored)
    bool          myBlendModeIsValid; ///< Is the cached blend mode valid? (if not, the cached value is ignored)
    bool          myViewportIsValid;  ///< Is the cached viewport valid? (if not, the cached value is ignored)
};

} // namespace sf


#endif // SFML_RENDERER_HPP


////////////////////////////////////////////////////////////
/// \class sf::Renderer
/// \ingroup graphics
///
/// sf::Renderer is the abstraction layer between SFML code
/// and the low-level drawing API (OpenGL). It manages
/// render states efficiently, and provides a lightweight
/// abstraction for rendering geometry.
///
/// The purpose of this class is to provide a single abstract
/// entry point for everything related to low-level rendering.
/// Hiding everything behind sf::Renderer makes optimizing
/// easy, as well as porting to other technologies in the future
/// (like OpenGL ES or OpenGL 3.x).
///
/// This class is mainly meant for internal usage, you should
/// never care about it unless you write your own sf::Drawable
/// class that uses raw geometry in its Render function.
///
////////////////////////////////////////////////////////////
