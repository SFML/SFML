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

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/GLCheck.hpp>
#include <SFML/Graphics/GLExtensions.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/VertexBuffer.hpp>

#include <SFML/Window/Context.hpp>

#include <SFML/System/EnumArray.hpp>
#include <SFML/System/Err.hpp>

#include <algorithm>
#include <mutex>
#include <ostream>
#include <unordered_map>

#include <cassert>
#include <cmath>
#include <cstddef>


namespace
{
// A nested named namespace is used here to allow unity builds of SFML.
namespace RenderTargetImpl
{
// Mutex to protect ID generation and our context-RenderTarget-map
std::recursive_mutex& getMutex()
{
    static std::recursive_mutex mutex;
    return mutex;
}

// Unique identifier, used for identifying RenderTargets when
// tracking the currently active RenderTarget within a given context
std::uint64_t getUniqueId()
{
    const std::lock_guard lock(getMutex());
    static std::uint64_t  id = 1; // start at 1, zero is "no RenderTarget"
    return id++;
}

// Map to help us detect whether a different RenderTarget
// has been activated within a single context
using ContextRenderTargetMap = std::unordered_map<std::uint64_t, std::uint64_t>;
ContextRenderTargetMap& getContextRenderTargetMap()
{
    static ContextRenderTargetMap contextRenderTargetMap;
    return contextRenderTargetMap;
}

// Check if a RenderTarget with the given ID is active in the current context
bool isActive(std::uint64_t id)
{
    const auto it = getContextRenderTargetMap().find(sf::Context::getActiveContextId());
    return (it != getContextRenderTargetMap().end()) && (it->second == id);
}

// Convert an sf::BlendMode::Factor constant to the corresponding OpenGL constant.
std::uint32_t factorToGlConstant(sf::BlendMode::Factor blendFactor)
{
    // clang-format off
    switch (blendFactor)
    {
        case sf::BlendMode::Factor::Zero:             return GL_ZERO;
        case sf::BlendMode::Factor::One:              return GL_ONE;
        case sf::BlendMode::Factor::SrcColor:         return GL_SRC_COLOR;
        case sf::BlendMode::Factor::OneMinusSrcColor: return GL_ONE_MINUS_SRC_COLOR;
        case sf::BlendMode::Factor::DstColor:         return GL_DST_COLOR;
        case sf::BlendMode::Factor::OneMinusDstColor: return GL_ONE_MINUS_DST_COLOR;
        case sf::BlendMode::Factor::SrcAlpha:         return GL_SRC_ALPHA;
        case sf::BlendMode::Factor::OneMinusSrcAlpha: return GL_ONE_MINUS_SRC_ALPHA;
        case sf::BlendMode::Factor::DstAlpha:         return GL_DST_ALPHA;
        case sf::BlendMode::Factor::OneMinusDstAlpha: return GL_ONE_MINUS_DST_ALPHA;
    }
    // clang-format on

    sf::err() << "Invalid value for sf::BlendMode::Factor! Fallback to sf::BlendMode::Factor::Zero." << std::endl;
    assert(false);
    return GL_ZERO;
}


// Convert an sf::BlendMode::Equation constant to the corresponding OpenGL constant.
std::uint32_t equationToGlConstant(sf::BlendMode::Equation blendEquation)
{
    switch (blendEquation)
    {
        case sf::BlendMode::Equation::Add:
            return GLEXT_GL_FUNC_ADD;
        case sf::BlendMode::Equation::Subtract:
            if (GLEXT_blend_subtract)
                return GLEXT_GL_FUNC_SUBTRACT;
            break;
        case sf::BlendMode::Equation::ReverseSubtract:
            if (GLEXT_blend_subtract)
                return GLEXT_GL_FUNC_REVERSE_SUBTRACT;
            break;
        case sf::BlendMode::Equation::Min:
            if (GLEXT_blend_minmax)
                return GLEXT_GL_MIN;
            break;
        case sf::BlendMode::Equation::Max:
            if (GLEXT_blend_minmax)
                return GLEXT_GL_MAX;
            break;
    }

    static bool warned = false;
    if (!warned)
    {
        sf::err() << "OpenGL extension EXT_blend_minmax or EXT_blend_subtract unavailable" << '\n'
                  << "Some blending equations will fallback to sf::BlendMode::Equation::Add" << '\n'
                  << "Ensure that hardware acceleration is enabled if available" << std::endl;

        warned = true;
    }

    return GLEXT_GL_FUNC_ADD;
}


// Convert an UpdateOperation constant to the corresponding OpenGL constant.
std::uint32_t stencilOperationToGlConstant(sf::StencilUpdateOperation operation)
{
    // clang-format off
    switch (operation)
    {
        case sf::StencilUpdateOperation::Keep:      return GL_KEEP;
        case sf::StencilUpdateOperation::Zero:      return GL_ZERO;
        case sf::StencilUpdateOperation::Replace:   return GL_REPLACE;
        case sf::StencilUpdateOperation::Increment: return GL_INCR;
        case sf::StencilUpdateOperation::Decrement: return GL_DECR;
        case sf::StencilUpdateOperation::Invert:    return GL_INVERT;
    }
    // clang-format on

    sf::err() << "Invalid value for sf::StencilUpdateOperation! Fallback to sf::StencilMode::Keep." << std::endl;
    assert(false);
    return GL_KEEP;
}


// Convert a Comparison constant to the corresponding OpenGL constant.
std::uint32_t stencilFunctionToGlConstant(sf::StencilComparison comparison)
{
    // clang-format off
    switch (comparison)
    {
        case sf::StencilComparison::Never:        return GL_NEVER;
        case sf::StencilComparison::Less:         return GL_LESS;
        case sf::StencilComparison::LessEqual:    return GL_LEQUAL;
        case sf::StencilComparison::Greater:      return GL_GREATER;
        case sf::StencilComparison::GreaterEqual: return GL_GEQUAL;
        case sf::StencilComparison::Equal:        return GL_EQUAL;
        case sf::StencilComparison::NotEqual:     return GL_NOTEQUAL;
        case sf::StencilComparison::Always:       return GL_ALWAYS;
    }
    // clang-format on

    sf::err() << "Invalid value for sf::StencilComparison! Fallback to sf::StencilMode::Always." << std::endl;
    assert(false);
    return GL_ALWAYS;
}
} // namespace RenderTargetImpl
} // namespace


