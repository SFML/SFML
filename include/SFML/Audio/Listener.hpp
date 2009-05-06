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

#ifndef SFML_LISTENER_HPP
#define SFML_LISTENER_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Config.hpp>
#include <SFML/System/Vector3.hpp>


namespace sf
{
////////////////////////////////////////////////////////////
/// Listener is a global interface for defining the audio
/// listener properties ; the audio listener is the point in
/// the scene from where all the sounds are heard
////////////////////////////////////////////////////////////
class SFML_API Listener
{
public :

    ////////////////////////////////////////////////////////////
    /// Change the global volume of all the sounds.
    /// The default volume is 100
    ///
    /// \param Volume : New global volume, in the range [0, 100]
    ///
    ////////////////////////////////////////////////////////////
    static void SetGlobalVolume(float Volume);

    ////////////////////////////////////////////////////////////
    /// Get the current value of the global volume of all the sounds
    ///
    /// \return Current global volume, in the range [0, 100]
    ///
    ////////////////////////////////////////////////////////////
    static float GetGlobalVolume();

    ////////////////////////////////////////////////////////////
    /// Change the position of the listener (take 3 values).
    /// The default position is (0, 0, 0)
    ///
    /// \param X, Y, Z : Position of the listener in the world
    ///
    ////////////////////////////////////////////////////////////
    static void SetPosition(float X, float Y, float Z);

    ////////////////////////////////////////////////////////////
    /// Change the position of the listener (take a 3D vector).
    /// The default position is (0, 0, 0)
    ///
    /// \param Position : Position of the listener in the world
    ///
    ////////////////////////////////////////////////////////////
    static void SetPosition(const Vector3f& Position);

    ////////////////////////////////////////////////////////////
    /// Get the current position of the listener
    ///
    /// \return Position of the listener in the world
    ///
    ////////////////////////////////////////////////////////////
    static Vector3f GetPosition();

    ////////////////////////////////////////////////////////////
    /// Change the orientation of the listener (the point
    /// he must look at) (take 3 values).
    /// The default target is (0, 0, -1)
    ///
    /// \param X, Y, Z : Position of the point the listener must look at
    ///
    ////////////////////////////////////////////////////////////
    static void SetTarget(float X, float Y, float Z);

    ////////////////////////////////////////////////////////////
    /// Change the orientation of the listener (the point
    /// he must look at) (take a 3D vector).
    /// The default target is (0, 0, -1)
    ///
    /// \param Target : Position of the point the listener must look at
    ///
    ////////////////////////////////////////////////////////////
    static void SetTarget(const Vector3f& Target);

    ////////////////////////////////////////////////////////////
    /// Get the current orientation of the listener (the point
    /// he's looking at)
    ///
    /// \return : Position of the point the listener is looking at
    ///
    ////////////////////////////////////////////////////////////
    static Vector3f GetTarget();
};

} // namespace sf


#endif // SFML_LISTENER_HPP
