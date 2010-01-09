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

#ifndef SFML_RENDERQUEUE_HPP
#define SFML_RENDERQUEUE_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Config.hpp>
#include <SFML/System/NonCopyable.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Matrix3.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <vector>


namespace sf
{
namespace priv
{
    class Batch;
    class GeometryRenderer;
}

class Image;
class Shader;

////////////////////////////////////////////////////////////
/// \brief Implements a queue of rendering commands
///
////////////////////////////////////////////////////////////
class SFML_API RenderQueue : NonCopyable
{
public :

    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    ////////////////////////////////////////////////////////////
    RenderQueue();

    ////////////////////////////////////////////////////////////
    /// \brief Destructor
    ///
    ////////////////////////////////////////////////////////////
    ~RenderQueue();

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
    /// \brief Combine a new projection matrix with the current one
    ///
    /// Note: any call to this function after a call to BeginBatch
    /// will be ignored, and delayed until BeginBatch is called again.
    ///
    /// \param matrix Model-view matrix to combine
    ///
    /// \see SetProjection
    ///
    ////////////////////////////////////////////////////////////
    void ApplyProjection(const Matrix3& matrix);

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
    /// \brief Begin a new geometry batch
    ///
    /// This function starts storing geometry and associates it
    /// to the current render states (viewport, color, blending, transform).
    /// Note: There's no EndBatch, a batch ends as soon as BeginBatch
    /// is called again.
    ///
    ////////////////////////////////////////////////////////////
    void BeginBatch();

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

    ////////////////////////////////////////////////////////////
    /// \brief Add a new triangle to be rendered
    ///
    /// This function adds a new triangle, using indices of previously
    /// added vertices. Note that the index base is set to 0
    /// everytime a new batch is started (BeginBatch).
    ///
    /// Example:
    /// \begincode
    /// queue.BeginBatch();
    /// queue.AddVertex(...);
    /// queue.AddVertex(...);
    /// queue.AddVertex(...);
    /// queue.AddTriangle(0, 1, 2);
    /// \endcode
    ///
    /// \param index0 Index of the first vertex of the triangle
    /// \param index1 Index of the second vertex of the triangle
    /// \param index2 Index of the third vertex of the triangle
    ///
    ////////////////////////////////////////////////////////////
    void AddTriangle(std::size_t index0, std::size_t index1, std::size_t index2);

    ////////////////////////////////////////////////////////////
    /// \brief Render the content of the whole queue
    ///
    /// After everything has been rendered, the render queue is
    /// automatically cleared.
    ///
    /// \see Clear
    ///
    ////////////////////////////////////////////////////////////
    void Render();

    ////////////////////////////////////////////////////////////
    /// \brief Clear the whole queue without rendering it
    ///
    ////////////////////////////////////////////////////////////
    void Clear();

private :

    ////////////////////////////////////////////////////////////
    // Structure holding the render states that can be stacked
    ////////////////////////////////////////////////////////////
    struct RenderStates
    {
        RenderStates() : color(255, 255, 255, 255) {}

        Matrix3 modelView;  ///< Model-view matrix
        Matrix3 projection; ///< Projection matrix
        Color   color;      ///< Vertex color
    };

    ////////////////////////////////////////////////////////////
    // Types
    ////////////////////////////////////////////////////////////
    typedef std::vector<priv::Batch>  BatchArray;
    typedef std::vector<float>        VertexArray;
    typedef std::vector<Uint32>       IndexArray;

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    RenderStates            myStatesStack[16];     ///< Stack of render states
    RenderStates*           myCurrentStates;       ///< Current set of render states
    Matrix3                 myCurrentTransform;    ///< Current combined projection-model-view matrix
    const Image*            myCurrentTexture;      ///< Current texture
    const Shader*           myCurrentShader;       ///< Current pixel shader
    Blend::Mode             myCurrentBlendMode;    ///< Current blending mode
    IntRect                 myCurrentViewport;     ///< Current target viewport
    Vector2f                myCurrentViewportSize; ///< Size of the current viewport (for vertex calculations)
    Uint32                  myBaseIndex;           ///< Base vertex index for the current batch
    priv::GeometryRenderer* myRenderer;            ///< Optimized geometry renderer
    priv::Batch*            myCurrentBatch;        ///< Current geometry block
    BatchArray              myBatches;             ///< Blocks of geometry to render
    VertexArray             myVertices;            ///< Geometry to be rendered
    IndexArray              myIndices;             ///< Indices defining the triangles to be rendered
    Uint32					myCurrentVertexCount;  ///< Current number of vertices in the vertex array
    Uint32					myCurrentIndexCount;   ///< Current number of indices in the index array
};

} // namespace sf


#endif // SFML_RENDERQUEUE_HPP


////////////////////////////////////////////////////////////
/// \class sf::RenderQueue
///
/// The RenderQueue class allows to delay the actual rendering
/// by storing the sequence of render states and geometry.
///
/// Delaying rendering is crucial in order to implement batching
/// (grouping all the geometry using the same states, and sending
/// it to the graphics card with only one call), which allow
/// huge improvements in performances.
///
/// Usage example:
/// \begincode
/// void MyDrawable::Render(sf::RenderTarget& target, sf::RenderQueue& queue)
/// {
///    queue.SetTexture(myImage);
///    queue.BeginBatch();
///    {
///       queue.AddVertex(...);
///       queue.AddVertex(...);
///       queue.AddVertex(...);
///
///       queue.AddTriangle(0, 1, 2);
///    }
/// }
/// \endcode
///
/// Note that this class is meant for internal use only
/// (it is used by render targets), unless you want to
/// inherit from sf::Drawable.
///
////////////////////////////////////////////////////////////