namespace sf
{
////////////////////////////////////////////////////////////
void RenderTarget::clear(Color color)
{
    if (RenderTargetImpl::isActive(m_id) || setActive(true))
    {
        // Unbind texture to fix RenderTexture preventing clear
        applyTexture(nullptr);

        // Apply the view (scissor testing can affect clearing)
        if (!m_cache.enable || m_cache.viewChanged)
            applyCurrentView();

        glCheck(glClearColor(color.r / 255.f, color.g / 255.f, color.b / 255.f, color.a / 255.f));
        glCheck(glClear(GL_COLOR_BUFFER_BIT));
    }
}


////////////////////////////////////////////////////////////
void RenderTarget::clearStencil(StencilValue stencilValue)
{
    if (RenderTargetImpl::isActive(m_id) || setActive(true))
    {
        // Unbind texture to fix RenderTexture preventing clear
        applyTexture(nullptr);

        // Apply the view (scissor testing can affect clearing)
        if (!m_cache.enable || m_cache.viewChanged)
            applyCurrentView();

        glCheck(glClearStencil(static_cast<int>(stencilValue.value)));
        glCheck(glClear(GL_STENCIL_BUFFER_BIT));
    }
}


////////////////////////////////////////////////////////////
void RenderTarget::clear(Color color, StencilValue stencilValue)
{
    if (RenderTargetImpl::isActive(m_id) || setActive(true))
    {
        // Unbind texture to fix RenderTexture preventing clear
        applyTexture(nullptr);

        // Apply the view (scissor testing can affect clearing)
        if (!m_cache.enable || m_cache.viewChanged)
            applyCurrentView();

        glCheck(glClearColor(color.r / 255.f, color.g / 255.f, color.b / 255.f, color.a / 255.f));
        glCheck(glClearStencil(static_cast<int>(stencilValue.value)));
        glCheck(glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));
    }
}


////////////////////////////////////////////////////////////
void RenderTarget::setView(const View& view)
{
    m_view              = view;
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
    const auto [width, height] = Vector2f(getSize());
    const FloatRect& viewport  = view.getViewport();

    return IntRect(Rect<long>({std::lround(width * viewport.position.x), std::lround(height * viewport.position.y)},
                              {std::lround(width * viewport.size.x), std::lround(height * viewport.size.y)}));
}


