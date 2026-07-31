////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2026 Laurent Gomila (laurent@sfml-dev.org)
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
#include <SFML/System/Exception.hpp>

#include <algorithm>
#include <array>
#include <memory>
#include <mutex>
#include <ostream>
#include <unordered_map>
#include <vector>

#include <cassert>
#include <cmath>
#include <cstddef>


namespace
{
// A nested named namespace is used here to allow unity builds of SFML.
namespace RenderTargetImpl
{
constexpr GLuint positionAttributeIndex = 0;
constexpr GLuint colorAttributeIndex    = 1;
constexpr GLuint texCoordAttributeIndex = 2;

constexpr std::array<float, 16> identityMatrix = {1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f};

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
struct RenderTarget::GLStatesStack
{
    struct VertexAttribute
    {
        GLint                  enabled{};
        GLint                  size{};
        GLint                  type{};
        GLint                  normalized{};
        GLint                  stride{};
        GLint                  buffer{};
        void*                  pointer{};
        std::array<GLfloat, 4> value{};
    };

    struct State
    {
        GLint                          program{};
        GLint                          activeTexture{};
        std::vector<GLint>             textureBindings;
        GLint                          arrayBuffer{};
        std::array<VertexAttribute, 3> attributes;
        std::array<GLint, 4>           viewport{};
        std::array<GLint, 4>           scissorBox{};
        GLboolean                      blendEnabled{};
        GLboolean                      stencilEnabled{};
        GLboolean                      scissorEnabled{};
        GLboolean                      cullEnabled{};
        GLboolean                      depthEnabled{};
        GLint                          blendSrcRgb{};
        GLint                          blendDstRgb{};
        GLint                          blendSrcAlpha{};
        GLint                          blendDstAlpha{};
        GLint                          blendEquationRgb{};
        GLint                          blendEquationAlpha{};
        GLint                          stencilFunc{};
        GLint                          stencilRef{};
        GLint                          stencilValueMask{};
        GLint                          stencilFail{};
        GLint                          stencilDepthFail{};
        GLint                          stencilDepthPass{};
        GLint                          stencilWriteMask{};
        GLint                          stencilBackFunc{};
        GLint                          stencilBackRef{};
        GLint                          stencilBackValueMask{};
        GLint                          stencilBackFail{};
        GLint                          stencilBackDepthFail{};
        GLint                          stencilBackDepthPass{};
        GLint                          stencilBackWriteMask{};
        std::array<GLboolean, 4>       colorMask{};
        std::array<GLfloat, 4>         clearColor{};
        GLint                          clearStencil{};
#ifndef SFML_OPENGL_ES
        bool      framebufferSrgbSupported{};
        GLboolean framebufferSrgbEnabled{};
#endif
    };

    std::vector<State> states;
};


////////////////////////////////////////////////////////////
RenderTarget::RenderTarget() = default;


////////////////////////////////////////////////////////////
RenderTarget::~RenderTarget() = default;


////////////////////////////////////////////////////////////
RenderTarget::RenderTarget(RenderTarget&&) noexcept = default;


////////////////////////////////////////////////////////////
RenderTarget& RenderTarget::operator=(RenderTarget&&) noexcept = default;


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
        setupDraw(states);

        glCheck(glBindBuffer(GL_ARRAY_BUFFER, 0));

        const auto* data = reinterpret_cast<const std::byte*>(vertices);
        glCheck(glVertexAttribPointer(RenderTargetImpl::positionAttributeIndex,
                                      2,
                                      GL_FLOAT,
                                      GL_FALSE,
                                      sizeof(Vertex),
                                      data + offsetof(Vertex, position)));
        glCheck(glVertexAttribPointer(RenderTargetImpl::colorAttributeIndex,
                                      4,
                                      GL_UNSIGNED_BYTE,
                                      GL_TRUE,
                                      sizeof(Vertex),
                                      data + offsetof(Vertex, color)));
        glCheck(glVertexAttribPointer(RenderTargetImpl::texCoordAttributeIndex,
                                      2,
                                      GL_FLOAT,
                                      GL_FALSE,
                                      sizeof(Vertex),
                                      data + offsetof(Vertex, texCoords)));

