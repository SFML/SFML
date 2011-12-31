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
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/GLCheck.hpp>
#include <iostream>


namespace sf
{
////////////////////////////////////////////////////////////
RenderTarget::RenderTarget() :
myDefaultView(),
myView       (),
myCache      ()
{
}


////////////////////////////////////////////////////////////
RenderTarget::~RenderTarget()
{
}


////////////////////////////////////////////////////////////
void RenderTarget::Clear(const Color& color)
{
    if (Activate(true))
    {
        GLCheck(glClearColor(color.r / 255.f, color.g / 255.f, color.b / 255.f, color.a / 255.f));
        GLCheck(glClear(GL_COLOR_BUFFER_BIT));
    }
}


////////////////////////////////////////////////////////////
void RenderTarget::SetView(const View& view)
{
    myView = view;
    myCache.ViewChanged = true;
}


////////////////////////////////////////////////////////////
const View& RenderTarget::GetView() const
{
    return myView;
}


////////////////////////////////////////////////////////////
const View& RenderTarget::GetDefaultView() const
{
    return myDefaultView;
}


////////////////////////////////////////////////////////////
IntRect RenderTarget::GetViewport(const View& view) const
{
    float width  = static_cast<float>(GetWidth());
    float height = static_cast<float>(GetHeight());
    const FloatRect& viewport = view.GetViewport();

    return IntRect(static_cast<int>(0.5f + width  * viewport.Left),
                   static_cast<int>(0.5f + height * viewport.Top),
                   static_cast<int>(width  * viewport.Width),
                   static_cast<int>(height * viewport.Height));
}


////////////////////////////////////////////////////////////
Vector2f RenderTarget::ConvertCoords(unsigned int x, unsigned int y) const
{
    return ConvertCoords(x, y, GetView());
}


////////////////////////////////////////////////////////////
Vector2f RenderTarget::ConvertCoords(unsigned int x, unsigned int y, const View& view) const
{
    // First, convert from viewport coordinates to homogeneous coordinates
    Vector2f coords;
    IntRect viewport = GetViewport(view);
    coords.x = -1.f + 2.f * (static_cast<int>(x) - viewport.Left) / viewport.Width;
    coords.y = 1.f  - 2.f * (static_cast<int>(y) - viewport.Top)  / viewport.Height;

    // Then transform by the inverse of the view matrix
    return view.GetInverseTransform().TransformPoint(coords);
}


////////////////////////////////////////////////////////////
void RenderTarget::Draw(const Drawable& drawable, const RenderStates& states)
{
    drawable.Draw(*this, states);
}


////////////////////////////////////////////////////////////
void RenderTarget::Draw(const Vertex* vertices, unsigned int vertexCount,
                        PrimitiveType type, const RenderStates& states)
{
    // Nothing to draw?
    if (!vertices || (vertexCount == 0))
        return;

    if (Activate(true))
    {
        // Check if the vertex count is low enough so that we can pre-transform them
        bool useVertexCache = (vertexCount <= StatesCache::VertexCacheSize);
        if (useVertexCache)
        {
            // Pre-transform the vertices and store them into the vertex cache
            for (unsigned int i = 0; i < vertexCount; ++i)
            {
                Vertex& vertex = myCache.VertexCache[i];
                vertex.Position = states.Transform * vertices[i].Position;
                vertex.Color = vertices[i].Color;
                vertex.TexCoords = vertices[i].TexCoords;
            }

            // Since vertices are transformed, we must use an identity transform to render them
            if (!myCache.UseVertexCache)
                ApplyTransform(Transform::Identity);
        }
        else
        {
            ApplyTransform(states.Transform);
        }

        // Apply the view
        if (myCache.ViewChanged)
            ApplyCurrentView();

        // Apply the blend mode
        if (states.BlendMode != myCache.LastBlendMode)
            ApplyBlendMode(states.BlendMode);

        // Apply the texture
        Uint64 textureId = states.Texture ? states.Texture->myCacheId : 0;
        if (textureId != myCache.LastTextureId)
            ApplyTexture(states.Texture);

        // Apply the shader
        if (states.Shader)
            ApplyShader(states.Shader);

        // If we pre-transform the vertices, we must use our internal vertex cache
        if (useVertexCache)
        {
            // ... and if we already used it previously, we don't need to set the pointers again
            if (!myCache.UseVertexCache)
                vertices = myCache.VertexCache;
            else
                vertices = NULL;
        }

        // Setup the pointers to the vertices' components
        if (vertices)
        {
            const char* data = reinterpret_cast<const char*>(vertices);
            GLCheck(glVertexPointer(2, GL_FLOAT, sizeof(Vertex), data + 0));
            GLCheck(glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(Vertex), data + 8));
            GLCheck(glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), data + 12));
        }

        // Find the OpenGL primitive type
        static const GLenum modes[] = {GL_POINTS, GL_LINES, GL_LINE_STRIP, GL_TRIANGLES,
                                       GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN, GL_QUADS};
        GLenum mode = modes[type];

        // Draw the primitives
        GLCheck(glDrawArrays(mode, 0, vertexCount));

        // Unbind the shader, if any
        if (states.Shader)
            ApplyShader(NULL);

        // Update the cache
        myCache.UseVertexCache = useVertexCache;
    }
}