////////////////////////////////////////////////////////////
IntRect RenderTarget::getScissor(const View& view) const
{
    const auto [width, height] = Vector2f(getSize());
    const FloatRect& scissor   = view.getScissor();

    return IntRect(Rect<long>({std::lround(width * scissor.position.x), std::lround(height * scissor.position.y)},
                              {std::lround(width * scissor.size.x), std::lround(height * scissor.size.y)}));
}


////////////////////////////////////////////////////////////
Vector2f RenderTarget::mapPixelToCoords(Vector2i point) const
{
    return mapPixelToCoords(point, getView());
}


////////////////////////////////////////////////////////////
Vector2f RenderTarget::mapPixelToCoords(Vector2i point, const View& view) const
{
    // First, convert from viewport coordinates to homogeneous coordinates
    const FloatRect viewport = FloatRect(getViewport(view));
    const Vector2f
        normalized = Vector2f(-1, 1) +
                     Vector2f(2, -2).componentWiseMul(Vector2f(point) - viewport.position).componentWiseDiv(viewport.size);

    // Then transform by the inverse of the view matrix
    return view.getInverseTransform().transformPoint(normalized);
}


////////////////////////////////////////////////////////////
Vector2i RenderTarget::mapCoordsToPixel(Vector2f point) const
{
    return mapCoordsToPixel(point, getView());
}


////////////////////////////////////////////////////////////
Vector2i RenderTarget::mapCoordsToPixel(Vector2f point, const View& view) const
{
    // First, transform the point by the view matrix
    const Vector2f normalized = view.getTransform().transformPoint(point);

    // Then convert to viewport coordinates
    const FloatRect viewport = FloatRect(getViewport(view));
    return Vector2i(
        (normalized.componentWiseMul({1, -1}) + sf::Vector2f(1, 1)).componentWiseDiv({2, 2}).componentWiseMul(viewport.size) +
        viewport.position);
}


////////////////////////////////////////////////////////////
void RenderTarget::draw(const Drawable& drawable, const RenderStates& states)
{
    drawable.draw(*this, states);
}


////////////////////////////////////////////////////////////
void RenderTarget::draw(const Vertex* vertices, std::size_t vertexCount, PrimitiveType type, const RenderStates& states)
{
    // Nothing to draw?
    if (!vertices || (vertexCount == 0))
        return;

    if (RenderTargetImpl::isActive(m_id) || setActive(true))
    {
        // Check if the vertex count is low enough so that we can pre-transform them
        const bool useVertexCache = (vertexCount <= m_cache.vertexCache.size());

        if (useVertexCache)
        {
            // Pre-transform the vertices and store them into the vertex cache
            for (std::size_t i = 0; i < vertexCount; ++i)
            {
                Vertex& vertex   = m_cache.vertexCache[i];
                vertex.position  = states.transform * vertices[i].position;
                vertex.color     = vertices[i].color;
                vertex.texCoords = vertices[i].texCoords;
            }
        }

        setupDraw(useVertexCache, states);

        // Check if texture coordinates array is needed, and update client state accordingly
        const bool enableTexCoordsArray = (states.texture || states.shader);
        if (!m_cache.enable || (enableTexCoordsArray != m_cache.texCoordsArrayEnabled))
        {
            if (enableTexCoordsArray)
                glCheck(glEnableClientState(GL_TEXTURE_COORD_ARRAY));
            else
                glCheck(glDisableClientState(GL_TEXTURE_COORD_ARRAY));
        }

        // If we switch between non-cache and cache mode or enable texture
        // coordinates we need to set up the pointers to the vertices' components
        if (!m_cache.enable || !useVertexCache || !m_cache.useVertexCache)
        {
            const auto* data = reinterpret_cast<const std::byte*>(vertices);

            // If we pre-transform the vertices, we must use our internal vertex cache
            if (useVertexCache)
                data = reinterpret_cast<const std::byte*>(m_cache.vertexCache.data());

            glCheck(glVertexPointer(2, GL_FLOAT, sizeof(Vertex), data + 0));
            glCheck(glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(Vertex), data + 8));
            if (enableTexCoordsArray)
                glCheck(glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), data + 12));
        }
        else if (enableTexCoordsArray && !m_cache.texCoordsArrayEnabled)
        {
            // If we enter this block, we are already using our internal vertex cache
            const auto* data = reinterpret_cast<const std::byte*>(m_cache.vertexCache.data());

            glCheck(glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), data + 12));
        }

        drawPrimitives(type, 0, vertexCount);
        cleanupDraw(states);

        // Update the cache
        m_cache.useVertexCache        = useVertexCache;
        m_cache.texCoordsArrayEnabled = enableTexCoordsArray;
    }
}


