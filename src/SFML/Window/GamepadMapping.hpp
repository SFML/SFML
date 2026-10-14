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
#include <SFML/Window/Gamepad.hpp>
#include <SFML/Window/Joystick.hpp>
#include <SFML/Window/JoystickImpl.hpp>

#include <SFML/System/EnumArray.hpp>

#include <array>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include <cstddef>
#include <cstdint>


namespace sf::priv
{
////////////////////////////////////////////////////////////
/// \brief Structure holding a gamepad's state
///
////////////////////////////////////////////////////////////
struct GamepadState
{
    bool connected{}; //!< Is the joystick connected and does it have a gamepad mapping?
    EnumArray<Gamepad::Button, bool, Gamepad::ButtonCount> buttons{}; //!< Status of each button (true = pressed)
    EnumArray<Gamepad::Axis, float, Gamepad::AxisCount> axes{}; //!< Position of each axis, in range [-100, 100] or [0, 100] for triggers
};


////////////////////////////////////////////////////////////
/// \brief Input element of a joystick that a gamepad button or axis is bound to
///
////////////////////////////////////////////////////////////
struct GamepadInput
{
    enum class Type
    {
        Button,
        Axis,
        Hat
    };

    Type           type{};            //!< Kind of element
    unsigned int   index{};           //!< Button, axis or hat index (SDL's index until resolved)
    Joystick::Axis axis{};            //!< SFML axis of an axis input, once resolved
    unsigned int   hatMask{};         //!< Hat directions of a hat input (1 up, 2 right, 4 down, 8 left)
    bool           hatUpIsNegative{}; //!< Is up on the hat reported as a negative PovY?
    float          min{-1.f};         //!< Value of an axis input mapped to the start of the output range
    float          max{1.f};          //!< Value of an axis input mapped to the end of the output range
};


////////////////////////////////////////////////////////////
/// \brief Binding of a gamepad button or axis to a joystick element
///
////////////////////////////////////////////////////////////
struct GamepadBinding
{
    GamepadInput    input;     //!< Joystick element
    bool            isAxis{};  //!< Is the output an axis (otherwise it's a button)?
    Gamepad::Button button{};  //!< Output button
    Gamepad::Axis   axis{};    //!< Output axis
    float           min{-1.f}; //!< Start of the output axis range
    float           max{1.f};  //!< End of the output axis range
};


////////////////////////////////////////////////////////////
/// \brief Bindings of a mapping resolved for a connected joystick
///
////////////////////////////////////////////////////////////
class GamepadBindings
{
public:
    ////////////////////////////////////////////////////////////
    /// \brief Default constructor, without any binding
    ///
    ////////////////////////////////////////////////////////////
    GamepadBindings() = default;

    ////////////////////////////////////////////////////////////
    /// \brief Construct from bindings that refer to SFML's elements
    ///
    ////////////////////////////////////////////////////////////
    explicit GamepadBindings(std::vector<GamepadBinding> bindings);

    ////////////////////////////////////////////////////////////
    /// \brief Compute the gamepad state from a joystick state
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] GamepadState evaluate(const JoystickState& state) const;

    ////////////////////////////////////////////////////////////
    /// \brief Check if a button is bound
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] bool hasButton(Gamepad::Button button) const;

    ////////////////////////////////////////////////////////////
    /// \brief Check if an axis is bound
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] bool hasAxis(Gamepad::Axis axis) const;

private:
    std::vector<GamepadBinding> m_bindings; //!< Bindings referring to SFML's elements
};


////////////////////////////////////////////////////////////
/// \brief Gamepad mapping in the format of the SDL gamepad database
///
////////////////////////////////////////////////////////////
class GamepadMapping
{
public:
    ////////////////////////////////////////////////////////////
    /// \brief Parse a mapping
    ///
    /// \param line Mapping in the format "GUID,name,binding,binding,...,platform:Name,"
    ///
    /// \return Mapping, or `std::nullopt` if the line isn't a mapping
    ///         SFML can use (comment, invalid GUID, SDL-internal driver, ...)
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] static std::optional<GamepadMapping> parse(std::string_view line);

    ////////////////////////////////////////////////////////////
    /// \brief Get the name of the gamepad given by the mapping
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] const std::string& getName() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the platform of the mapping, empty if it applies to all
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] const std::string& getPlatform() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the mapping as it was parsed
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] const std::string& getString() const;

    ////////////////////////////////////////////////////////////
    /// \brief Check if two mappings are for the same device
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] bool isForSameDevice(const GamepadMapping& other) const;

    ////////////////////////////////////////////////////////////
    /// \brief Rate how well the mapping fits a joystick
    ///
    /// \return 0 if the mapping isn't for this joystick, higher is better
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] int getMatchScore(const Joystick::Identification& identification, const JoystickLayout& layout) const;

    ////////////////////////////////////////////////////////////
    /// \brief Translate the SDL indices of the bindings to SFML's elements
    ///
    /// Bindings to elements SFML doesn't read are dropped.
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] GamepadBindings resolve(const JoystickCaps& capabilities, const JoystickLayout& layout) const;

private:
    std::array<std::uint8_t, 16> m_guid{};     //!< Raw GUID
    std::uint16_t                m_bus{};      //!< Bus type
    std::uint16_t                m_vendor{};   //!< Vendor ID, 0 for name-based GUIDs
    std::uint16_t                m_product{};  //!< Product ID, 0 for name-based GUIDs
    std::uint16_t                m_version{};  //!< Product version, 0 if it applies to all versions
    std::string                  m_namePrefix; //!< Start of the device name for name-based GUIDs
    std::string                  m_name;       //!< Name given by the mapping
    std::string                  m_platform;   //!< Platform the mapping applies to
    std::string                  m_string;     //!< Mapping as it was parsed
    std::vector<GamepadBinding>  m_bindings;   //!< Bindings referring to SDL's indices
};


////////////////////////////////////////////////////////////
/// \brief Collection of the known gamepad mappings
///
////////////////////////////////////////////////////////////
class GamepadMappingDatabase
{
public:
    ////////////////////////////////////////////////////////////
    /// \brief Add the mappings for the current platform
    ///
    /// \param mappings Mappings separated by line breaks
    ///
    /// \return Number of mappings added or replaced
    ///
    ////////////////////////////////////////////////////////////
    std::size_t add(std::string_view mappings);

    ////////////////////////////////////////////////////////////
    /// \brief Find the mapping that fits a joystick best
    ///
    /// \return Mapping, or `nullptr` if none fits
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] const GamepadMapping* find(const Joystick::Identification& identification,
                                             const JoystickLayout&           layout) const;

private:
    std::vector<GamepadMapping> m_mappings; //!< Known mappings
};

} // namespace sf::priv
