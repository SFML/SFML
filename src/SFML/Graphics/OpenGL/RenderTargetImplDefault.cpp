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
#include <SFML/Graphics/OpenGL/RenderTargetImplDefault.hpp>
#include <SFML/Graphics/GLCheck.hpp>
#include <SFML/Graphics/GLExtensions.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/VertexBuffer.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

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
    switch (operation)
    {
        case sf::StencilUpdateOperation::Keep:      return GL_KEEP;
        case sf::StencilUpdateOperation::Zero:      return GL_ZERO;
        case sf::StencilUpdateOperation::Replace:   return GL_REPLACE;
        case sf::StencilUpdateOperation::Increment: return GL_INCR;
        case sf::StencilUpdateOperation::Decrement: return GL_DECR;
        case sf::StencilUpdateOperation::Invert:    return GL_INVERT;
    }

    sf::err() << "Invalid value for sf::StencilUpdateOperation! Fallback to sf::StencilMode::Keep." << std::endl;
    assert(false);
    return GL_KEEP;
}


// Convert a Comparison constant to the corresponding OpenGL constant.
std::uint32_t stencilFunctionToGlConstant(sf::StencilComparison comparison)
{
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

    sf::err() << "Invalid value for sf::StencilComparison! Fallback to sf::StencilMode::Always." << std::endl;
    assert(false);
    return GL_ALWAYS;
}
} // namespace


namespace sf::priv
{
////////////////////////////////////////////////////////////
RenderTargetImplDefault::RenderTargetImplDefault(RenderTarget& owner) :
m_owner(&owner),
m_id(getUniqueId())
{
}


////////////////////////////////////////////////////////////
void RenderTargetImplDefault::initialize()
{
    // Set GL states only on first draw, so that we don't pollute user's states
    m_cache.glStatesSet = false;
    m_cache.viewChanged = true;
}


////////////////////////////////////////////////////////////
void RenderTargetImplDefault::clear(Color color)
{
    if (isActive(m_id) || m_owner->setActive(true))
    {
        // Unbind texture to fix RenderTexture preventing clear
        applyTexture(nullptr, CoordinateType::Pixels);

        // Apply the view (scissor testing can affect clearing)
        if (!m_cache.enable || m_cache.viewChanged)
            applyCurrentView();

        glCheck(glClearColor(color.r / 255.f, color.g / 255.f, color.b / 255.f, color.a / 255.f));
        glCheck(glClear(GL_COLOR_BUFFER_BIT));
    }
}


////////////////////////////////////////////////////////////
void RenderTargetImplDefault::clearStencil(StencilValue stencilValue)
{
    if (isActive(m_id) || m_owner->setActive(true))
    {
        // Unbind texture to fix RenderTexture preventing clear
        applyTexture(nullptr, CoordinateType::Pixels);

        // Apply the view (scissor testing can affect clearing)
        if (!m_cache.enable || m_cache.viewChanged)
            applyCurrentView();

        glCheck(glClearStencil(static_cast<int>(stencilValue.value)));
        glCheck(glClear(GL_STENCIL_BUFFER_BIT));
    }
}


////////////////////////////////////////////////////////////
void RenderTargetImplDefault::clear(Color color, StencilValue stencilValue)
{
    if (isActive(m_id) || m_owner->setActive(true))
    {
        // Unbind texture to fix RenderTexture preventing clear
        applyTexture(nullptr, CoordinateType::Pixels);

        // Apply the view (scissor testing can affect clearing)
        if (!m_cache.enable || m_cache.viewChanged)
            applyCurrentView();

        glCheck(glClearColor(color.r / 255.f, color.g / 255.f, color.b / 255.f, color.a / 255.f));
        glCheck(glClearStencil(static_cast<int>(stencilValue.value)));
        glCheck(glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));
    }
}