////////////////////////////////////////////////////////////
void RenderTarget::draw(const VertexBuffer& vertexBuffer, const RenderStates& states)
{
    draw(vertexBuffer, 0, vertexBuffer.getVertexCount(), states);
}


////////////////////////////////////////////////////////////
void RenderTarget::draw(const VertexBuffer& vertexBuffer, std::size_t firstVertex, std::size_t vertexCount, const RenderStates& states)
{
    // VertexBuffer not supported?
    if (!VertexBuffer::isAvailable())
    {
        err() << "sf::VertexBuffer is not available, drawing skipped" << std::endl;
        return;
    }

    // Sanity check
    if (firstVertex > vertexBuffer.getVertexCount())
        return;

    // Clamp vertexCount to something that makes sense
    vertexCount = std::min(vertexCount, vertexBuffer.getVertexCount() - firstVertex);

    // Nothing to draw?
    if (!vertexCount || !vertexBuffer.getNativeHandle())
        return;

    if (RenderTargetImpl::isActive(m_id) || setActive(true))
    {
        setupDraw(false, states);

        // Bind vertex buffer
        VertexBuffer::bind(&vertexBuffer);

        // Always enable texture coordinates
        if (!m_cache.enable || !m_cache.texCoordsArrayEnabled)
            glCheck(glEnableClientState(GL_TEXTURE_COORD_ARRAY));

        glCheck(glVertexPointer(2, GL_FLOAT, sizeof(Vertex), reinterpret_cast<const void*>(0)));
        glCheck(glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(Vertex), reinterpret_cast<const void*>(8)));
        glCheck(glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), reinterpret_cast<const void*>(12)));

        drawPrimitives(vertexBuffer.getPrimitiveType(), firstVertex, vertexCount);

        // Unbind vertex buffer
        VertexBuffer::bind(nullptr);

        cleanupDraw(states);

        // Update the cache
        m_cache.useVertexCache        = false;
        m_cache.texCoordsArrayEnabled = true;
    }
}


////////////////////////////////////////////////////////////
bool RenderTarget::isSrgb() const
{
    // By default sRGB encoding is not enabled for an arbitrary RenderTarget
    return false;
}


////////////////////////////////////////////////////////////
bool RenderTarget::setActive(bool active)
{
    // Mark this RenderTarget as active or no longer active in the tracking map
    const std::lock_guard lock(RenderTargetImpl::getMutex());

    const std::uint64_t contextId = Context::getActiveContextId();

    using RenderTargetImpl::getContextRenderTargetMap;
    auto&      contextRenderTargetMap = getContextRenderTargetMap();
    const auto it                     = contextRenderTargetMap.find(contextId);

    if (active)
    {
        if (it == contextRenderTargetMap.end())
        {
            contextRenderTargetMap[contextId] = m_id;

            m_cache.glStatesSet = false;
            m_cache.enable      = false;
        }
        else if (it->second != m_id)
        {
            it->second = m_id;

            m_cache.enable = false;
        }
    }
    else
    {
        if (it != contextRenderTargetMap.end())
            contextRenderTargetMap.erase(it);

        m_cache.enable = false;
    }

    return true;
}


////////////////////////////////////////////////////////////
void RenderTarget::pushGLStates()
{
    if (RenderTargetImpl::isActive(m_id) || setActive(true))
    {
#ifdef SFML_DEBUG
        // make sure that the user didn't leave an unchecked OpenGL error
        const GLenum error = glGetError();
        if (error != GL_NO_ERROR)
        {
            err() << "OpenGL error (" << error << ") detected in user code, "
                  << "you should check for errors with glGetError()" << std::endl;
        }
#endif

#ifndef SFML_OPENGL_ES
        glCheck(glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS));
        glCheck(glPushAttrib(GL_ALL_ATTRIB_BITS));
#endif
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
    if (RenderTargetImpl::isActive(m_id) || setActive(true))
    {
        glCheck(glMatrixMode(GL_PROJECTION));
        glCheck(glPopMatrix());
        glCheck(glMatrixMode(GL_MODELVIEW));
        glCheck(glPopMatrix());
        glCheck(glMatrixMode(GL_TEXTURE));
        glCheck(glPopMatrix());
#ifndef SFML_OPENGL_ES
        glCheck(glPopClientAttrib());
        glCheck(glPopAttrib());
#endif
    }
}


