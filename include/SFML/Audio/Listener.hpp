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

#ifndef SFML_LISTENER_HPP
#define SFML_LISTENER_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Audio/Export.hpp>
#include <SFML/System/Vector3.hpp>


namespace sf
{
////////////////////////////////////////////////////////////
/// \brief The audio listener is the point in the scene
///        from where all the sounds are heard
///
////////////////////////////////////////////////////////////
class SFML_AUDIO_API Listener
{
public :

    ////////////////////////////////////////////////////////////
    /// \brief Change the global volume of all the sounds and musics
    ///
    /// The volume is a number between 0 and 100; it is combined with
    /// the individual volume of each sound / music.
    /// The default value for the volume is 100 (maximum).
    ///
    /// \param volume New global volume, in the range [0, 100]
    ///
    /// \see getGlobalVolume
    ///
    ////////////////////////////////////////////////////////////
    static void setGlobalVolume(float volume);

    ////////////////////////////////////////////////////////////
    /// \brief Get the current value of the global volume
    ///
    /// \return Current global volume, in the range [0, 100]
    ///
    /// \see setGlobalVolume
    ///
    ////////////////////////////////////////////////////////////
    static float getGlobalVolume();

    ////////////////////////////////////////////////////////////
    /// \brief Set the position of the listener in the scene
    ///
    /// The default listener's position is (0, 0, 0).
    ///
    /// \param x X coordinate of the listener's position
    /// \param y Y coordinate of the listener's position
    /// \param z Z coordinate of the listener's position
    ///
    /// \see getPosition, setDirection
    ///
    ////////////////////////////////////////////////////////////
    static void setPosition(float x, float y, float z);

    ////////////////////////////////////////////////////////////
    /// \brief Set the position of the listener in the scene
    ///
    /// The default listener's position is (0, 0, 0).
    ///
    /// \param position New listener's position
    ///
    /// \see getPosition, setDirection
    ///
    ////////////////////////////////////////////////////////////
    static void setPosition(const Vector3f& position);

    ////////////////////////////////////////////////////////////
    /// \brief Get the current position of the listener in the scene
    ///
    /// \return Listener's position
    ///
    /// \see setPosition
    ///
    ////////////////////////////////////////////////////////////
    static Vector3f getPosition();

    ////////////////////////////////////////////////////////////
    /// \brief Set the orientation of the listener in the scene
    ///
    /// The orientation defines the 3D axes of the listener
    /// (left, up, front) in the scene. The orientation vector
    /// doesn't have to be normalized.
    /// The default listener's orientation is (0, 0, -1).
    ///
    /// \param x X coordinate of the listener's orientation
    /// \param y Y coordinate of the listener's orientation
    /// \param z Z coordinate of the listener's orientation
    ///
    /// \see getDirection, setPosition
    ///
    ////////////////////////////////////////////////////////////
    static void setDirection(float x, float y, float z);

    ////////////////////////////////////////////////////////////
    /// \brief Set the orientation of the listener in the scene
    ///
    /// The orientation defines the 3D axes of the listener
    /// (left, up, front) in the scene. The orientation vector
    /// doesn't have to be normalized.
    /// The default listener's orientation is (0, 0, -1).
    ///
    /// \param direction New listener's orientation
    ///
    /// \see getDirection, setPosition
    ///
    ////////////////////////////////////////////////////////////
    static void setDirection(const Vector3f& direction);

    ////////////////////////////////////////////////////////////
    /// \brief Get the current orientation of the listener in the scene
    ///
    /// \return Listener's orientation
    ///
    /// \see setDirection
    ///
    ////////////////////////////////////////////////////////////
    static Vector3f getDirection();
};

} // namespace sf


#endif // SFML_LISTENER_HPP


////////////////////////////////////////////////////////////
/// \class sf::Listener
/// \ingroup audio
///
/// The audio listener defines the global properties of the
/// audio environment, it defines where and how sounds and musics
/// are heard. If sf::View is the eyes of the user, then sf::Listener
/// is his ears (by the way, they are often linked together --
/// same position, orientation, etc.). 
///
/// sf::Listener is a simple interface, which allows to setup the
/// listener in the 3D audio environment (position and direction),
/// and to adjust the global volume.
///
/// Because the listener is unique in the scene, sf::Listener only
/// contains static functions and doesn't have to be instanciated.
///
/// Usage example:
/// \code
/// // Move the listener to the position (1, 0, -5)
/// sf::Listener::setPosition(1, 0, -5);
///
/// // Make it face the right axis (1, 0, 0)
/// sf::Listener::setDirection(1, 0, 0);
///
/// // Reduce the global volume
/// sf::Listener::setGlobalVolume(50);
/// \endcode
///
////////////////////////////////////////////////////////////
