////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2013 Laurent Gomila (laurent.gom@gmail.com)
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
#include <SFML/System/Err.hpp>
#include <iostream>


namespace sf
{
////////////////////////////////////////////////////////////
RenderTarget::RenderTarget() :
m_defaultView(),
m_view       (),
m_cache      ()
{
    m_cache.glStatesSet = false;
}


////////////////////////////////////////////////////////////
RenderTarget::~RenderTarget()
{
}


////////////////////////////////////////////////////////////
void RenderTarget::clear(const Color& color)
{
    if (activate(true))
    {
        glCheck(glClearColor(color.r / 255.f, color.g / 255.f, color.b / 255.f, color.a / 255.f));
        glCheck(glClear(GL_COLOR_BUFFER_BIT));
    }
}


////////////////////////////////////////////////////////////
void RenderTarget::setView(const View& view)
{
    m_view = view;
    m_cache.viewChanged = true;
}


////////////////////////////////////////////////////////////
const View& RenderTarget::getView() const
{
    return m_view;
}


////////////////////////////////////////////////////////////
const View& RenderTarget::getDefaultView() const
{
    return m_defaultView;
}


////////////////////////////////////////////////////////////
IntRect RenderTarget::getViewport(const View& view) const
{
    float width  = static_cast<float>(getSize().x);
    float height = static_cast<float>(getSize().y);
    const FloatRect& viewport = view.getViewport();

    return IntRect(static_cast<int>(0.5f + width  * viewport.left),
                   static_cast<int>(0.5f + height * viewport.top),
                   static_cast<int>(width  * viewport.width),
                   static_cast<int>(height * viewport.height));
}


////////////////////////////////////////////////////////////
Vector2f RenderTarget::mapPixelToCoords(const Vector2i& point) const
{
    return mapPixelToCoords(point, getView());
}


////////////////////////////////////////////////////////////
Vector2f RenderTarget::mapPixelToCoords(const Vector2i& point, const View& view) const
{
    // First, convert from viewport coordinates to homogeneous coordinates
    Vector2f normalized;
    IntRect viewport = getViewport(view);
    normalized.x = -1.f + 2.f * (point.x - viewport.left) / viewport.width;
    normalized.y =  1.f - 2.f * (point.y - viewport.top)  / viewport.height;

    // Then transform by the inverse of the view matrix
    return view.getInverseTransform().transformPoint(normalized);
}

////////////////////////////////////////////////////////////
Vector2i RenderTarget::mapCoordsToPixel(const Vector2f& point) const
{
    return mapCoordsToPixel(point, getView());
}

////////////////////////////////////////////////////////////
Vector2i RenderTarget::mapCoordsToPixel(const Vector2f& point, const View& view) const
{
    // First, transform the point by the view matrix
    Vector2f normalized = view.getTransform().transformPoint(point);

    // Then convert to viewport coordinates
    Vector2i pixel;
    IntRect viewport = getViewport(view);
    pixel.x = static_cast<int>(( normalized.x + 1.f) / 2.f * viewport.width  + viewport.left);
    pixel.y = static_cast<int>((-normalized.y + 1.f) / 2.f * viewport.height + viewport.top);

    return pixel;
}

////////////////////////////////////////////////////////////
void RenderTarget::draw(const Drawable& drawable, const RenderStates& states)
{
    drawable.draw(*this, states);
}


////////////////////////////////////////////////////////////
void RenderTarget::draw(const Vertex* vertices, unsigned int vertexCount,
                        PrimitiveType type, const RenderStates& states)
{
    // Nothing to draw?
    if (!vertices || (vertexCount == 0))
        return;

    if (activate(true))
    {
        // First set the persistent OpenGL states if it's the very first call
        if (!m_cache.glStatesSet)
            resetGLStates();

        // Check if the vertex count is low enough so that we can pre-transform them
        bool useVertexCache = (vertexCount <= StatesCache::VertexCacheSize);
        if (useVertexCache)
        {
            // Pre-transform the vertices and store them into the vertex cache
            for (unsigned int i = 0; i < vertexCount; ++i)
            {
                Vertex& vertex = m_cache.vertexCache[i];
                vertex.position = states.transform * vertices[i].position;
                vertex.color = vertices[i].color;
                vertex.texCoords = vertices[i].texCoords;
            }

            // Since vertices are transformed, we must use an identity transform to render them
            if (!m_cache.useVertexCache)
                applyTransform(Transform::Identity);
        }
        else
        {
            applyTransform(states.transform);
        }

        // Apply the view
        if (m_cache.viewChanged)
            applyCurrentView();

        // Apply the blend mode
        if (states.blendMode != m_cache.lastBlendMode)
            applyBlendMode(states.blendMode);

        // Apply the texture
        Uint64 textureId = states.texture ? states.texture->m_cacheId : 0;
        if (textureId != m_cache.lastTextureId)
            applyTexture(states.texture);

        // Apply the shader
        if (states.shader)
            applyShader(states.shader);

        // If we pre-transform the vertices, we must use our internal vertex cache
        if (useVertexCache)
        {
            // ... and if we already used it previously, we don't need to set the pointers again
            if (!m_cache.useVertexCache)
                vertices = m_cache.vertexCache;
            else
                vertices = NULL;
        }

        // Setup the pointers to the vertices' components
        if (vertices)
        {
            const char* data = reinterpret_cast<const char*>(vertices);
            glCheck(glVertexPointer(2, GL_FLOAT, sizeof(Vertex), data + 0));
            glCheck(glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(Vertex), data + 8));
            glCheck(glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), data + 12));
        }

        // Find the OpenGL primitive type
        static const GLenum modes[] = {GL_POINTS, GL_LINES, GL_LINE_STRIP, GL_TRIANGLES,
                                       GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN, GL_QUADS};
        GLenum mode = modes[type];

        // Draw the primitives
        glCheck(glDrawArrays(mode, 0, vertexCount));

        // Unbind the shader, if any
        if (states.shader)
            applyShader(NULL);

        // Update the cache
        m_cache.useVertexCache = useVertexCache;
    }
}


