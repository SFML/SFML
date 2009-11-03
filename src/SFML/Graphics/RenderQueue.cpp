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

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics/RenderQueue.hpp>
#include <SFML/Graphics/Batch.hpp>
#include <SFML/Graphics/GeometryRenderer.hpp>
#include <SFML/Graphics/GLCheck.hpp>


////////////////////////////////////////////////////////////
// Private data
////////////////////////////////////////////////////////////
namespace
{
    // Fast float to int conversion
    inline sf::Int32 Round(double value)
    {
        // Use a union rather than reinterpret_cast, because it doesn't break strict-aliasing
        // rules and results in a correct behaviour when compiling in optimized mode
        union DoubleToInt
        {
            double d; 
            sf::Int32 i[2];
        };

        DoubleToInt u;
        u.d = value + 6755399441055744.0;

        #if defined(SFML_ENDIAN_LITTLE)
            return u.i[0];
        #else
            return u.i[1];
        #endif
    } 
}


namespace sf
{
////////////////////////////////////////////////////////////
RenderQueue::RenderQueue() :
myCurrentTexture    (NULL),
myCurrentBlendMode  (Blend::Alpha),
myBaseIndex         (0),
myRenderer          (priv::GeometryRenderer::New()),
myCurrentBatch      (NULL),
myVertices          (512),
myIndices           (512),
myCurrentVertexCount(0),
myCurrentIndexCount (0)
{
    myCurrentStates = &myStatesStack[0];
}


////////////////////////////////////////////////////////////
RenderQueue::~RenderQueue()
{
    // Destroy the geometry renderer
    delete myRenderer;
}


////////////////////////////////////////////////////////////
void RenderQueue::PushStates()
{
    myCurrentStates++;
    *myCurrentStates = *(myCurrentStates - 1);
}


////////////////////////////////////////////////////////////
void RenderQueue::PopStates()
{
    myCurrentStates--;
}


////////////////////////////////////////////////////////////
void RenderQueue::SetModelView(const Matrix3& matrix)
{
    myCurrentStates->modelView = matrix;
}


////////////////////////////////////////////////////////////
void RenderQueue::ApplyModelView(const Matrix3& matrix)
{
    myCurrentStates->modelView *= matrix;
}


////////////////////////////////////////////////////////////
void RenderQueue::SetProjection(const Matrix3& matrix)
{
    myCurrentStates->projection = matrix;
}


////////////////////////////////////////////////////////////
void RenderQueue::ApplyProjection(const Matrix3& matrix)
{
    myCurrentStates->projection *= matrix;
}


////////////////////////////////////////////////////////////
void RenderQueue::SetColor(const Color& color)
{
    myCurrentStates->color = color;
}


////////////////////////////////////////////////////////////
void RenderQueue::ApplyColor(const Color& color)
{
    myCurrentStates->color *= color;
}


////////////////////////////////////////////////////////////
void RenderQueue::SetViewport(const IntRect& viewport)
{
    myCurrentViewport = viewport;
    myCurrentViewportSize.x = viewport.GetSize().x / 2.f;
    myCurrentViewportSize.y = viewport.GetSize().y / 2.f;
}


////////////////////////////////////////////////////////////
void RenderQueue::SetBlendMode(Blend::Mode mode)
{
    myCurrentBlendMode = mode;
}


////////////////////////////////////////////////////////////
void RenderQueue::SetTexture(const Image* texture)
{
    myCurrentTexture = texture;
}


////////////////////////////////////////////////////////////
void RenderQueue::SetShader(const Shader* shader)
{
    myCurrentShader = shader;
}


////////////////////////////////////////////////////////////
void RenderQueue::BeginBatch()
{
    // Check if the current batch differs from the new render states
    if (!myCurrentBatch || !myCurrentBatch->Matches(myCurrentTexture, myCurrentShader, myCurrentBlendMode, myCurrentViewport))
    {
        // Close the current batch
        if (myCurrentBatch)
            myCurrentBatch->End(myCurrentIndexCount);

        // Create a new one
        priv::Batch batch(myCurrentTexture, myCurrentShader, myCurrentBlendMode, myCurrentViewport);
        myBatches.push_back(batch);
        myCurrentBatch = &myBatches.back();
        myCurrentBatch->Begin(myCurrentIndexCount);
    }

    // Update the combined transform matrix
    myCurrentTransform = myCurrentStates->projection * myCurrentStates->modelView;

    // Update the current base index
    myBaseIndex = myCurrentVertexCount / 8;
}


////////////////////////////////////////////////////////////
void RenderQueue::AddVertex(float x, float y)
{
    AddVertex(x, y, 0.f, 0.f, Color::White);
}


////////////////////////////////////////////////////////////
void RenderQueue::AddVertex(float x, float y, float u, float v)
{
    AddVertex(x, y, u, v, Color::White);
}


////////////////////////////////////////////////////////////
void RenderQueue::AddVertex(float x, float y, const Color& color)
{
    AddVertex(x, y, 0.f, 0.f, color);
}


////////////////////////////////////////////////////////////
void RenderQueue::AddVertex(float x, float y, float u, float v, const Color& color)
{
    // Apply the current transform matrix to the vertex position
    sf::Vector2f transformedPoint = myCurrentTransform.Transform(sf::Vector2f(x, y));

    // Apply the current global color
    sf::Color combinedColor = myCurrentStates->color * color;

    // Round the vertex position so that it matches the
    // viewport's pixels, and thus avoid rendering artifacts
    int i1 = Round((transformedPoint.x + 1.f) * myCurrentViewportSize.x);
    int i2 = Round((transformedPoint.y + 1.f) * myCurrentViewportSize.y);
    transformedPoint.x = i1 / myCurrentViewportSize.x - 1.f;
    transformedPoint.y = i2 / myCurrentViewportSize.y - 1.f;

    // Here we choose not to rely on vector::clear and vector::push_back,
    // and to handle resizing and appending manually, for performances reasons

    // Resize the vertex buffer if it is too small
    std::size_t size = myVertices.size();
    if (myCurrentVertexCount + 8 > size)
        myVertices.resize(size + size / 2);

    // Copy the vertex data
    float* ptr = &myVertices[myCurrentVertexCount];
    *ptr++ = transformedPoint.x;
    *ptr++ = transformedPoint.y;
    *ptr++ = combinedColor.r / 255.f;
    *ptr++ = combinedColor.g / 255.f;
    *ptr++ = combinedColor.b / 255.f;
    *ptr++ = combinedColor.a / 255.f;
    *ptr++ = u;
    *ptr++ = v;

    // Increase the vertex count
    myCurrentVertexCount += 8;
}


////////////////////////////////////////////////////////////
void RenderQueue::AddTriangle(unsigned int index0, unsigned int index1, unsigned int index2)
{
    // Here we choose not to rely on vector::clear and vector::push_back,
    // and to handle resizing and appending manually, for performances reasons

    // Resize the index buffer if it is too small
    std::size_t size = myIndices.size();
    if (myCurrentIndexCount + 3 > size)
        myIndices.resize(size + size / 2);

    // Copy the index data
    myIndices[myCurrentIndexCount + 0] = index0 + myBaseIndex;
    myIndices[myCurrentIndexCount + 1] = index1 + myBaseIndex;
    myIndices[myCurrentIndexCount + 2] = index2 + myBaseIndex;

    // Increase the index count
    myCurrentIndexCount += 3;
}


////////////////////////////////////////////////////////////
void RenderQueue::Render()
{
    if (myCurrentVertexCount && myCurrentIndexCount)
    {
        // Save the current OpenGL states
        GLCheck(glPushAttrib(GL_COLOR_BUFFER_BIT | GL_ENABLE_BIT | GL_TEXTURE_BIT | GL_VIEWPORT_BIT));
        GLCheck(glPushClientAttrib(GL_CLIENT_VERTEX_ARRAY_BIT));
        GLCheck(glDisable(GL_LIGHTING));
        GLCheck(glDisable(GL_DEPTH_TEST));
        GLCheck(glEnable(GL_ALPHA_TEST));
        GLCheck(glAlphaFunc(GL_GREATER, 0));
        GLCheck(glMatrixMode(GL_MODELVIEW));
        GLCheck(glPushMatrix());
        GLCheck(glLoadIdentity());
        GLCheck(glMatrixMode(GL_PROJECTION));
        GLCheck(glPushMatrix());
        GLCheck(glLoadIdentity());

        // Close the last batch
        if (myCurrentBatch)
            myCurrentBatch->End(myCurrentIndexCount);

        // Prepare the geometry renderer
        myRenderer->Begin(&myVertices[0], myCurrentVertexCount, &myIndices[0], myCurrentIndexCount);

        // Render the batches in order
        for (BatchArray::const_iterator it = myBatches.begin(); it != myBatches.end(); ++it)
            it->Render(*myRenderer);

        // Stop rendering
        myRenderer->End();

        // Restore the previous OpenGL states
        GLCheck(glMatrixMode(GL_PROJECTION));
        GLCheck(glPopMatrix());
        GLCheck(glMatrixMode(GL_MODELVIEW));
        GLCheck(glPopMatrix());
        GLCheck(glPopAttrib());
        GLCheck(glPopClientAttrib());
    }

    // Clear everything
    Clear();
}


////////////////////////////////////////////////////////////
void RenderQueue::Clear()
{
    myBatches.clear();
    myCurrentBatch = NULL;
    myCurrentVertexCount = 0;
    myCurrentIndexCount = 0;
}

} // namespace sf