////////////////////////////////////////////////////////////
bool RenderTargetImplDefault::setActive(bool active)
{
    // Mark this RenderTarget as active or no longer active in the tracking map
    const std::lock_guard lock(getMutex());

    const std::uint64_t contextId = Context::getActiveContextId();

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
void RenderTargetImplDefault::pushGLStates()
{
    if (isActive(m_id) || m_owner->setActive(true))
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
void RenderTargetImplDefault::popGLStates()
{
    if (isActive(m_id) || m_owner->setActive(true))
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
void RenderTargetImplDefault::resetGLStates()
{
    const bool shaderAvailable       = Shader::isAvailable();
    const bool vertexBufferAvailable = VertexBuffer::isAvailable();

#if defined(SFML_SYSTEM_MACOS)
    if (!setActive(false))
    {
        err() << "Failed to set render target inactive" << std::endl;
    }
#endif

    if (isActive(m_id) || m_owner->setActive(true))
    {
        ensureExtensionsInit();

        if (GLEXT_multitexture)
        {
            glCheck(GLEXT_glClientActiveTexture(GLEXT_GL_TEXTURE0));
            glCheck(GLEXT_glActiveTexture(GLEXT_GL_TEXTURE0));
        }

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

        applyBlendMode(BlendAlpha);
        applyStencilMode(StencilMode());
        applyTexture(nullptr, CoordinateType::Pixels);
        if (shaderAvailable)
            applyShader(nullptr);

        if (vertexBufferAvailable)
            glCheck(VertexBuffer::bind(nullptr));

        m_cache.texCoordsArrayEnabled = true;
        m_cache.useVertexCache = false;

        applyCurrentView();

        m_cache.enable = true;
    }
}


////////////////////////////////////////////////////////////
void RenderTargetImplDefault::invalidateView()
{
    m_cache.viewChanged = true;
}


////////////////////////////////////////////////////////////
void RenderTargetImplDefault::applyCurrentView()
{
    const View& view = m_owner->getView();
    const IntRect viewport = m_owner->getViewport(view);
    const int     viewportTop = static_cast<int>(m_owner->getSize().y) - (viewport.position.y + viewport.size.y);
    glCheck(glViewport(viewport.position.x, viewportTop, viewport.size.x, viewport.size.y));

    if (view.getScissor() == FloatRect({0, 0}, {1, 1}))
    {
        if (!m_cache.enable || m_cache.scissorEnabled)
        {
            glCheck(glDisable(GL_SCISSOR_TEST));
            m_cache.scissorEnabled = false;
        }
    }
    else
    {
        const IntRect pixelScissor = m_owner->getScissor(view);
        const int     scissorTop   = static_cast<int>(m_owner->getSize().y) - (pixelScissor.position.y + pixelScissor.size.y);
        glCheck(glScissor(pixelScissor.position.x, scissorTop, pixelScissor.size.x, pixelScissor.size.y));

        if (!m_cache.enable || !m_cache.scissorEnabled)
        {
            glCheck(glEnable(GL_SCISSOR_TEST));
            m_cache.scissorEnabled = true;
        }
    }

    glCheck(glMatrixMode(GL_PROJECTION));
    glCheck(glLoadMatrixf(view.getTransform().getMatrix()));
    glCheck(glMatrixMode(GL_MODELVIEW));

    m_cache.viewChanged = false;
}


////////////////////////////////////////////////////////////
void RenderTargetImplDefault::applyBlendMode(const BlendMode& mode)
{
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

    m_cache.lastBlendMode = mode;
}


////////////////////////////////////////////////////////////
void RenderTargetImplDefault::applyStencilMode(const StencilMode& mode)
{
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
void RenderTargetImplDefault::applyTransform(const Transform& transform)
{
    if (transform == Transform::Identity)
        glCheck(glLoadIdentity());
    else
        glCheck(glLoadMatrixf(transform.getMatrix()));
}


////////////////////////////////////////////////////////////
void RenderTargetImplDefault::applyTexture(const Texture* texture, CoordinateType coordinateType)
{
    Texture::bind(texture, coordinateType);

    m_cache.lastTextureId      = texture ? texture->getNativeHandle() : 0;

    m_cache.lastCoordinateType = coordinateType;
}


////////////////////////////////////////////////////////////
void RenderTargetImplDefault::applyShader(const Shader* shader)
{
    Shader::bind(shader);
}


////////////////////////////////////////////////////////////
void RenderTargetImplDefault::setupDraw(bool useVertexCache, const RenderStates& states)
{
    if (!isActive(m_id) && !m_owner->setActive(true))
        return;
#ifndef SFML_OPENGL_ES
    if (!m_cache.enable)
    {
        if (m_owner->isSrgb())
            glCheck(glEnable(GL_FRAMEBUFFER_SRGB));
        else if (GLEXT_framebuffer_sRGB)
            glCheck(glDisable(GL_FRAMEBUFFER_SRGB));
    }
#endif

    if (!m_cache.glStatesSet)
        resetGLStates();

    if (useVertexCache)
    {
        if (!m_cache.enable || !m_cache.useVertexCache)
            glCheck(glLoadIdentity());
    }
    else
    {
        applyTransform(states.transform);
    }

    if (!m_cache.enable || m_cache.viewChanged)
        applyCurrentView();

    if (!m_cache.enable || (states.blendMode != m_cache.lastBlendMode))
        applyBlendMode(states.blendMode);

    if (!m_cache.enable || (states.stencilMode != m_cache.lastStencilMode))
        applyStencilMode(states.stencilMode);

    if (states.stencilMode.stencilOnly)
        glCheck(glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE));

    // Handle texture
    const std::uint64_t textureId = states.texture ? states.texture->getNativeHandle() : 0;
    if (!m_cache.enable || textureId != m_cache.lastTextureId || states.coordinateType != m_cache.lastCoordinateType)
    {
        applyTexture(states.texture, states.coordinateType);
    }

    if (states.shader)
        applyShader(states.shader);

    m_cache.useVertexCache = useVertexCache;
}


////////////////////////////////////////////////////////////
void RenderTargetImplDefault::drawVertices(const Vertex* vertices,
                                           std::size_t vertexCount,
                                           PrimitiveType type,
                                           const RenderStates& states)
{
    if (!vertices || vertexCount == 0)
        return;

    if (!isActive(m_id) && !m_owner->setActive(true))
        return;

    const bool useVertexCache = (vertexCount <= m_cache.vertexCache.size());
    const Vertex* drawVertices = vertices;

    if (useVertexCache)
    {
        for (std::size_t i = 0; i < vertexCount; ++i)
        {
            m_cache.vertexCache[i].position = states.transform.transformPoint(vertices[i].position);
            m_cache.vertexCache[i].color    = vertices[i].color;
            m_cache.vertexCache[i].texCoords = vertices[i].texCoords;
        }
        drawVertices = m_cache.vertexCache.data();
    }

    setupDraw(useVertexCache, states);

    glCheck(glVertexPointer(2, GL_FLOAT, sizeof(Vertex), &drawVertices[0].position));
    glCheck(glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(Vertex), &drawVertices[0].color));
    glCheck(glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), &drawVertices[0].texCoords));

    if (states.texture)
    {
        if (!m_cache.enable || !m_cache.texCoordsArrayEnabled)
        {
            glCheck(glEnableClientState(GL_TEXTURE_COORD_ARRAY));
            m_cache.texCoordsArrayEnabled = true;
        }
    }
    else
    {
        if (!m_cache.enable || m_cache.texCoordsArrayEnabled)
        {
            glCheck(glDisableClientState(GL_TEXTURE_COORD_ARRAY));
            m_cache.texCoordsArrayEnabled = false;
        }
    }

    drawPrimitives(type, 0, vertexCount);
    cleanupDraw(states);
}