////////////////////////////////////////////////////////////
void RenderTarget::PushGLStates()
{
    if (Activate(true))
    {
        GLCheck(glPushAttrib(GL_ALL_ATTRIB_BITS));
        GLCheck(glMatrixMode(GL_MODELVIEW));
        GLCheck(glPushMatrix());
        GLCheck(glMatrixMode(GL_PROJECTION));
        GLCheck(glPushMatrix());
        GLCheck(glMatrixMode(GL_TEXTURE));
        GLCheck(glPushMatrix());
    }

    ResetGLStates();
}


////////////////////////////////////////////////////////////
void RenderTarget::PopGLStates()
{
    if (Activate(true))
    {
        GLCheck(glPopAttrib());
        GLCheck(glMatrixMode(GL_PROJECTION));
        GLCheck(glPopMatrix());
        GLCheck(glMatrixMode(GL_MODELVIEW));
        GLCheck(glPopMatrix());
        GLCheck(glMatrixMode(GL_TEXTURE));
        GLCheck(glPopMatrix());
    }
}


////////////////////////////////////////////////////////////
void RenderTarget::ResetGLStates()
{
    if (Activate(true))
    {
        // Make sure that GLEW is initialized
        priv::EnsureGlewInit();

        // Define the default OpenGL states
        GLCheck(glDisable(GL_LIGHTING));
        GLCheck(glDisable(GL_DEPTH_TEST));
        GLCheck(glEnable(GL_TEXTURE_2D));
        GLCheck(glEnable(GL_ALPHA_TEST));
        GLCheck(glEnable(GL_BLEND));
        GLCheck(glAlphaFunc(GL_GREATER, 0));
        GLCheck(glMatrixMode(GL_MODELVIEW));
        GLCheck(glEnableClientState(GL_VERTEX_ARRAY));
        GLCheck(glEnableClientState(GL_COLOR_ARRAY));
        GLCheck(glEnableClientState(GL_TEXTURE_COORD_ARRAY));

        // Apply the default SFML states
        ApplyBlendMode(BlendAlpha);
        ApplyTransform(Transform::Identity);
        ApplyTexture(NULL);
        if (Shader::IsAvailable())
            ApplyShader(NULL);
        myCache.UseVertexCache = false;

        // Set the default view
        SetView(GetView());
    }
}


////////////////////////////////////////////////////////////
void RenderTarget::Initialize()
{
    // Setup the default and current views
    myDefaultView.Reset(FloatRect(0, 0, static_cast<float>(GetWidth()), static_cast<float>(GetHeight())));
    myView = myDefaultView;

    // Initialize the default OpenGL render-states
    ResetGLStates();
}