////////////////////////////////////////////////////////////
void RenderTarget::pushGLStates()
{
    if (activate(true))
    {
#ifdef SFML_DEBUG
        // make sure that the user didn't leave an unchecked OpenGL error
        GLenum error = glGetError();
        if (error != GL_NO_ERROR)
        {
            err() << "OpenGL error (" << error << ") detected in user code, "
                  << "you should check for errors with glGetError()"
                  << std::endl;
        }
#endif

        glCheck(glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS));
        glCheck(glPushAttrib(GL_ALL_ATTRIB_BITS));
        glCheck(glMatrixMode(GL_MODELVIEW));
        glCheck(glPushMatrix());
        glCheck(glMatrixMode(GL_PROJECTION));
        glCheck(glPushMatrix());
        glCheck(glMatrixMode(GL_TEXTURE));
        glCheck(glPushMatrix());
    }

    resetGLStates();
}


////////////////////////////////////////////////////////////
void RenderTarget::popGLStates()
{
    if (activate(true))
    {
        glCheck(glMatrixMode(GL_PROJECTION));
        glCheck(glPopMatrix());
        glCheck(glMatrixMode(GL_MODELVIEW));
        glCheck(glPopMatrix());
        glCheck(glMatrixMode(GL_TEXTURE));
        glCheck(glPopMatrix());
        glCheck(glPopClientAttrib());
        glCheck(glPopAttrib());
    }
}