////////////////////////////////////////////////////////////
void RenderTarget::resetGLStates()
{
    // Check here to make sure a context change does not happen after activate(true)
    const bool shaderAvailable       = Shader::isAvailable();
    const bool vertexBufferAvailable = VertexBuffer::isAvailable();

// Workaround for states not being properly reset on
// macOS unless a context switch really takes place
#if defined(SFML_SYSTEM_MACOS)
    if (!setActive(false))
    {
        err() << "Failed to set render target inactive" << std::endl;
    }
#endif

    if (RenderTargetImpl::isActive(m_id) || setActive(true))
    {
        // Make sure that extensions are initialized
        priv::ensureExtensionsInit();

        // Make sure that the texture unit which is active is the number 0
        if (GLEXT_multitexture)
        {
            glCheck(GLEXT_glClientActiveTexture(GLEXT_GL_TEXTURE0));
            glCheck(GLEXT_glActiveTexture(GLEXT_GL_TEXTURE0));
        }

        // Define the default OpenGL states
        glCheck(glDisable(GL_CULL_FACE));
        glCheck(glDisable(GL_LIGHTING));
        glCheck(glDisable(GL_STENCIL_TEST));
        glCheck(glDisable(GL_DEPTH_TEST));
        glCheck(glDisable(GL_ALPHA_TEST));
        glCheck(glDisable(GL_SCISSOR_TEST));
        glCheck(glEnable(GL_TEXTURE_2D));
        glCheck(glEnable(GL_BLEND));
        glCheck(glMatrixMode(GL_MODELVIEW));
        glCheck(glLoadIdentity());
        glCheck(glEnableClientState(GL_VERTEX_ARRAY));
        glCheck(glEnableClientState(GL_COLOR_ARRAY));
        glCheck(glEnableClientState(GL_TEXTURE_COORD_ARRAY));
        glCheck(glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE));
        m_cache.scissorEnabled = false;
        m_cache.stencilEnabled = false;
        m_cache.glStatesSet    = true;

        // Apply the default SFML states
        applyBlendMode(BlendAlpha);
        applyStencilMode(StencilMode());
        applyTexture(nullptr);
        if (shaderAvailable)
            applyShader(nullptr);

        if (vertexBufferAvailable)
            glCheck(VertexBuffer::bind(nullptr));

        m_cache.texCoordsArrayEnabled = true;

        m_cache.useVertexCache = false;

        // Set the default view
        setView(getView());

        m_cache.enable = true;
    }
}


////////////////////////////////////////////////////////////
void RenderTarget::initialize()
{
    // Setup the default and current views
    m_defaultView = View(FloatRect({0, 0}, Vector2f(getSize())));
    m_view        = m_defaultView;

    // Set GL states only on first draw, so that we don't pollute user's states
    m_cache.glStatesSet = false;

    // Generate a unique ID for this RenderTarget to track
    // whether it is active within a specific context
    m_id = RenderTargetImpl::getUniqueId();
}


////////////////////////////////////////////////////////////
void RenderTarget::applyCurrentView()
{
    // Set the viewport
    const IntRect viewport    = getViewport(m_view);
    const int     viewportTop = static_cast<int>(getSize().y) - (viewport.position.y + viewport.size.y);
    glCheck(glViewport(viewport.position.x, viewportTop, viewport.size.x, viewport.size.y));

    // Set the scissor rectangle and enable/disable scissor testing
    if (m_view.getScissor() == FloatRect({0, 0}, {1, 1}))
    {
        if (!m_cache.enable || m_cache.scissorEnabled)
        {
            glCheck(glDisable(GL_SCISSOR_TEST));
            m_cache.scissorEnabled = false;
        }
    }
    else
    {
        const IntRect pixelScissor = getScissor(m_view);
        const int     scissorTop   = static_cast<int>(getSize().y) - (pixelScissor.position.y + pixelScissor.size.y);
        glCheck(glScissor(pixelScissor.position.x, scissorTop, pixelScissor.size.x, pixelScissor.size.y));

        if (!m_cache.enable || !m_cache.scissorEnabled)
        {
            glCheck(glEnable(GL_SCISSOR_TEST));
            m_cache.scissorEnabled = true;
        }
    }

    // Set the projection matrix
    glCheck(glMatrixMode(GL_PROJECTION));
    glCheck(glLoadMatrixf(m_view.getTransform().getMatrix()));

    // Go back to model-view mode
    glCheck(glMatrixMode(GL_MODELVIEW));

    m_cache.viewChanged = false;
}


