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
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/VertexBuffer.hpp>
#include <SFML/Graphics/OpenGL/RenderTargetImplDefault.hpp>

#include <SFML/System/Err.hpp>

#include <algorithm>
#include <cmath>
#include <cstddef>


namespace sf
{
////////////////////////////////////////////////////////////
RenderTarget::RenderTarget() :
m_impl(std::make_unique<priv::RenderTargetImplDefault>(*this))
{
}


////////////////////////////////////////////////////////////
RenderTarget::~RenderTarget() = default;


////////////////////////////////////////////////////////////
RenderTarget::RenderTarget(RenderTarget&&) noexcept = default;


////////////////////////////////////////////////////////////
RenderTarget& RenderTarget::operator=(RenderTarget&&) noexcept = default;


////////////////////////////////////////////////////////////
void RenderTarget::clear(Color color)
{
    m_impl->clear(color);
}


////////////////////////////////////////////////////////////
void RenderTarget::clearStencil(StencilValue stencilValue)
{
    m_impl->clearStencil(stencilValue);
}


////////////////////////////////////////////////////////////
void RenderTarget::clear(Color color, StencilValue stencilValue)
{
    m_impl->clear(color, stencilValue);
}


////////////////////////////////////////////////////////////
void RenderTarget::setView(const View& view)
{
    m_view = view;
    m_impl->invalidateView();
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
    if (!vertices || vertexCount == 0)
        return;

    m_impl->drawVertices(vertices, vertexCount, type, states);
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

    m_impl->setupDraw(false, states);

    // Bind vertex buffer
    VertexBuffer::bind(&vertexBuffer);

    m_impl->drawPrimitives(vertexBuffer.getPrimitiveType(), firstVertex, vertexCount);

    // Unbind vertex buffer
    VertexBuffer::bind(nullptr);

    m_impl->cleanupDraw(states);
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
    return m_impl->setActive(active);
}


////////////////////////////////////////////////////////////
void RenderTarget::pushGLStates()
{
    m_impl->pushGLStates();
}


////////////////////////////////////////////////////////////
void RenderTarget::popGLStates()
{
    m_impl->popGLStates();
}


////////////////////////////////////////////////////////////
void RenderTarget::resetGLStates()
{
    m_impl->resetGLStates();
}


////////////////////////////////////////////////////////////
void RenderTarget::initialize()
{
    // Setup the default and current views
    m_defaultView = View(FloatRect({0, 0}, Vector2f(getSize())));
    m_view        = m_defaultView;

    m_impl->initialize();
}


////////////////////////////////////////////////////////////
void RenderTarget::applyCurrentView()
{
    m_impl->applyCurrentView();
}


////////////////////////////////////////////////////////////
void RenderTarget::applyBlendMode(const BlendMode& mode)
{
    m_impl->applyBlendMode(mode);
}


////////////////////////////////////////////////////////////
void RenderTarget::applyStencilMode(const StencilMode& mode)
{
    m_impl->applyStencilMode(mode);
}


////////////////////////////////////////////////////////////
void RenderTarget::applyTransform(const Transform& transform)
{
    m_impl->applyTransform(transform);
}


////////////////////////////////////////////////////////////
void RenderTarget::applyTexture(const Texture* texture, CoordinateType coordinateType)
{
    m_impl->applyTexture(texture, coordinateType);
}


////////////////////////////////////////////////////////////
void RenderTarget::applyShader(const Shader* shader)
{
    m_impl->applyShader(shader);
}


////////////////////////////////////////////////////////////
void RenderTarget::setupDraw(bool useVertexCache, const RenderStates& states)
{
    m_impl->setupDraw(useVertexCache, states);
}


////////////////////////////////////////////////////////////
void RenderTarget::drawPrimitives(PrimitiveType type, std::size_t firstVertex, std::size_t vertexCount)
{
    m_impl->drawPrimitives(type, firstVertex, vertexCount);
}


////////////////////////////////////////////////////////////
void RenderTarget::cleanupDraw(const RenderStates& states)
{
    m_impl->cleanupDraw(states);
}

} // namespace sf