////////////////////////////////////////////////////////////
void RenderTarget::resetGLStates()
{
    if (activate(true))
    {
        // Make sure that GLEW is initialized
        priv::ensureGlewInit();

        // Define the default OpenGL states
        glCheck(glDisable(GL_CULL_FACE));
        glCheck(glDisable(GL_LIGHTING));
        glCheck(glDisable(GL_DEPTH_TEST));
        glCheck(glDisable(GL_ALPHA_TEST));
        glCheck(glEnable(GL_TEXTURE_2D));
        glCheck(glEnable(GL_BLEND));
        glCheck(glMatrixMode(GL_MODELVIEW));
        glCheck(glEnableClientState(GL_VERTEX_ARRAY));
        glCheck(glEnableClientState(GL_COLOR_ARRAY));
        glCheck(glEnableClientState(GL_TEXTURE_COORD_ARRAY));
        m_cache.glStatesSet = true;

        // Apply the default SFML states
        applyBlendMode(BlendAlpha);
        applyTransform(Transform::Identity);
        applyTexture(NULL);
        if (Shader::isAvailable())
            applyShader(NULL);
        m_cache.useVertexCache = false;

        // Set the default view
        setView(getView());
    }
}


////////////////////////////////////////////////////////////
void RenderTarget::initialize()
{
    // Setup the default and current views
    m_defaultView.reset(FloatRect(0, 0, static_cast<float>(getSize().x), static_cast<float>(getSize().y)));
    m_view = m_defaultView;

    // Set GL states only on first draw, so that we don't pollute user's states
    m_cache.glStatesSet = false;
}


////////////////////////////////////////////////////////////
void RenderTarget::applyCurrentView()
{
    // Set the viewport
    IntRect viewport = getViewport(m_view);
    int top = getSize().y - (viewport.top + viewport.height);
    glCheck(glViewport(viewport.left, top, viewport.width, viewport.height));

    // Set the projection matrix
    glCheck(glMatrixMode(GL_PROJECTION));
    glCheck(glLoadMatrixf(m_view.getTransform().getMatrix()));

    // Go back to model-view mode
    glCheck(glMatrixMode(GL_MODELVIEW));

    m_cache.viewChanged = false;
}


////////////////////////////////////////////////////////////
void RenderTarget::applyBlendMode(BlendMode mode)
{
    switch (mode)
    {
        // glBlendFuncSeparateEXT is used when available to avoid an incorrect alpha value when the target
        // is a RenderTexture -- in this case the alpha value must be written directly to the target buffer

        // Alpha blending
        default :
        case BlendAlpha :
            if (GLEW_EXT_blend_func_separate)
                glCheck(glBlendFuncSeparateEXT(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA));
            else
                glCheck(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
            break;

        // Additive blending
        case BlendAdd :
            if (GLEW_EXT_blend_func_separate)
                glCheck(glBlendFuncSeparateEXT(GL_SRC_ALPHA, GL_ONE, GL_ONE, GL_ONE));
            else
                glCheck(glBlendFunc(GL_SRC_ALPHA, GL_ONE));
            break;

        // Multiplicative blending
        case BlendMultiply :
            glCheck(glBlendFunc(GL_DST_COLOR, GL_ZERO));
            break;

        // No blending
        case BlendNone :
            glCheck(glBlendFunc(GL_ONE, GL_ZERO));
            break;
    }

    m_cache.lastBlendMode = mode;
}


////////////////////////////////////////////////////////////
void RenderTarget::applyTransform(const Transform& transform)
{
    // No need to call glMatrixMode(GL_MODELVIEW), it is always the
    // current mode (for optimization purpose, since it's the most used)
    glCheck(glLoadMatrixf(transform.getMatrix()));
}


////////////////////////////////////////////////////////////
void RenderTarget::applyTexture(const Texture* texture)
{
    Texture::bind(texture, Texture::Pixels);

    m_cache.lastTextureId = texture ? texture->m_cacheId : 0;
}


////////////////////////////////////////////////////////////
void RenderTarget::applyShader(const Shader* shader)
{
    Shader::bind(shader);
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
