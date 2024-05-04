////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2024 Laurent Gomila (laurent@sfml-dev.org)
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
#include <SFML/Audio/AudioDevice.hpp>
#include <SFML/Audio/Listener.hpp>


namespace sf
{
////////////////////////////////////////////////////////////
void Listener::setGlobalVolume(float volume)
{
    priv::AudioDevice::get().setGlobalVolume(volume);
}


////////////////////////////////////////////////////////////
float Listener::getGlobalVolume()
{
    return priv::AudioDevice::get().getGlobalVolume();
}


////////////////////////////////////////////////////////////
void Listener::setPosition(const Vector3f& position)
{
    priv::AudioDevice::get().setPosition(position);
}


////////////////////////////////////////////////////////////
Vector3f Listener::getPosition()
{
    return priv::AudioDevice::get().getPosition();
}


////////////////////////////////////////////////////////////
void Listener::setDirection(const Vector3f& direction)
{
    priv::AudioDevice::get().setDirection(direction);
}


////////////////////////////////////////////////////////////
Vector3f Listener::getDirection()
{
    return priv::AudioDevice::get().getDirection();
}


////////////////////////////////////////////////////////////
void Listener::setVelocity(const Vector3f& velocity)
{
    priv::AudioDevice::get().setVelocity(velocity);
}


////////////////////////////////////////////////////////////
Vector3f Listener::getVelocity()
{
    return priv::AudioDevice::get().getVelocity();
}


////////////////////////////////////////////////////////////
void Listener::setCone(const Listener::Cone& cone)
{
    priv::AudioDevice::get().setCone(cone);
}


////////////////////////////////////////////////////////////
Listener::Cone Listener::getCone()
{
    return priv::AudioDevice::get().getCone();
}


////////////////////////////////////////////////////////////
void Listener::setUpVector(const Vector3f& upVector)
{
    priv::AudioDevice::get().setUpVector(upVector);
}


////////////////////////////////////////////////////////////
Vector3f Listener::getUpVector()
{
    return priv::AudioDevice::get().getUpVector();
}

} // namespace sf
