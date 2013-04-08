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
#include <SFML/Audio/Listener.hpp>
#include <SFML/Audio/ALCheck.hpp>


namespace sf
{
////////////////////////////////////////////////////////////
void Listener::setGlobalVolume(float volume)
{
    priv::ensureALInit();

    alCheck(alListenerf(AL_GAIN, volume * 0.01f));
}


////////////////////////////////////////////////////////////
float Listener::getGlobalVolume()
{
    priv::ensureALInit();

    float volume = 0.f;
    alCheck(alGetListenerf(AL_GAIN, &volume));

    return volume * 100;
}


////////////////////////////////////////////////////////////
void Listener::setPosition(float x, float y, float z)
{
    priv::ensureALInit();

    alCheck(alListener3f(AL_POSITION, x, y, z));
}


////////////////////////////////////////////////////////////
void Listener::setPosition(const Vector3f& position)
{
    setPosition(position.x, position.y, position.z);
}


////////////////////////////////////////////////////////////
Vector3f Listener::getPosition()
{
    priv::ensureALInit();

    Vector3f position;
    alCheck(alGetListener3f(AL_POSITION, &position.x, &position.y, &position.z));

    return position;
}


////////////////////////////////////////////////////////////
void Listener::setDirection(float x, float y, float z)
{
    priv::ensureALInit();

    float orientation[] = {x, y, z, 0.f, 1.f, 0.f};
    alCheck(alListenerfv(AL_ORIENTATION, orientation));
}


////////////////////////////////////////////////////////////
void Listener::setDirection(const Vector3f& direction)
{
    setDirection(direction.x, direction.y, direction.z);
}


////////////////////////////////////////////////////////////
Vector3f Listener::getDirection()
{
    priv::ensureALInit();

    float orientation[6];
    alCheck(alGetListenerfv(AL_ORIENTATION, orientation));

    return Vector3f(orientation[0], orientation[1], orientation[2]);
}

} // namespace sf