        drawPrimitives(type, 0, vertexCount);
        cleanupDraw(states);
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
        setupDraw(states);

        // Bind vertex buffer
        VertexBuffer::bind(&vertexBuffer);

        glCheck(glVertexAttribPointer(RenderTargetImpl::positionAttributeIndex,
                                      2,
                                      GL_FLOAT,
                                      GL_FALSE,
                                      sizeof(Vertex),
                                      reinterpret_cast<const void*>(offsetof(Vertex, position))));
        glCheck(glVertexAttribPointer(RenderTargetImpl::colorAttributeIndex,
                                      4,
                                      GL_UNSIGNED_BYTE,
                                      GL_TRUE,
                                      sizeof(Vertex),
                                      reinterpret_cast<const void*>(offsetof(Vertex, color))));
        glCheck(glVertexAttribPointer(RenderTargetImpl::texCoordAttributeIndex,
                                      2,
                                      GL_FLOAT,
                                      GL_FALSE,
                                      sizeof(Vertex),
                                      reinterpret_cast<const void*>(offsetof(Vertex, texCoords))));

        drawPrimitives(vertexBuffer.getPrimitiveType(), firstVertex, vertexCount);

        // Unbind vertex buffer
        VertexBuffer::bind(nullptr);

        cleanupDraw(states);
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

        if (!m_glStatesStack)
            m_glStatesStack = std::make_unique<GLStatesStack>();

        GLStatesStack::State state;

        glCheck(glGetIntegerv(GL_CURRENT_PROGRAM, &state.program));
        glCheck(glGetIntegerv(GL_ACTIVE_TEXTURE, &state.activeTexture));

        GLint textureUnitCount = 0;
        glCheck(glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &textureUnitCount));
        state.textureBindings.resize(static_cast<std::size_t>(std::max(textureUnitCount, 1)));
        for (std::size_t i = 0; i < state.textureBindings.size(); ++i)
        {
            glCheck(glActiveTexture(GL_TEXTURE0 + static_cast<GLenum>(i)));
            glCheck(glGetIntegerv(GL_TEXTURE_BINDING_2D, &state.textureBindings[i]));
        }
        glCheck(glActiveTexture(static_cast<GLenum>(state.activeTexture)));