////////////////////////////////////////////////////////////
void RenderTarget::applyBlendMode(const BlendMode& mode)
{
    using RenderTargetImpl::equationToGlConstant;
    using RenderTargetImpl::factorToGlConstant;

    // Apply the blend mode, falling back to the non-separate versions if necessary
    if (GLEXT_blend_func_separate)
    {
        glCheck(GLEXT_glBlendFuncSeparate(factorToGlConstant(mode.colorSrcFactor),
                                          factorToGlConstant(mode.colorDstFactor),
                                          factorToGlConstant(mode.alphaSrcFactor),
                                          factorToGlConstant(mode.alphaDstFactor)));
    }
    else
    {
        glCheck(glBlendFunc(factorToGlConstant(mode.colorSrcFactor), factorToGlConstant(mode.colorDstFactor)));
    }

    if (GLEXT_blend_minmax || GLEXT_blend_subtract)
    {
        if (GLEXT_blend_equation_separate)
        {
            glCheck(GLEXT_glBlendEquationSeparate(equationToGlConstant(mode.colorEquation),
                                                  equationToGlConstant(mode.alphaEquation)));
        }
        else
        {
            glCheck(GLEXT_glBlendEquation(equationToGlConstant(mode.colorEquation)));
        }
    }
    else if ((mode.colorEquation != BlendMode::Equation::Add) || (mode.alphaEquation != BlendMode::Equation::Add))
    {
        static bool warned = false;

        if (!warned)
        {
#ifdef SFML_OPENGL_ES
            err() << "OpenGL ES extension OES_blend_subtract unavailable" << std::endl;
#else
            err() << "OpenGL extension EXT_blend_minmax and EXT_blend_subtract unavailable" << std::endl;
#endif
            err() << "Selecting a blend equation not possible" << '\n'
                  << "Ensure that hardware acceleration is enabled if available" << std::endl;

            warned = true;
        }
    }

    m_cache.lastBlendMode = mode;
}


////////////////////////////////////////////////////////////
void RenderTarget::applyStencilMode(const StencilMode& mode)
{
    using RenderTargetImpl::stencilFunctionToGlConstant;
    using RenderTargetImpl::stencilOperationToGlConstant;

    // Fast path if we have a default (disabled) stencil mode
    if (mode == StencilMode())
    {
        if (!m_cache.enable || m_cache.stencilEnabled)
        {
            glCheck(glDisable(GL_STENCIL_TEST));
            glCheck(glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE));

            m_cache.stencilEnabled = false;
        }
    }
    else
    {
        // Apply the stencil mode
        if (!m_cache.enable || !m_cache.stencilEnabled)
            glCheck(glEnable(GL_STENCIL_TEST));

        glCheck(glStencilOp(GL_KEEP,
                            stencilOperationToGlConstant(mode.stencilUpdateOperation),
                            stencilOperationToGlConstant(mode.stencilUpdateOperation)));
        glCheck(glStencilFunc(stencilFunctionToGlConstant(mode.stencilComparison),
                              static_cast<int>(mode.stencilReference.value),
                              mode.stencilMask.value));

        m_cache.stencilEnabled = true;
    }

    m_cache.lastStencilMode = mode;
}


////////////////////////////////////////////////////////////
void RenderTarget::applyTransform(const Transform& transform)
{
    // No need to call glMatrixMode(GL_MODELVIEW), it is always the
    // current mode (for optimization purpose, since it's the most used)
    if (transform == Transform::Identity)
        glCheck(glLoadIdentity());
    else
        glCheck(glLoadMatrixf(transform.getMatrix()));
}