////////////////////////////////////////////////////////////
void RenderTargetImplDefault::drawPrimitives(PrimitiveType type, std::size_t firstVertex, std::size_t vertexCount)
{
    static constexpr EnumArray<PrimitiveType, GLenum, 6> modes =
        {GL_POINTS, GL_LINES, GL_LINE_STRIP, GL_TRIANGLES, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN};
    const GLenum mode = modes[type];

    glCheck(glDrawArrays(mode, static_cast<GLint>(firstVertex), static_cast<GLsizei>(vertexCount)));
}


////////////////////////////////////////////////////////////
void RenderTargetImplDefault::cleanupDraw(const RenderStates& states)
{
    if (states.shader)
        applyShader(nullptr);

    if (states.stencilMode.stencilOnly)
        glCheck(glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE));

    m_cache.enable = true;
}

////////////////////////////////////////////////////////////
void RenderTargetImplDefault::setOwner(RenderTarget& newOwner)
{
    m_owner = &newOwner;
}

////////////////////////////////////////////////////////////
void RenderTargetImplDefault::setupVertexBufferDraw(const RenderStates& states)
{
    // When using a VBO, the vertex data is already in GPU memory
    // We need to set up the vertex attribute pointers with offsets instead of addresses
    // The VBO must be bound before calling this function

    // Set up vertex pointer (offset 0 from the start of each Vertex struct)
    glCheck(glVertexPointer(2, GL_FLOAT, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, position))));

    // Set up color pointer (offset of color member in Vertex struct)
    glCheck(glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, color))));

    // Set up texture coordinate pointer (offset of texCoords member in Vertex struct)
    glCheck(glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, texCoords))));

    // Enable or disable texture coordinate array based on whether a texture is used
    if (states.texture)
    {
        if (!m_cache.enable || !m_cache.texCoordsArrayEnabled)
        {
            glCheck(glEnableClientState(GL_TEXTURE_COORD_ARRAY));
            m_cache.texCoordsArrayEnabled = true;
        }
    }
    else
    {
        if (!m_cache.enable || m_cache.texCoordsArrayEnabled)
        {
            glCheck(glDisableClientState(GL_TEXTURE_COORD_ARRAY));
            m_cache.texCoordsArrayEnabled = false;
        }
    }
}

} // namespace sf::priv
