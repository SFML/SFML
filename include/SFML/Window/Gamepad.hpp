////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2026 Laurent Gomila (laurent@sfml-dev.org)
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
#include <SFML/Window/Export.hpp>

#include <SFML/System/String.hpp>

#include <filesystem>
#include <string>
#include <string_view>

#include <cstddef>


////////////////////////////////////////////////////////////
/// \brief Give access to the real-time state of joysticks
///        that have a gamepad mapping
///
////////////////////////////////////////////////////////////
namespace sf::Gamepad
{
////////////////////////////////////////////////////////////
/// \brief Constants related to gamepad capabilities
///
////////////////////////////////////////////////////////////
// NOLINTBEGIN(readability-identifier-naming)
static constexpr unsigned int ButtonCount{21}; //!< Number of gamepad buttons
static constexpr unsigned int AxisCount{6};    //!< Number of gamepad axes
// NOLINTEND(readability-identifier-naming)

////////////////////////////////////////////////////////////
/// \brief Gamepad buttons
///
/// The face buttons are named after their position, not
/// their label, since the labels differ between vendors.
///
////////////////////////////////////////////////////////////
enum class Button
{
    South,         //!< Bottom face button (A on Xbox, Cross on PlayStation, B on Nintendo)
    East,          //!< Right face button (B on Xbox, Circle on PlayStation, A on Nintendo)
    West,          //!< Left face button (X on Xbox, Square on PlayStation, Y on Nintendo)
    North,         //!< Top face button (Y on Xbox, Triangle on PlayStation, X on Nintendo)
    Back,          //!< Back, View, Share, Select or Minus button
    Guide,         //!< Guide, Xbox, PS or Home button
    Start,         //!< Start, Menu, Options or Plus button
    LeftStick,     //!< Left stick pressed in
    RightStick,    //!< Right stick pressed in
    LeftShoulder,  //!< Left shoulder button
    RightShoulder, //!< Right shoulder button
    DPadUp,        //!< Up on the directional pad
    DPadDown,      //!< Down on the directional pad
    DPadLeft,      //!< Left on the directional pad
    DPadRight,     //!< Right on the directional pad
    Misc1,         //!< Additional button (Share on Xbox Series, Microphone on PS5, Capture on Switch)
    RightPaddle1,  //!< Upper or primary paddle under the right hand
    LeftPaddle1,   //!< Upper or primary paddle under the left hand
    RightPaddle2,  //!< Lower or secondary paddle under the right hand
    LeftPaddle2,   //!< Lower or secondary paddle under the left hand
    Touchpad       //!< Touchpad button (PS4, PS5)
};

////////////////////////////////////////////////////////////
/// \brief Gamepad axes
///
////////////////////////////////////////////////////////////
enum class Axis
{
    LeftX,       //!< Left stick, from -100 (left) to 100 (right)
    LeftY,       //!< Left stick, from -100 (up) to 100 (down)
    RightX,      //!< Right stick, from -100 (left) to 100 (right)
    RightY,      //!< Right stick, from -100 (up) to 100 (down)
    LeftTrigger, //!< Left trigger, from 0 (released) to 100 (fully pressed)
    RightTrigger //!< Right trigger, from 0 (released) to 100 (fully pressed)
};

////////////////////////////////////////////////////////////
/// \brief Check if a joystick is connected and has a gamepad mapping
///
/// \param joystick Index of the joystick to check
///
/// \return `true` if the joystick is connected and can be used as a gamepad, `false` otherwise
///
////////////////////////////////////////////////////////////
[[nodiscard]] SFML_WINDOW_API bool isConnected(unsigned int joystick);

////////////////////////////////////////////////////////////
/// \brief Check if a gamepad has a given button
///
/// If the joystick has no gamepad mapping, this function returns `false`.
///
/// \param joystick Index of the joystick
/// \param button   Button to check
///
/// \return `true` if the gamepad has the button, `false` otherwise
///
////////////////////////////////////////////////////////////
[[nodiscard]] SFML_WINDOW_API bool hasButton(unsigned int joystick, Button button);

////////////////////////////////////////////////////////////
/// \brief Check if a gamepad has a given axis
///
/// If the joystick has no gamepad mapping, this function returns `false`.
///
/// \param joystick Index of the joystick
/// \param axis     Axis to check
///
/// \return `true` if the gamepad has the axis, `false` otherwise
///
////////////////////////////////////////////////////////////
[[nodiscard]] SFML_WINDOW_API bool hasAxis(unsigned int joystick, Axis axis);

////////////////////////////////////////////////////////////
/// \brief Check if a gamepad button is pressed
///
/// If the joystick has no gamepad mapping, this function returns `false`.
///
/// \param joystick Index of the joystick
/// \param button   Button to check
///
/// \return `true` if the button is pressed, `false` otherwise
///
////////////////////////////////////////////////////////////
[[nodiscard]] SFML_WINDOW_API bool isButtonPressed(unsigned int joystick, Button button);

////////////////////////////////////////////////////////////
/// \brief Get the current position of a gamepad axis
///
/// If the joystick has no gamepad mapping, this function returns 0.
///
/// \param joystick Index of the joystick
/// \param axis     Axis to check
///
/// \return Current position of the axis, in range [-100 .. 100] for
///         the sticks and [0 .. 100] for the triggers
///
////////////////////////////////////////////////////////////
[[nodiscard]] SFML_WINDOW_API float getAxisPosition(unsigned int joystick, Axis axis);

////////////////////////////////////////////////////////////
/// \brief Get the name of a gamepad as given by its mapping
///
/// This is the name from the mapping database, which is often
/// more readable than the name the driver reports in
/// `sf::Joystick::getIdentification`.
///
/// \param joystick Index of the joystick
///
/// \return Name of the gamepad, empty if the joystick has no gamepad mapping
///
////////////////////////////////////////////////////////////
[[nodiscard]] SFML_WINDOW_API String getName(unsigned int joystick);

////////////////////////////////////////////////////////////
/// \brief Get the mapping used for a gamepad
///
/// \param joystick Index of the joystick
///
/// \return Mapping in the format of the SDL gamepad database,
///         empty if the joystick has no gamepad mapping
///
////////////////////////////////////////////////////////////
[[nodiscard]] SFML_WINDOW_API std::string getMapping(unsigned int joystick);

////////////////////////////////////////////////////////////
/// \brief Add gamepad mappings
///
/// The mappings use the format of the SDL gamepad database,
/// one mapping per line. Mappings for other platforms are
/// skipped. A mapping for a device that already has one
/// replaces the existing one. Connected joysticks pick up
/// the new mappings right away.
///
/// \param mappings One or more mappings, separated by line breaks
///
/// \return Number of mappings that were added or replaced
///
/// \see `addMappingsFromFile`
///
////////////////////////////////////////////////////////////
SFML_WINDOW_API std::size_t addMappings(std::string_view mappings);

////////////////////////////////////////////////////////////
/// \brief Add gamepad mappings from a file
///
/// This is typically the `gamecontrollerdb.txt` file of the
/// community-maintained SDL gamepad database.
///
/// \param filename Path of the file to load
///
/// \return Number of mappings that were added or replaced
///
/// \see `addMappings`
///
////////////////////////////////////////////////////////////
SFML_WINDOW_API std::size_t addMappingsFromFile(const std::filesystem::path& filename);
} // namespace sf::Gamepad


