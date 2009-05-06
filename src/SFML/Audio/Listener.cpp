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
#include <SFML/Audio/OpenAL.hpp>


namespace sf
{
////////////////////////////////////////////////////////////
/// Change the global volume of all the sounds
////////////////////////////////////////////////////////////
void Listener::SetGlobalVolume(float Volume)
{
    ALCheck(alListenerf(AL_GAIN, Volume * 0.01f));
}


////////////////////////////////////////////////////////////
/// Get the current value of the global volume of all the sounds
////////////////////////////////////////////////////////////
float Listener::GetGlobalVolume()
{
    float Volume = 0.f;
    ALCheck(alGetListenerf(AL_GAIN, &Volume));

    return Volume;
}


////////////////////////////////////////////////////////////
/// Change the position of the listener (take 3 values)
////////////////////////////////////////////////////////////
void Listener::SetPosition(float X, float Y, float Z)
{
    ALCheck(alListener3f(AL_POSITION, X, Y, Z));
}


////////////////////////////////////////////////////////////
/// Change the position of the listener (take a 3D vector)
////////////////////////////////////////////////////////////
void Listener::SetPosition(const Vector3f& Position)
{
    SetPosition(Position.x, Position.y, Position.z);
}


////////////////////////////////////////////////////////////
/// Get the current position of the listener
////////////////////////////////////////////////////////////
Vector3f Listener::GetPosition()
{
    Vector3f Position;
    ALCheck(alGetListener3f(AL_POSITION, &Position.x, &Position.y, &Position.z));

    return Position;
}


////////////////////////////////////////////////////////////
/// Change the orientation of the listener (the point
/// he must look at) (take 3 values)
////////////////////////////////////////////////////////////
void Listener::SetTarget(float X, float Y, float Z)
{
    float Orientation[] = {X, Y, Z, 0.f, 1.f, 0.f};
    ALCheck(alListenerfv(AL_ORIENTATION, Orientation));
}


////////////////////////////////////////////////////////////
/// Change the orientation of the listener (the point
/// he must look at) (take a 3D vector)
////////////////////////////////////////////////////////////
void Listener::SetTarget(const Vector3f& Target)
{
    SetTarget(Target.x, Target.y, Target.z);
}


////////////////////////////////////////////////////////////
/// Get the current orientation of the listener (the point
/// he's looking at)
////////////////////////////////////////////////////////////
Vector3f Listener::GetTarget()
{
    float Orientation[6];
    ALCheck(alGetListenerfv(AL_ORIENTATION, Orientation));

    return Vector3f(Orientation[0], Orientation[1], Orientation[2]);
}

} // namespace sf
