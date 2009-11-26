////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2009 Laurent Gomila (laurent.gom@gmail.com)
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
void Listener::SetGlobalVolume(float volume)
{
    priv::EnsureALInit();

    ALCheck(alListenerf(AL_GAIN, volume * 0.01f));
}


////////////////////////////////////////////////////////////
float Listener::GetGlobalVolume()
{
    priv::EnsureALInit();

    float volume = 0.f;
    ALCheck(alGetListenerf(AL_GAIN, &volume));

    return volume;
}


////////////////////////////////////////////////////////////
void Listener::SetPosition(float x, float y, float z)
{
    priv::EnsureALInit();

    ALCheck(alListener3f(AL_POSITION, x, y, z));
}


////////////////////////////////////////////////////////////
void Listener::SetPosition(const Vector3f& position)
{
    SetPosition(position.x, position.y, position.z);
}


////////////////////////////////////////////////////////////
Vector3f Listener::GetPosition()
{
    priv::EnsureALInit();

    Vector3f position;
    ALCheck(alGetListener3f(AL_POSITION, &position.x, &position.y, &position.z));

    return position;
}


////////////////////////////////////////////////////////////
void Listener::SetDirection(float x, float y, float z)
{
    priv::EnsureALInit();

    float orientation[] = {x, y, z, 0.f, 1.f, 0.f};
    ALCheck(alListenerfv(AL_ORIENTATION, orientation));
}


////////////////////////////////////////////////////////////
void Listener::SetDirection(const Vector3f& direction)
{
    SetDirection(direction.x, direction.y, direction.z);
}


////////////////////////////////////////////////////////////
Vector3f Listener::GetDirection()
{
    priv::EnsureALInit();

    float orientation[6];
    ALCheck(alGetListenerfv(AL_ORIENTATION, orientation));

    return Vector3f(orientation[0], orientation[1], orientation[2]);
}

} // namespace sf
