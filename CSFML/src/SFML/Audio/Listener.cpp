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
#include <SFML/Audio/Listener.h>
#include <SFML/Audio/Listener.hpp>



////////////////////////////////////////////////////////////
/// Change the global volume of all the sounds
////////////////////////////////////////////////////////////
void sfListener_SetGlobalVolume(float volume)
{
    sf::Listener::SetGlobalVolume(volume);
}


////////////////////////////////////////////////////////////
/// Get the current value of the global volume of all the sounds
////////////////////////////////////////////////////////////
float sfListener_GetGlobalVolume()
{
    return sf::Listener::GetGlobalVolume();
}


////////////////////////////////////////////////////////////
/// Change the position of the listener
////////////////////////////////////////////////////////////
void sfListener_SetPosition(float x, float y, float PosZ)
{
    sf::Listener::SetPosition(sf::Vector3f(x, y, PosZ));
}


////////////////////////////////////////////////////////////
/// Get the current position of the listener
////////////////////////////////////////////////////////////
void sfListener_GetPosition(float* x, float* y, float* z)
{
    if (x && y && z)
    {
        sf::Vector3f position = sf::Listener::GetPosition();
        *x = position.x;
        *y = position.y;
        *z = position.z;
    }
}


////////////////////////////////////////////////////////////
/// Change the orientation of the listener (the point
/// he must look at)
////////////////////////////////////////////////////////////
void sfListener_SetTarget(float x, float y, float z)
{
    sf::Listener::SetTarget(sf::Vector3f(x, y, z));
}


////////////////////////////////////////////////////////////
/// Get the current orientation of the listener (the point
/// he's looking at)
////////////////////////////////////////////////////////////
void sfListener_GetTarget(float* x, float* y, float* z)
{
    if (x && y && z)
    {
        sf::Vector3f target = sf::Listener::GetTarget();
        *x = target.x;
        *y = target.y;
        *z = target.z;
    }
}