////////////////////////////////////////////////////////////
void RenderTarget::applyTexture(const Texture* texture, CoordinateType coordinateType)
{
    Texture::bind(texture, coordinateType);

    m_cache.lastTextureId      = texture ? texture->m_cacheId : 0;
    m_cache.lastCoordinateType = coordinateType;
}


////////////////////////////////////////////////////////////
void RenderTarget::applyShader(const Shader* shader)
{
    Shader::bind(shader);
}


////////////////////////////////////////////////////////////
void RenderTarget::setupDraw(bool useVertexCache, const RenderStates& states)
{
    // GL_FRAMEBUFFER_SRGB is not available on OpenGL ES
    // If a framebuffer supports sRGB, it will always be enabled on OpenGL ES
#ifndef SFML_OPENGL_ES
    // Enable or disable sRGB encoding
    // This is needed for drivers that do not check the format of the surface drawn to before applying sRGB conversion
    if (!m_cache.enable)
    {
        if (isSrgb())
            glCheck(glEnable(GL_FRAMEBUFFER_SRGB));
        else
            glCheck(glDisable(GL_FRAMEBUFFER_SRGB));
    }
#endif

    // First set the persistent OpenGL states if it's the very first call
    if (!m_cache.glStatesSet)
        resetGLStates();

    if (useVertexCache)
    {
        // Since vertices are transformed, we must use an identity transform to render them
        if (!m_cache.enable || !m_cache.useVertexCache)
            glCheck(glLoadIdentity());
    }
    else
    {
        applyTransform(states.transform);
    }

    // Apply the view
    if (!m_cache.enable || m_cache.viewChanged)
        applyCurrentView();

    // Apply the blend mode
    if (!m_cache.enable || (states.blendMode != m_cache.lastBlendMode))
        applyBlendMode(states.blendMode);

    // Apply the stencil mode
    if (!m_cache.enable || (states.stencilMode != m_cache.lastStencilMode))
        applyStencilMode(states.stencilMode);

    // Mask the color buffer off if necessary
    if (states.stencilMode.stencilOnly)
        glCheck(glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE));

    // Apply the texture
    if (!m_cache.enable || (states.texture && states.texture->m_fboAttachment))
    {
        // If the texture is an FBO attachment, always rebind it
        // in order to inform the OpenGL driver that we want changes
        // made to it in other contexts to be visible here as well
        // This saves us from having to call glFlush() in
        // RenderTextureImplFBO which can be quite costly
        // See: https://www.khronos.org/opengl/wiki/Memory_Model
        applyTexture(states.texture, states.coordinateType);
    }
    else
    {
        const std::uint64_t textureId = states.texture ? states.texture->m_cacheId : 0;
        if (textureId != m_cache.lastTextureId || states.coordinateType != m_cache.lastCoordinateType)
            applyTexture(states.texture, states.coordinateType);
    }

    // Apply the shader
    if (states.shader)
        applyShader(states.shader);
}


////////////////////////////////////////////////////////////
void RenderTarget::drawPrimitives(PrimitiveType type, std::size_t firstVertex, std::size_t vertexCount)
{
    // Find the OpenGL primitive type
    static constexpr priv::EnumArray<PrimitiveType, GLenum, 6> modes =
        {GL_POINTS, GL_LINES, GL_LINE_STRIP, GL_TRIANGLES, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN};
    const GLenum mode = modes[type];

    // Draw the primitives
    glCheck(glDrawArrays(mode, static_cast<GLint>(firstVertex), static_cast<GLsizei>(vertexCount)));
}


////////////////////////////////////////////////////////////
void RenderTarget::cleanupDraw(const RenderStates& states)
{
    // Unbind the shader, if any
    if (states.shader)
        applyShader(nullptr);

    // If the texture we used to draw belonged to a RenderTexture, then forcibly unbind that texture.
    // This prevents a bug where some drivers do not clear RenderTextures properly.
    if (states.texture && states.texture->m_fboAttachment)
        applyTexture(nullptr);

    // Mask the color buffer back on if necessary
    if (states.stencilMode.stencilOnly)
        glCheck(glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE));

    // Re-enable the cache at the end of the draw if it was disabled
    m_cache.enable = true;
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
//   Since it overloads the == operator, we can easily check
//   whether any of the 6 blending components changed and,
//   thus, whether we need to update the blend mode.
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
