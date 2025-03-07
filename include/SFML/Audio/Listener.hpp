////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2025 Laurent Gomila (laurent@sfml-dev.org)
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

#pragma once

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Audio/Export.hpp>

#include <SFML/System/Angle.hpp>
#include <SFML/System/Vector3.hpp>


////////////////////////////////////////////////////////////
/// \brief The audio listener is the point in the scene
///        from where all the sounds are heard
///
////////////////////////////////////////////////////////////
namespace sf::Listener
{
////////////////////////////////////////////////////////////
/// \brief Structure defining the properties of a directional cone
///
/// Sounds will play at gain 1 when they are positioned
/// within the inner angle of the cone. Sounds will play
/// at `outerGain` when they are positioned outside the
/// outer angle of the cone. The gain declines linearly
/// from 1 to `outerGain` as the sound moves from the inner
/// angle to the outer angle.
///
////////////////////////////////////////////////////////////
struct Cone
{
    Angle innerAngle;  //!< Inner angle
    Angle outerAngle;  //!< Outer angle
    float outerGain{}; //!< Outer gain
};

////////////////////////////////////////////////////////////
/// \brief Change the global volume of all the sounds and musics
///
/// `volume` is a number between 0 and 100; it is combined
/// with the individual volume of each sound / music.
/// The default value for the volume is 100 (maximum).
///
/// \param volume New global volume, in the range [0, 100]
///
/// \see `getGlobalVolume`
///
////////////////////////////////////////////////////////////
SFML_AUDIO_API void setGlobalVolume(float volume);

////////////////////////////////////////////////////////////
/// \brief Get the current value of the global volume
///
/// \return Current global volume, in the range [0, 100]
///
/// \see `setGlobalVolume`
///
////////////////////////////////////////////////////////////
[[nodiscard]] SFML_AUDIO_API float getGlobalVolume();

////////////////////////////////////////////////////////////
/// \brief Set the position of the listener in the scene
///
/// The default listener's position is (0, 0, 0).
///
/// \param position New listener's position
///
/// \see `getPosition`, `setDirection`
///
////////////////////////////////////////////////////////////
SFML_AUDIO_API void setPosition(const Vector3f& position);

////////////////////////////////////////////////////////////
/// \brief Get the current position of the listener in the scene
///
/// \return Listener's position
///
/// \see `setPosition`
///
////////////////////////////////////////////////////////////
[[nodiscard]] SFML_AUDIO_API Vector3f getPosition();

////////////////////////////////////////////////////////////
/// \brief Set the forward vector of the listener in the scene
///
/// The direction (also called "at vector") is the vector
/// pointing forward from the listener's perspective. Together
/// with the up vector, it defines the 3D orientation of the
/// listener in the scene. The direction vector doesn't
/// have to be normalized.
/// The default listener's direction is (0, 0, -1).
///
/// \param direction New listener's direction
///
/// \see `getDirection`, `setUpVector`, `setPosition`
///
////////////////////////////////////////////////////////////
SFML_AUDIO_API void setDirection(const Vector3f& direction);

////////////////////////////////////////////////////////////
/// \brief Get the current forward vector of the listener in the scene
///
/// \return Listener's forward vector (not normalized)
///
/// \see `setDirection`
///
////////////////////////////////////////////////////////////
[[nodiscard]] SFML_AUDIO_API Vector3f getDirection();

////////////////////////////////////////////////////////////
/// \brief Set the velocity of the listener in the scene
///
/// The default listener's velocity is (0, 0, -1).
///
/// \param velocity New listener's velocity
///
/// \see `getVelocity`, `getDirection`, `setUpVector`, `setPosition`
///
////////////////////////////////////////////////////////////
SFML_AUDIO_API void setVelocity(const Vector3f& velocity);

////////////////////////////////////////////////////////////
/// \brief Get the current forward vector of the listener in the scene
///
/// \return Listener's velocity
///
/// \see `setVelocity`
///
////////////////////////////////////////////////////////////
[[nodiscard]] SFML_AUDIO_API Vector3f getVelocity();

////////////////////////////////////////////////////////////
/// \brief Set the cone properties of the listener in the audio scene
///
/// The cone defines how directional attenuation is applied.
/// The default cone of a sound is (2 * PI, 2 * PI, 1).
///
/// \param cone Cone properties of the listener in the scene
///
/// \see `getCone`
///
////////////////////////////////////////////////////////////
SFML_AUDIO_API void setCone(const Listener::Cone& cone);

////////////////////////////////////////////////////////////
/// \brief Get the cone properties of the listener in the audio scene
///
/// \return Cone properties of the listener
///
/// \see `setCone`
///
////////////////////////////////////////////////////////////
[[nodiscard]] SFML_AUDIO_API Listener::Cone getCone();

////////////////////////////////////////////////////////////
/// \brief Set the upward vector of the listener in the scene
///
/// The up vector is the vector that points upward from the
/// listener's perspective. Together with the direction, it
/// defines the 3D orientation of the listener in the scene.
/// The up vector doesn't have to be normalized.
/// The default listener's up vector is (0, 1, 0). It is usually
/// not necessary to change it, especially in 2D scenarios.
///
/// \param upVector New listener's up vector
///
/// \see `getUpVector`, `setDirection`, `setPosition`
///
////////////////////////////////////////////////////////////
SFML_AUDIO_API void setUpVector(const Vector3f& upVector);

////////////////////////////////////////////////////////////
/// \brief Get the current upward vector of the listener in the scene
///
/// \return Listener's upward vector (not normalized)
///
/// \see `setUpVector`
///
////////////////////////////////////////////////////////////
[[nodiscard]] SFML_AUDIO_API Vector3f getUpVector();
} // namespace sf::Listener


////////////////////////////////////////////////////////////
/// \namespace sf::Listener
/// \ingroup audio
///
/// The audio listener defines the global properties of the
/// audio environment, it defines where and how sounds and musics
/// are heard. If `sf::View` is the eyes of the user, then
/// `sf::Listener` are their ears (by the way, they are often linked
/// together -- same position, orientation, etc.).
///
/// `sf::Listener` is a simple interface, which allows to setup the
/// listener in the 3D audio environment (position, direction and
/// up vector), and to adjust the global volume.
///
/// Usage example:
/// \code
/// // Move the listener to the position (1, 0, -5)
/// sf::Listener::setPosition({1, 0, -5});
///
/// // Make it face the right axis (1, 0, 0)
/// sf::Listener::setDirection({1, 0, 0});
///
/// // Reduce the global volume
/// sf::Listener::setGlobalVolume(50);
/// \endcode
///
////////////////////////////////////////////////////////////