////////////////////////////////////////////////////////////
void RenderTarget::ApplyCurrentView()
{
    // Set the viewport
    IntRect viewport = GetViewport(myView);
    int top = GetHeight() - (viewport.Top + viewport.Height);
    GLCheck(glViewport(viewport.Left, top, viewport.Width, viewport.Height));

    // Set the projection matrix
    GLCheck(glMatrixMode(GL_PROJECTION));
    GLCheck(glLoadMatrixf(myView.GetTransform().GetMatrix()));

    // Go back to model-view mode
    GLCheck(glMatrixMode(GL_MODELVIEW));

    myCache.ViewChanged = false;
}


////////////////////////////////////////////////////////////
void RenderTarget::ApplyBlendMode(BlendMode mode)
{
    switch (mode)
    {
        // Alpha blending
        // glBlendFuncSeparateEXT is used when available to avoid an incorrect alpha value when the target
        // is a RenderTexture -- in this case the alpha value must be written directly to the target buffer
        default :
        case BlendAlpha :
            if (GLEW_EXT_blend_func_separate)
                GLCheck(glBlendFuncSeparateEXT(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA));
            else
                GLCheck(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
            break;

        // Additive blending
        case BlendAdd :
            GLCheck(glBlendFunc(GL_SRC_ALPHA, GL_ONE));
            break;

        // Multiplicative blending
        case BlendMultiply :
            GLCheck(glBlendFunc(GL_DST_COLOR, GL_ZERO));
            break;

        // No blending
        case BlendNone :
            GLCheck(glBlendFunc(GL_ONE, GL_ZERO));
            break;
    }

    myCache.LastBlendMode = mode;
}


////////////////////////////////////////////////////////////
void RenderTarget::ApplyTransform(const Transform& transform)
{
    // No need to call glMatrixMode(GL_MODELVIEW), it is always the
    // current mode (for optimization purpose, since it's the most used)
    GLCheck(glLoadMatrixf(transform.GetMatrix()));
}


////////////////////////////////////////////////////////////
void RenderTarget::ApplyTexture(const Texture* texture)
{
    if (texture)
        texture->Bind(Texture::Pixels);
    else
        GLCheck(glBindTexture(GL_TEXTURE_2D, 0));

    myCache.LastTextureId = texture ? texture->myCacheId : 0;
}


////////////////////////////////////////////////////////////
void RenderTarget::ApplyShader(const Shader* shader)
{
    if (shader)
        shader->Bind();
    else
        GLCheck(glUseProgramObjectARB(0));
}

} // namespace sf


////////////////////////////////////////////////////////////
// Render states caching strategies
//
// * View
//   If SetView was called since last draw, the projection
//   matrix is updated. We don't need more, the view doesn't
//   change frequently.
//
// * Transform
//   The transform matrix is usually expensive because each
//   entity will most likely use a different transform. This can
//   lead, in worst case, to changing it every 4 vertices.
//   To avoid that, when the vertex count is low enough, we
//   pre-transform them and therefore use an identity transform
//   to render them.
//
// * Blending mode
//   It's a simple integral value, so we can easily check
//   whether the value to apply is the same as before or not.
//
// * Texture
//   Storing the pointer or OpenGL ID of the last used texture
//   is not enough; if the sf::Texture instance is destroyed,
//   both the pointer and the OpenGL ID might be recycled in
//   a new texture instance. We need to use our own unique
//   identifier system to ensure consistent caching.
//
// * Shader
//   Shaders are very hard to optimize, because they have
//   parameters that can be hard (if not impossible) to track,
//   like matrices or textures. The only optimization that we
//   do is that we avoid setting a null shader if there was
//   already none for the previous draw.
// 
////////////////////////////////////////////////////////////