        glCheck(glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &state.arrayBuffer));
        for (GLuint i = 0; i < state.attributes.size(); ++i)
        {
            auto& attribute = state.attributes[i];
            glCheck(glGetVertexAttribiv(i, GL_VERTEX_ATTRIB_ARRAY_ENABLED, &attribute.enabled));
            glCheck(glGetVertexAttribiv(i, GL_VERTEX_ATTRIB_ARRAY_SIZE, &attribute.size));
            glCheck(glGetVertexAttribiv(i, GL_VERTEX_ATTRIB_ARRAY_TYPE, &attribute.type));
            glCheck(glGetVertexAttribiv(i, GL_VERTEX_ATTRIB_ARRAY_NORMALIZED, &attribute.normalized));
            glCheck(glGetVertexAttribiv(i, GL_VERTEX_ATTRIB_ARRAY_STRIDE, &attribute.stride));
            glCheck(glGetVertexAttribiv(i, GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING, &attribute.buffer));
            glCheck(glGetVertexAttribPointerv(i, GL_VERTEX_ATTRIB_ARRAY_POINTER, &attribute.pointer));
            glCheck(glGetVertexAttribfv(i, GL_CURRENT_VERTEX_ATTRIB, attribute.value.data()));
        }

        glCheck(glGetIntegerv(GL_VIEWPORT, state.viewport.data()));
        glCheck(glGetIntegerv(GL_SCISSOR_BOX, state.scissorBox.data()));
        state.blendEnabled   = glIsEnabled(GL_BLEND);
        state.stencilEnabled = glIsEnabled(GL_STENCIL_TEST);
        state.scissorEnabled = glIsEnabled(GL_SCISSOR_TEST);
        state.cullEnabled    = glIsEnabled(GL_CULL_FACE);
        state.depthEnabled   = glIsEnabled(GL_DEPTH_TEST);

        glCheck(glGetIntegerv(GL_BLEND_SRC_RGB, &state.blendSrcRgb));
        glCheck(glGetIntegerv(GL_BLEND_DST_RGB, &state.blendDstRgb));
        glCheck(glGetIntegerv(GL_BLEND_SRC_ALPHA, &state.blendSrcAlpha));
        glCheck(glGetIntegerv(GL_BLEND_DST_ALPHA, &state.blendDstAlpha));
        glCheck(glGetIntegerv(GL_BLEND_EQUATION_RGB, &state.blendEquationRgb));
        glCheck(glGetIntegerv(GL_BLEND_EQUATION_ALPHA, &state.blendEquationAlpha));

        glCheck(glGetIntegerv(GL_STENCIL_FUNC, &state.stencilFunc));
        glCheck(glGetIntegerv(GL_STENCIL_REF, &state.stencilRef));
        glCheck(glGetIntegerv(GL_STENCIL_VALUE_MASK, &state.stencilValueMask));
        glCheck(glGetIntegerv(GL_STENCIL_FAIL, &state.stencilFail));
        glCheck(glGetIntegerv(GL_STENCIL_PASS_DEPTH_FAIL, &state.stencilDepthFail));
        glCheck(glGetIntegerv(GL_STENCIL_PASS_DEPTH_PASS, &state.stencilDepthPass));
        glCheck(glGetIntegerv(GL_STENCIL_WRITEMASK, &state.stencilWriteMask));
        glCheck(glGetIntegerv(GL_STENCIL_BACK_FUNC, &state.stencilBackFunc));
        glCheck(glGetIntegerv(GL_STENCIL_BACK_REF, &state.stencilBackRef));
        glCheck(glGetIntegerv(GL_STENCIL_BACK_VALUE_MASK, &state.stencilBackValueMask));
        glCheck(glGetIntegerv(GL_STENCIL_BACK_FAIL, &state.stencilBackFail));
        glCheck(glGetIntegerv(GL_STENCIL_BACK_PASS_DEPTH_FAIL, &state.stencilBackDepthFail));
        glCheck(glGetIntegerv(GL_STENCIL_BACK_PASS_DEPTH_PASS, &state.stencilBackDepthPass));
        glCheck(glGetIntegerv(GL_STENCIL_BACK_WRITEMASK, &state.stencilBackWriteMask));

        glCheck(glGetBooleanv(GL_COLOR_WRITEMASK, state.colorMask.data()));
        glCheck(glGetFloatv(GL_COLOR_CLEAR_VALUE, state.clearColor.data()));
        glCheck(glGetIntegerv(GL_STENCIL_CLEAR_VALUE, &state.clearStencil));

#ifndef SFML_OPENGL_ES
        state.framebufferSrgbSupported = GLEXT_framebuffer_sRGB;
        if (state.framebufferSrgbSupported)
            state.framebufferSrgbEnabled = glIsEnabled(GL_FRAMEBUFFER_SRGB);
#endif

        m_glStatesStack->states.push_back(std::move(state));
    }

    resetGLStates();
}


