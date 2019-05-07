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
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/OpenGL/GL1/TextureImplDefault.hpp>
#include <SFML/Graphics/Renderer.hpp>
#include <SFML/Graphics/Image.hpp>


namespace sf
{
////////////////////////////////////////////////////////////
Texture::Texture() :
m_impl(NULL)
{
    if ((sf::getRenderer() == sf::Renderer::Default) || (sf::getRenderer() == sf::Renderer::OpenGL1))
        m_impl = new priv::TextureImplDefault;
}


////////////////////////////////////////////////////////////
Texture::Texture(const Texture& copy) :
m_impl(NULL)
{
    if ((sf::getRenderer() == sf::Renderer::Default) || (sf::getRenderer() == sf::Renderer::OpenGL1))
        m_impl = new priv::TextureImplDefault(*static_cast<const priv::TextureImplDefault*>(copy.m_impl));
}


////////////////////////////////////////////////////////////
Texture::~Texture()
{
    delete m_impl;
}


////////////////////////////////////////////////////////////
bool Texture::create(unsigned int width, unsigned int height)
{
    return m_impl->create(width, height);
}


////////////////////////////////////////////////////////////
bool Texture::loadFromFile(const std::string& filename, const IntRect& area)
{
    Image image;
    return image.loadFromFile(filename) && loadFromImage(image, area);
}


////////////////////////////////////////////////////////////
bool Texture::loadFromMemory(const void* data, std::size_t size, const IntRect& area)
{
    Image image;
    return image.loadFromMemory(data, size) && loadFromImage(image, area);
}


////////////////////////////////////////////////////////////
bool Texture::loadFromStream(InputStream& stream, const IntRect& area)
{
    Image image;
    return image.loadFromStream(stream) && loadFromImage(image, area);
}


////////////////////////////////////////////////////////////
bool Texture::loadFromImage(const Image& image, const IntRect& area)
{
    return m_impl->loadFromImage(image, area);
}


////////////////////////////////////////////////////////////
Vector2u Texture::getSize() const
{
    return m_impl->getSize();
}


////////////////////////////////////////////////////////////
Image Texture::copyToImage() const
{
    return m_impl->copyToImage();
}


////////////////////////////////////////////////////////////
void Texture::update(const Uint8* pixels)
{
    // Update the whole texture
    update(pixels, m_impl->getSize().x, m_impl->getSize().y, 0, 0);
}


////////////////////////////////////////////////////////////
void Texture::update(const Uint8* pixels, unsigned int width, unsigned int height, unsigned int x, unsigned int y)
{
    m_impl->update(pixels, width, height, x, y);
}


////////////////////////////////////////////////////////////
void Texture::update(const Texture& texture)
{
    // Update the whole texture
    update(texture, 0, 0);
}


////////////////////////////////////////////////////////////
void Texture::update(const Texture& texture, unsigned int x, unsigned int y)
{
    m_impl->update(*texture.m_impl, x, y);
}


////////////////////////////////////////////////////////////
void Texture::update(const Image& image)
{
    // Update the whole texture
    update(image.getPixelsPtr(), image.getSize().x, image.getSize().y, 0, 0);
}


////////////////////////////////////////////////////////////
void Texture::update(const Image& image, unsigned int x, unsigned int y)
{
    update(image.getPixelsPtr(), image.getSize().x, image.getSize().y, x, y);
}


////////////////////////////////////////////////////////////
void Texture::update(const Window& window)
{
    update(window, 0, 0);
}


////////////////////////////////////////////////////////////
void Texture::update(const Window& window, unsigned int x, unsigned int y)
{
    m_impl->update(window, x, y);
}


////////////////////////////////////////////////////////////
void Texture::setSmooth(bool smooth)
{
    m_impl->setSmooth(smooth);
}


////////////////////////////////////////////////////////////
bool Texture::isSmooth() const
{
    return m_impl->isSmooth();
}


////////////////////////////////////////////////////////////
void Texture::setSrgb(bool sRgb)
{
    m_impl->setSrgb(sRgb);
}


////////////////////////////////////////////////////////////
bool Texture::isSrgb() const
{
    return m_impl->isSrgb();
}


////////////////////////////////////////////////////////////
void Texture::setRepeated(bool repeated)
{
    m_impl->setRepeated(repeated);
}


////////////////////////////////////////////////////////////
bool Texture::isRepeated() const
{
    return m_impl->isRepeated();
}


////////////////////////////////////////////////////////////
bool Texture::generateMipmap()
{
    return m_impl->generateMipmap();
}


////////////////////////////////////////////////////////////
void Texture::bind(const Texture* texture, CoordinateType coordinateType)
{
    if ((sf::getRenderer() == sf::Renderer::Default) || (sf::getRenderer() == sf::Renderer::OpenGL1))
        priv::TextureImplDefault::bind(texture ? static_cast<const priv::TextureImplDefault*>(texture->m_impl) : 0, coordinateType);
}


////////////////////////////////////////////////////////////
unsigned int Texture::getMaximumSize()
{
    return priv::TextureImplDefault::getMaximumSize();
}


////////////////////////////////////////////////////////////
Texture& Texture::operator =(const Texture& right)
{
    Texture temp(right);

    swap(temp);

    return *this;
}


////////////////////////////////////////////////////////////
void Texture::swap(Texture& right)
{
    std::swap(m_impl, right.m_impl);
}


////////////////////////////////////////////////////////////
unsigned int Texture::getNativeHandle() const
{
    return 0;
}

} // namespace sf
