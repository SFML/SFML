////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2023 Laurent Gomila (laurent@sfml-dev.org)
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
#include <SFML/Graphics/Transformable.hpp>
#include "SFML/Graphics/Shape.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Text.hpp"
#include <cmath>


namespace sf
{
////////////////////////////////////////////////////////////
Transformable::Transformable() :
m_origin                    (0, 0),
m_position                  (0, 0),
m_rotation                  (0),
m_scale                     (1, 1),
m_transform                 (),
m_transformNeedUpdate       (true),
m_inverseTransform          (),
m_inverseTransformNeedUpdate(true)
{
}


////////////////////////////////////////////////////////////
Transformable::~Transformable()
{
}


////////////////////////////////////////////////////////////
void Transformable::setPosition(float x, float y)
{
    m_position.x = x;
    m_position.y = y;
    m_transformNeedUpdate = true;
    m_inverseTransformNeedUpdate = true;
}


////////////////////////////////////////////////////////////
void Transformable::setPosition(const Vector2f& position)
{
    setPosition(position.x, position.y);
}


////////////////////////////////////////////////////////////
void sf::Transformable::setPosition(Location origin, Location refOrigin, sf::Transformable& transformableRef) {
    // Get the current positions of both transformables
    sf::Vector2f position = this->getPosition();
    sf::Vector2f refPosition = transformableRef.getPosition();

    // Placeholder for sizes. Since sf::Transformable doesn't provide size, we assume zero size here.
    sf::Vector2f thisSize(0.f, 0.f);
    sf::Vector2f refSize(0.f, 0.f);

    // Check if the current object is of a type that provides bounds (like sf::Sprite, sf::Text, etc.)
    if (const sf::Sprite* sprite = dynamic_cast<const sf::Sprite*>(this)) {
        thisSize = sf::Vector2f(sprite->getGlobalBounds().width, sprite->getGlobalBounds().height);
    } else if (const sf::Text* text = dynamic_cast<const sf::Text*>(this)) {
        thisSize = sf::Vector2f(text->getGlobalBounds().width, text->getGlobalBounds().height);
    } else if (const sf::Shape* shape = dynamic_cast<const sf::Shape*>(this)) {
        thisSize = sf::Vector2f(shape->getGlobalBounds().width, shape->getGlobalBounds().height);
    }

    // Do the same for the reference transformable
    if (const sf::Sprite* sprite = dynamic_cast<const sf::Sprite*>(&transformableRef)) {
        refSize = sf::Vector2f(sprite->getGlobalBounds().width, sprite->getGlobalBounds().height);
    } else if (const sf::Text* text = dynamic_cast<const sf::Text*>(&transformableRef)) {
        refSize = sf::Vector2f(text->getGlobalBounds().width, text->getGlobalBounds().height);
    } else if (const sf::Shape* shape = dynamic_cast<const sf::Shape*>(&transformableRef)) {
        refSize = sf::Vector2f(shape->getGlobalBounds().width, shape->getGlobalBounds().height);
    }

    // These will hold the offsets for the desired locations
    sf::Vector2f offsetOrigin;
    sf::Vector2f offsetRefOrigin;

    // Determine the position for the origin based on the `origin` parameter (this transformable)
    switch (origin) {
        case TopLeft:      offsetOrigin = sf::Vector2f(0.f, 0.f); break;
        case TopMid:       offsetOrigin = sf::Vector2f(thisSize.x / 2.f, 0.f); break;
        case TopRight:     offsetOrigin = sf::Vector2f(thisSize.x, 0.f); break;
        case MidLeft:      offsetOrigin = sf::Vector2f(0.f, thisSize.y / 2.f); break;
        case MidMid:       offsetOrigin = sf::Vector2f(thisSize.x / 2.f, thisSize.y / 2.f); break;
        case MidRight:     offsetOrigin = sf::Vector2f(thisSize.x, thisSize.y / 2.f); break;
        case BottomLeft:   offsetOrigin = sf::Vector2f(0.f, thisSize.y); break;
        case BottomMid:    offsetOrigin = sf::Vector2f(thisSize.x / 2.f, thisSize.y); break;
        case BottomRight:  offsetOrigin = sf::Vector2f(thisSize.x, thisSize.y); break;
    }

    // Determine the position for the reference origin based on the `refOrigin` parameter (reference transformable)
    switch (refOrigin) {
        case TopLeft:      offsetRefOrigin = sf::Vector2f(0.f, 0.f); break;
        case TopMid:       offsetRefOrigin = sf::Vector2f(refSize.x / 2.f, 0.f); break;
        case TopRight:     offsetRefOrigin = sf::Vector2f(refSize.x, 0.f); break;
        case MidLeft:      offsetRefOrigin = sf::Vector2f(0.f, refSize.y / 2.f); break;
        case MidMid:       offsetRefOrigin = sf::Vector2f(refSize.x / 2.f, refSize.y / 2.f); break;
        case MidRight:     offsetRefOrigin = sf::Vector2f(refSize.x, refSize.y / 2.f); break;
        case BottomLeft:   offsetRefOrigin = sf::Vector2f(0.f, refSize.y); break;
        case BottomMid:    offsetRefOrigin = sf::Vector2f(refSize.x / 2.f, refSize.y); break;
        case BottomRight:  offsetRefOrigin = sf::Vector2f(refSize.x, refSize.y); break;
    }

    // Set the new position of the calling transformable based on the offset positions
    this->setPosition(refPosition + offsetRefOrigin - offsetOrigin);
}


////////////////////////////////////////////////////////////
void sf::Transformable::setPosition(Location origin, Location refOrigin, const sf::RenderWindow& windowRef) {
    // Get the current position of this transformable
    sf::Vector2f position = this->getPosition();

    // Placeholder for the size of the transformable (since sf::Transformable doesn't provide size)
    sf::Vector2f thisSize(0.f, 0.f);

    // Check if the current object is of a type that provides bounds (like sf::Sprite, sf::Text, etc.)
    if (const sf::Sprite* sprite = dynamic_cast<const sf::Sprite*>(this)) {
        thisSize = sf::Vector2f(sprite->getGlobalBounds().width, sprite->getGlobalBounds().height);
    } else if (const sf::Text* text = dynamic_cast<const sf::Text*>(this)) {
        thisSize = sf::Vector2f(text->getGlobalBounds().width, text->getGlobalBounds().height);
    } else if (const sf::Shape* shape = dynamic_cast<const sf::Shape*>(this)) {
        thisSize = sf::Vector2f(shape->getGlobalBounds().width, shape->getGlobalBounds().height);
    }

    // Get the size of the window
    sf::Vector2u windowSize = windowRef.getSize();
    sf::Vector2f refSize(static_cast<float>(windowSize.x), static_cast<float>(windowSize.y));

    // These will hold the offsets for the desired locations
    sf::Vector2f offsetOrigin;
    sf::Vector2f offsetRefOrigin;

    // Determine the position for the origin based on the `origin` parameter (this transformable)
    switch (origin) {
        case TopLeft:      offsetOrigin = sf::Vector2f(0.f, 0.f); break;
        case TopMid:       offsetOrigin = sf::Vector2f(thisSize.x / 2.f, 0.f); break;
        case TopRight:     offsetOrigin = sf::Vector2f(thisSize.x, 0.f); break;
        case MidLeft:      offsetOrigin = sf::Vector2f(0.f, thisSize.y / 2.f); break;
        case MidMid:       offsetOrigin = sf::Vector2f(thisSize.x / 2.f, thisSize.y / 2.f); break;
        case MidRight:     offsetOrigin = sf::Vector2f(thisSize.x, thisSize.y / 2.f); break;
        case BottomLeft:   offsetOrigin = sf::Vector2f(0.f, thisSize.y); break;
        case BottomMid:    offsetOrigin = sf::Vector2f(thisSize.x / 2.f, thisSize.y); break;
        case BottomRight:  offsetOrigin = sf::Vector2f(thisSize.x, thisSize.y); break;
    }

    // Determine the position for the reference origin based on the `refOrigin` parameter (the window)
    switch (refOrigin) {
        case TopLeft:      offsetRefOrigin = sf::Vector2f(0.f, 0.f); break;
        case TopMid:       offsetRefOrigin = sf::Vector2f(refSize.x / 2.f, 0.f); break;
        case TopRight:     offsetRefOrigin = sf::Vector2f(refSize.x, 0.f); break;
        case MidLeft:      offsetRefOrigin = sf::Vector2f(0.f, refSize.y / 2.f); break;
        case MidMid:       offsetRefOrigin = sf::Vector2f(refSize.x / 2.f, refSize.y / 2.f); break;
        case MidRight:     offsetRefOrigin = sf::Vector2f(refSize.x, refSize.y / 2.f); break;
        case BottomLeft:   offsetRefOrigin = sf::Vector2f(0.f, refSize.y); break;
        case BottomMid:    offsetRefOrigin = sf::Vector2f(refSize.x / 2.f, refSize.y); break;
        case BottomRight:  offsetRefOrigin = sf::Vector2f(refSize.x, refSize.y); break;
    }

    // Set the new position of the calling transformable based on the offset positions
    this->setPosition(offsetRefOrigin - offsetOrigin);
}


////////////////////////////////////////////////////////////
void Transformable::setRotation(float angle)
{
    m_rotation = std::fmod(angle, 360.f);
    if (m_rotation < 0)
        m_rotation += 360.f;

    m_transformNeedUpdate = true;
    m_inverseTransformNeedUpdate = true;
}


////////////////////////////////////////////////////////////
void Transformable::setScale(float factorX, float factorY)
{
    m_scale.x = factorX;
    m_scale.y = factorY;
    m_transformNeedUpdate = true;
    m_inverseTransformNeedUpdate = true;
}


////////////////////////////////////////////////////////////
void Transformable::setScale(const Vector2f& factors)
{
    setScale(factors.x, factors.y);
}


////////////////////////////////////////////////////////////
void Transformable::setOrigin(float x, float y)
{
    m_origin.x = x;
    m_origin.y = y;
    m_transformNeedUpdate = true;
    m_inverseTransformNeedUpdate = true;
}


////////////////////////////////////////////////////////////
void Transformable::setOrigin(const Vector2f& origin)
{
    setOrigin(origin.x, origin.y);
}


////////////////////////////////////////////////////////////
const Vector2f& Transformable::getPosition() const
{
    return m_position;
}


////////////////////////////////////////////////////////////
float Transformable::getRotation() const
{
    return m_rotation;
}


////////////////////////////////////////////////////////////
const Vector2f& Transformable::getScale() const
{
    return m_scale;
}


////////////////////////////////////////////////////////////
const Vector2f& Transformable::getOrigin() const
{
    return m_origin;
}


////////////////////////////////////////////////////////////
void Transformable::move(float offsetX, float offsetY)
{
    setPosition(m_position.x + offsetX, m_position.y + offsetY);
}


////////////////////////////////////////////////////////////
void Transformable::move(const Vector2f& offset)
{
    setPosition(m_position.x + offset.x, m_position.y + offset.y);
}


////////////////////////////////////////////////////////////
void Transformable::rotate(float angle)
{
    setRotation(m_rotation + angle);
}


////////////////////////////////////////////////////////////
void Transformable::scale(float factorX, float factorY)
{
    setScale(m_scale.x * factorX, m_scale.y * factorY);
}


////////////////////////////////////////////////////////////
void Transformable::scale(const Vector2f& factor)
{
    setScale(m_scale.x * factor.x, m_scale.y * factor.y);
}


////////////////////////////////////////////////////////////
const Transform& Transformable::getTransform() const
{
    // Recompute the combined transform if needed
    if (m_transformNeedUpdate)
    {
        float angle  = -m_rotation * 3.141592654f / 180.f;
        float cosine = std::cos(angle);
        float sine   = std::sin(angle);
        float sxc    = m_scale.x * cosine;
        float syc    = m_scale.y * cosine;
        float sxs    = m_scale.x * sine;
        float sys    = m_scale.y * sine;
        float tx     = -m_origin.x * sxc - m_origin.y * sys + m_position.x;
        float ty     =  m_origin.x * sxs - m_origin.y * syc + m_position.y;

        m_transform = Transform( sxc, sys, tx,
                                -sxs, syc, ty,
                                 0.f, 0.f, 1.f);
        m_transformNeedUpdate = false;
    }

    return m_transform;
}


////////////////////////////////////////////////////////////
const Transform& Transformable::getInverseTransform() const
{
    // Recompute the inverse transform if needed
    if (m_inverseTransformNeedUpdate)
    {
        m_inverseTransform = getTransform().getInverse();
        m_inverseTransformNeedUpdate = false;
    }

    return m_inverseTransform;
}

} // namespace sf
