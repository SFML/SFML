////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2014 Laurent Gomila (laurent.gom@gmail.com)
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
#include <SFML/Audio/Listener.hpp>
#include <SFML/Audio/ALCheck.hpp>


namespace
{
    float        listenerVolume = 100.f;
    sf::Vector3f listenerPosition (0.f, 0.f, 0.f);
    sf::Vector3f listenerDirection(0.f, 0.f, -1.f);
    sf::Vector3f listenerUpVector (0.f, 1.f, 0.f);
}


namespace sf
{
////////////////////////////////////////////////////////////
void Listener::setGlobalVolume(float volume)
{
	if (volume != listenerVolume)
    {
        priv::ensureALInit();

        alCheck(alListenerf(AL_GAIN, volume * 0.01f));
        listenerVolume = volume;
    }
}


////////////////////////////////////////////////////////////
float Listener::getGlobalVolume()
{
    return listenerVolume;
}


////////////////////////////////////////////////////////////
void Listener::setPosition(float x, float y, float z)
{
    setPosition(Vector3f(x, y, z));
}


////////////////////////////////////////////////////////////
void Listener::setPosition(const Vector3f& position)
{
    if (position != listenerPosition)
    {
        priv::ensureALInit();

        alCheck(alListener3f(AL_POSITION, position.x, position.y, position.z));
        listenerPosition = position;
    }
}


////////////////////////////////////////////////////////////
Vector3f Listener::getPosition()
{
    return listenerPosition;
}


////////////////////////////////////////////////////////////
void Listener::setDirection(float x, float y, float z)
{
    setDirection(Vector3f(x, y, z));
}


////////////////////////////////////////////////////////////
void Listener::setDirection(const Vector3f& direction)
{
    if (direction != listenerDirection)
    {
        priv::ensureALInit();

        float orientation[] = {direction.x, direction.y, direction.z, listenerUpVector.x, listenerUpVector.y, listenerUpVector.z};
        alCheck(alListenerfv(AL_ORIENTATION, orientation));
        listenerDirection = direction;
    }
}


////////////////////////////////////////////////////////////
Vector3f Listener::getDirection()
{
    return listenerDirection;
}


////////////////////////////////////////////////////////////
void Listener::setUpVector(float x, float y, float z)
{
    setUpVector(Vector3f(x, y, z));
}


////////////////////////////////////////////////////////////
void Listener::setUpVector(const Vector3f& upVector)
{
    if (upVector != listenerUpVector)
    {
        priv::ensureALInit();

        float orientation[] = {listenerDirection.x, listenerDirection.y, listenerDirection.z, upVector.x, upVector.y, upVector.z};
        alCheck(alListenerfv(AL_ORIENTATION, orientation));
        listenerUpVector = upVector;
    }
}


////////////////////////////////////////////////////////////
Vector3f Listener::getUpVector()
{
    return listenerUpVector;
}

} // namespace sf