////////////////////////////////////////////////////////////
void RenderTarget::popGLStates()
{
    if (RenderTargetImpl::isActive(m_id) || setActive(true))
    {
        if (!m_glStatesStack || m_glStatesStack->states.empty())
        {
            err() << "Attempted to pop OpenGL states without a matching pushGLStates()" << std::endl;
            return;
        }

        GLStatesStack::State state = std::move(m_glStatesStack->states.back());
        m_glStatesStack->states.pop_back();

        glCheck(glUseProgram(static_cast<GLuint>(state.program)));

        for (std::size_t i = 0; i < state.textureBindings.size(); ++i)
        {
            glCheck(glActiveTexture(GL_TEXTURE0 + static_cast<GLenum>(i)));
            glCheck(glBindTexture(GL_TEXTURE_2D, static_cast<GLuint>(state.textureBindings[i])));
        }
        glCheck(glActiveTexture(static_cast<GLenum>(state.activeTexture)));

        for (GLuint i = 0; i < state.attributes.size(); ++i)
        {
            const auto& attribute = state.attributes[i];
            glCheck(glBindBuffer(GL_ARRAY_BUFFER, static_cast<GLuint>(attribute.buffer)));
            glCheck(glVertexAttribPointer(i,
                                          attribute.size,
                                          static_cast<GLenum>(attribute.type),
                                          static_cast<GLboolean>(attribute.normalized),
                                          attribute.stride,
                                          attribute.pointer));
            glCheck(glVertexAttrib4fv(i, attribute.value.data()));
            if (attribute.enabled)
                glCheck(glEnableVertexAttribArray(i));
            else
                glCheck(glDisableVertexAttribArray(i));
        }
        glCheck(glBindBuffer(GL_ARRAY_BUFFER, static_cast<GLuint>(state.arrayBuffer)));

        glCheck(glViewport(state.viewport[0], state.viewport[1], state.viewport[2], state.viewport[3]));
        glCheck(glScissor(state.scissorBox[0], state.scissorBox[1], state.scissorBox[2], state.scissorBox[3]));
        glCheck(glBlendFuncSeparate(static_cast<GLenum>(state.blendSrcRgb),
                                    static_cast<GLenum>(state.blendDstRgb),
                                    static_cast<GLenum>(state.blendSrcAlpha),
                                    static_cast<GLenum>(state.blendDstAlpha)));
        glCheck(glBlendEquationSeparate(static_cast<GLenum>(state.blendEquationRgb),
                                        static_cast<GLenum>(state.blendEquationAlpha)));

        glCheck(glStencilFuncSeparate(GL_FRONT,
                                      static_cast<GLenum>(state.stencilFunc),
                                      state.stencilRef,
                                      static_cast<GLuint>(state.stencilValueMask)));
        glCheck(glStencilOpSeparate(GL_FRONT,
                                    static_cast<GLenum>(state.stencilFail),
                                    static_cast<GLenum>(state.stencilDepthFail),
                                    static_cast<GLenum>(state.stencilDepthPass)));
        glCheck(glStencilMaskSeparate(GL_FRONT, static_cast<GLuint>(state.stencilWriteMask)));
        glCheck(glStencilFuncSeparate(GL_BACK,
                                      static_cast<GLenum>(state.stencilBackFunc),
                                      state.stencilBackRef,
                                      static_cast<GLuint>(state.stencilBackValueMask)));
        glCheck(glStencilOpSeparate(GL_BACK,
                                    static_cast<GLenum>(state.stencilBackFail),
                                    static_cast<GLenum>(state.stencilBackDepthFail),
                                    static_cast<GLenum>(state.stencilBackDepthPass)));
        glCheck(glStencilMaskSeparate(GL_BACK, static_cast<GLuint>(state.stencilBackWriteMask)));

        glCheck(glColorMask(state.colorMask[0], state.colorMask[1], state.colorMask[2], state.colorMask[3]));
        glCheck(glClearColor(state.clearColor[0], state.clearColor[1], state.clearColor[2], state.clearColor[3]));
        glCheck(glClearStencil(state.clearStencil));

        const auto restoreEnable = [](GLenum capability, GLboolean enabled)
        {
            if (enabled)
                glCheck(glEnable(capability));
            else
                glCheck(glDisable(capability));
        };

        restoreEnable(GL_BLEND, state.blendEnabled);
        restoreEnable(GL_STENCIL_TEST, state.stencilEnabled);
        restoreEnable(GL_SCISSOR_TEST, state.scissorEnabled);
        restoreEnable(GL_CULL_FACE, state.cullEnabled);
        restoreEnable(GL_DEPTH_TEST, state.depthEnabled);

#ifndef SFML_OPENGL_ES
        if (state.framebufferSrgbSupported)
            restoreEnable(GL_FRAMEBUFFER_SRGB, state.framebufferSrgbEnabled);
#endif

        m_cache.enable      = false;
        m_cache.glStatesSet = false;
    }
}


