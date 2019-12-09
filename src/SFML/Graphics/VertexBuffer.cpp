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

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics/VertexBuffer.hpp>
#include <SFML/Graphics/OpenGL/GL1/VertexBufferImplDefault.hpp>
#include <SFML/Graphics/Renderer.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Err.hpp>


namespace sf
{
////////////////////////////////////////////////////////////
VertexBuffer::VertexBuffer() :
m_primitiveType(Points)
{
    if ((sf::getRenderer() == sf::Renderer::Default) || (sf::getRenderer() == sf::Renderer::OpenGL1))
        m_impl = new priv::VertexBufferImplDefault;
}


////////////////////////////////////////////////////////////
VertexBuffer::VertexBuffer(PrimitiveType type) :
m_primitiveType(type)
{
    if ((sf::getRenderer() == sf::Renderer::Default) || (sf::getRenderer() == sf::Renderer::OpenGL1))
        m_impl = new priv::VertexBufferImplDefault;
}


////////////////////////////////////////////////////////////
VertexBuffer::VertexBuffer(VertexBuffer::Usage usage) :
m_primitiveType(Points)
{
    if ((sf::getRenderer() == sf::Renderer::Default) || (sf::getRenderer() == sf::Renderer::OpenGL1))
        m_impl = new priv::VertexBufferImplDefault(usage);
}


////////////////////////////////////////////////////////////
VertexBuffer::VertexBuffer(PrimitiveType type, VertexBuffer::Usage usage) :
m_primitiveType(type)
{
    if ((sf::getRenderer() == sf::Renderer::Default) || (sf::getRenderer() == sf::Renderer::OpenGL1))
        m_impl = new priv::VertexBufferImplDefault(usage);
}


////////////////////////////////////////////////////////////
VertexBuffer::VertexBuffer(const VertexBuffer& copy) :
m_primitiveType(copy.m_primitiveType)
{
    if ((sf::getRenderer() == sf::Renderer::Default) || (sf::getRenderer() == sf::Renderer::OpenGL1))
        m_impl = new priv::VertexBufferImplDefault(copy.m_impl->getUsage());

    if (copy.getNativeHandle() && copy.getVertexCount())
    {
        if (!create(copy.getVertexCount()))
        {
            err() << "Could not create vertex buffer for copying" << std::endl;
            return;
        }

        if (!update(copy))
            err() << "Could not copy vertex buffer" << std::endl;
    }
}


////////////////////////////////////////////////////////////
VertexBuffer::~VertexBuffer()
{
    delete m_impl;
}


////////////////////////////////////////////////////////////
bool VertexBuffer::create(std::size_t vertexCount)
{
    return m_impl->create(vertexCount);
}


////////////////////////////////////////////////////////////
std::size_t VertexBuffer::getVertexCount() const
{
    return m_impl->getVertexCount();
}


////////////////////////////////////////////////////////////
bool VertexBuffer::update(const Vertex* vertices)
{
    return update(vertices, m_impl->getVertexCount(), 0);
}


////////////////////////////////////////////////////////////
bool VertexBuffer::update(const Vertex* vertices, std::size_t vertexCount, unsigned int offset)
{
    return m_impl->update(vertices, vertexCount, offset);
}


////////////////////////////////////////////////////////////
bool VertexBuffer::update(const VertexBuffer& vertexBuffer)
{
    return m_impl->update(*vertexBuffer.m_impl);
}


////////////////////////////////////////////////////////////
VertexBuffer& VertexBuffer::operator =(const VertexBuffer& right)
{
    VertexBuffer temp(right);

    swap(temp);

    return *this;
}


////////////////////////////////////////////////////////////
void VertexBuffer::swap(VertexBuffer& right)
{
    std::swap(m_impl,          right.m_impl);
    std::swap(m_primitiveType, right.m_primitiveType);
}


////////////////////////////////////////////////////////////
unsigned int VertexBuffer::getNativeHandle() const
{
    return m_impl->getNativeHandle();
}


////////////////////////////////////////////////////////////
void VertexBuffer::bind(const VertexBuffer* vertexBuffer)
{
    if ((sf::getRenderer() == sf::Renderer::Default) || (sf::getRenderer() == sf::Renderer::OpenGL1))
        priv::VertexBufferImplDefault::bind(vertexBuffer ? static_cast<const priv::VertexBufferImplDefault*>(vertexBuffer->m_impl) : 0);
}


////////////////////////////////////////////////////////////
void VertexBuffer::setPrimitiveType(PrimitiveType type)
{
    m_primitiveType = type;
}


////////////////////////////////////////////////////////////
PrimitiveType VertexBuffer::getPrimitiveType() const
{
    return m_primitiveType;
}


////////////////////////////////////////////////////////////
void VertexBuffer::setUsage(VertexBuffer::Usage usage)
{
    m_impl->setUsage(usage);
}


////////////////////////////////////////////////////////////
VertexBuffer::Usage VertexBuffer::getUsage() const
{
    return m_impl->getUsage();
}


////////////////////////////////////////////////////////////
bool VertexBuffer::isAvailable()
{
    if ((sf::getRenderer() == sf::Renderer::Default) || (sf::getRenderer() == sf::Renderer::OpenGL1))
        return priv::VertexBufferImplDefault::isAvailable();

    return false;
}


////////////////////////////////////////////////////////////
void VertexBuffer::draw(RenderTarget& target, RenderStates states) const
{
    if (getNativeHandle() && getVertexCount())
        target.draw(*this, 0, getVertexCount(), states);
}

} // namespace sf