////////////////////////////////////////////////////////////
/// \namespace sf::Gamepad
/// \ingroup window
///
/// `sf::Gamepad` provides a fixed layout on top of `sf::Joystick`:
/// instead of numbered buttons and generic axes, which differ
/// for every device and platform, it reports positions such as
/// the bottom face button, the left stick or the right trigger.
///
/// Gamepads use the same index as `sf::Joystick`. A joystick
/// can be used as a gamepad once a mapping for it is known,
/// which `isConnected` tells. The raw state stays available
/// through `sf::Joystick` at all times.
///
/// The mappings use the format of the community-maintained
/// SDL gamepad database (https://github.com/mdqinc/SDL_GameControllerDB).
/// On Windows, Linux and macOS, SFML has a copy of that database
/// built in, unless it was built with `SFML_EMBED_GAMEPAD_DB`
/// turned off. Mappings in the `SDL_GAMECONTROLLERCONFIG`
/// environment variable are added on top, like SDL does.
/// Load a newer version of the database, or your own mappings,
/// with `addMappingsFromFile` or `addMappings`. They take
/// precedence over the built-in ones.
///
/// The states are updated together with the joysticks, see
/// `sf::Joystick::update`. The `GamepadButtonPressed`,
/// `GamepadButtonReleased`, `GamepadMoved`, `GamepadConnected`
/// and `GamepadDisconnected` events report the same changes
/// through a window.
///
/// Usage example:
/// \code
/// // Optional: use a newer version of the database than the built-in one
/// sf::Gamepad::addMappingsFromFile("gamecontrollerdb.txt");
///
/// // Can joystick #0 be used as a gamepad?
/// if (sf::Gamepad::isConnected(0))
/// {
///     // Is the bottom face button pressed?
///     bool jump = sf::Gamepad::isButtonPressed(0, sf::Gamepad::Button::South);
///
///     // How far is the right trigger pressed?
///     float throttle = sf::Gamepad::getAxisPosition(0, sf::Gamepad::Axis::RightTrigger);
/// }
/// \endcode
///
/// \see `sf::Joystick`
///
////////////////////////////////////////////////////////////