////////////////////////////////////////////////////////////
void RenderTarget::resetGLStates()
{
    // Check here to make sure a context change does not happen after activate(true)
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
        glCheck(glActiveTexture(GL_TEXTURE0));

        // Define the default OpenGL states
        glCheck(glDisable(GL_CULL_FACE));
        glCheck(glDisable(GL_STENCIL_TEST));
        glCheck(glDisable(GL_DEPTH_TEST));
        glCheck(glDisable(GL_SCISSOR_TEST));
        glCheck(glEnable(GL_BLEND));
#ifndef SFML_OPENGL_ES
        if (isSrgb())
            glCheck(glEnable(GL_FRAMEBUFFER_SRGB));
        else if (GLEXT_framebuffer_sRGB)
            glCheck(glDisable(GL_FRAMEBUFFER_SRGB));
#endif
        glCheck(glEnableVertexAttribArray(RenderTargetImpl::positionAttributeIndex));
        glCheck(glEnableVertexAttribArray(RenderTargetImpl::colorAttributeIndex));
        glCheck(glEnableVertexAttribArray(RenderTargetImpl::texCoordAttributeIndex));
        glCheck(glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE));
        m_cache.scissorEnabled = false;
        m_cache.stencilEnabled = false;
        m_cache.glStatesSet    = true;

        // Apply the default SFML states
        applyBlendMode(BlendAlpha);
        applyStencilMode(StencilMode());
        applyTexture(nullptr);

        if (vertexBufferAvailable)
            glCheck(VertexBuffer::bind(nullptr));

        // Set the default view
        setView(getView());

        if (m_defaultShader)
        {
            RenderStates states;
            applyShader(m_defaultShader.get(), states);
        }

        m_cache.enable = true;
    }
}


////////////////////////////////////////////////////////////
void RenderTarget::initialize()
{
    // Generate a unique ID for this RenderTarget to track
    // whether it is active within a specific context.
    m_id = RenderTargetImpl::getUniqueId();

    if (!setActive(true))
        throw Exception("Failed to activate render target while creating its default shader");

    auto defaultShader = std::make_unique<Shader>();
    if (!defaultShader->loadFromMemory(Shader::getDefaultVertexShaderSource(), Shader::getDefaultFragmentShaderSource()))
        throw Exception("Failed to create render target default shader");

    m_defaultShader = std::move(defaultShader);

    // Setup the default and current views
    m_defaultView = View(FloatRect({0, 0}, Vector2f(getSize())));
    m_view        = m_defaultView;

    // Set GL states only on first draw, so that we don't pollute user's states
    m_cache = {};
    m_glStatesStack.reset();
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
void RenderTarget::applyTexture(const Texture* texture, CoordinateType coordinateType)
{
    Texture::bind(texture);

    m_cache.lastTextureId      = texture ? texture->m_cacheId : 0;
    m_cache.lastCoordinateType = coordinateType;
}


////////////////////////////////////////////////////////////
void RenderTarget::applyShader(const Shader* shader, const RenderStates& states)
{
    const bool textureEnabled = states.texture && states.texture->m_texture;
    const auto textureMatrix  = textureEnabled ? states.texture->getTextureMatrix(states.coordinateType)
                                               : RenderTargetImpl::identityMatrix;

    shader->bindForDraw(states.transform, m_view.getTransform(), textureMatrix.data(), textureEnabled);
    m_cache.lastProgram       = shader->getNativeHandle();
    m_cache.lastModelView     = states.transform;
    m_cache.lastProjection    = m_view.getTransform();
    m_cache.lastTextureMatrix = textureMatrix;
}


////////////////////////////////////////////////////////////
void RenderTarget::setupDraw(const RenderStates& states)
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
        else if (GLEXT_framebuffer_sRGB)
            glCheck(glDisable(GL_FRAMEBUFFER_SRGB));
    }
#endif

    // First set the persistent OpenGL states if it's the very first call
    if (!m_cache.glStatesSet)
        resetGLStates();

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

    // Bind a valid custom shader, or the target's own default shader.
    const bool customShader = states.shader && states.shader->getNativeHandle();
    applyShader(customShader ? states.shader : m_defaultShader.get(), states);
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
    // Restore SFML's default program after a custom shader draw.
    if (states.shader && states.shader->getNativeHandle())
    {
        RenderStates defaultStates;
        applyShader(m_defaultShader.get(), defaultStates);
    }

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
//   If SetView was called since last draw, the viewport and
//   scissor are updated. The projection matrix is uploaded to
//   the active shader for every draw.
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
//   Shader matrix and texture uniforms are uploaded for every
//   draw because users can change custom shader state between
//   draw calls. SFML always restores the target's default shader
//   after drawing with a custom shader.
//
////////////////////////////////////////////////////////////
