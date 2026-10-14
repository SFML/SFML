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

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Window/GamepadMapping.hpp>

#include <algorithm>
#include <charconv>
#include <utility>


namespace
{
// Name SDL uses for the platform in the "platform:" field
#if defined(SFML_SYSTEM_WINDOWS)
constexpr std::string_view currentPlatform = "Windows";
#elif defined(SFML_SYSTEM_MACOS)
constexpr std::string_view currentPlatform = "Mac OS X";
#elif defined(SFML_SYSTEM_LINUX)
constexpr std::string_view currentPlatform = "Linux";
#elif defined(SFML_SYSTEM_ANDROID)
constexpr std::string_view currentPlatform = "Android";
#elif defined(SFML_SYSTEM_IOS)
constexpr std::string_view currentPlatform = "iOS";
#elif defined(SFML_SYSTEM_FREEBSD)
constexpr std::string_view currentPlatform = "FreeBSD";
#elif defined(SFML_SYSTEM_OPENBSD)
constexpr std::string_view currentPlatform = "OpenBSD";
#elif defined(SFML_SYSTEM_NETBSD)
constexpr std::string_view currentPlatform = "NetBSD";
#else
constexpr std::string_view currentPlatform;
#endif

// Hat directions as SDL encodes them
constexpr unsigned int hatUp    = 1;
constexpr unsigned int hatRight = 2;
constexpr unsigned int hatDown  = 4;
constexpr unsigned int hatLeft  = 8;

// Tolerance for axis values that end up slightly outside of their range due to rounding
constexpr float rangeTolerance = 0.001f;

std::string_view trim(std::string_view text)
{
    const auto isSpace = [](char c) { return c == ' ' || c == '\t' || c == '\r' || c == '\n'; };

    while (!text.empty() && isSpace(text.front()))
        text.remove_prefix(1);

    while (!text.empty() && isSpace(text.back()))
        text.remove_suffix(1);

    return text;
}

std::optional<unsigned int> parseUnsigned(std::string_view text, int base = 10)
{
    unsigned int value{};
    const auto [end, error] = std::from_chars(text.data(), text.data() + text.size(), value, base);

    if (error != std::errc() || end != text.data() + text.size())
        return std::nullopt;

    return value;
}

std::optional<std::array<std::uint8_t, 16>> parseGuid(std::string_view text)
{
    std::array<std::uint8_t, 16> guid{};

    if (text.size() != guid.size() * 2)
        return std::nullopt;

    for (std::size_t i = 0; i < guid.size(); ++i)
    {
        const auto byte = parseUnsigned(text.substr(i * 2, 2), 16);
        if (!byte)
            return std::nullopt;

        guid[i] = static_cast<std::uint8_t>(*byte);
    }

    return guid;
}

std::uint16_t readUint16(const std::array<std::uint8_t, 16>& guid, std::size_t offset)
{
    return static_cast<std::uint16_t>(guid[offset] | (guid[offset + 1] << 8));
}

std::optional<sf::Gamepad::Button> toButton(std::string_view name)
{
    using sf::Gamepad::Button;

    // clang-format off
    if (name == "a")             return Button::South;
    if (name == "b")             return Button::East;
    if (name == "x")             return Button::West;
    if (name == "y")             return Button::North;
    if (name == "back")          return Button::Back;
    if (name == "guide")         return Button::Guide;
    if (name == "start")         return Button::Start;
    if (name == "leftstick")     return Button::LeftStick;
    if (name == "rightstick")    return Button::RightStick;
    if (name == "leftshoulder")  return Button::LeftShoulder;
    if (name == "rightshoulder") return Button::RightShoulder;
    if (name == "dpup")          return Button::DPadUp;
    if (name == "dpdown")        return Button::DPadDown;
    if (name == "dpleft")        return Button::DPadLeft;
    if (name == "dpright")       return Button::DPadRight;
    if (name == "misc1")         return Button::Misc1;
    if (name == "paddle1")       return Button::RightPaddle1;
    if (name == "paddle2")       return Button::LeftPaddle1;
    if (name == "paddle3")       return Button::RightPaddle2;
    if (name == "paddle4")       return Button::LeftPaddle2;
    if (name == "touchpad")      return Button::Touchpad;
    // clang-format on

    return std::nullopt;
}

std::optional<sf::Gamepad::Axis> toAxis(std::string_view name)
{
    using sf::Gamepad::Axis;

    // clang-format off
    if (name == "leftx")        return Axis::LeftX;
    if (name == "lefty")        return Axis::LeftY;
    if (name == "rightx")       return Axis::RightX;
    if (name == "righty")       return Axis::RightY;
    if (name == "lefttrigger")  return Axis::LeftTrigger;
    if (name == "righttrigger") return Axis::RightTrigger;
    // clang-format on

    return std::nullopt;
}

bool isTrigger(sf::Gamepad::Axis axis)
{
    return axis == sf::Gamepad::Axis::LeftTrigger || axis == sf::Gamepad::Axis::RightTrigger;
}

// Parse an input such as "b3", "a2", "+a2", "-a1", "a5~" or "h0.4"
std::optional<sf::priv::GamepadInput> parseInput(std::string_view text)
{
    sf::priv::GamepadInput input;

    char half = 0;
    if (!text.empty() && (text.front() == '+' || text.front() == '-'))
    {
        half = text.front();
        text.remove_prefix(1);
    }

    bool inverted = false;
    if (!text.empty() && text.back() == '~')
    {
        inverted = true;
        text.remove_suffix(1);
    }

    if (text.size() < 2)
        return std::nullopt;

    const char type = text.front();
    text.remove_prefix(1);

    switch (type)
    {
        case 'b':
        {
            const auto index = parseUnsigned(text);
            if (!index || half || inverted)
                return std::nullopt;

            input.type  = sf::priv::GamepadInput::Type::Button;
            input.index = *index;
            return input;
        }

        case 'a':
        {
            const auto index = parseUnsigned(text);
            if (!index)
                return std::nullopt;

            input.type  = sf::priv::GamepadInput::Type::Axis;
            input.index = *index;

            if (half == '+')
            {
                input.min = 0.f;
                input.max = 1.f;
            }
            else if (half == '-')
            {
                input.min = 0.f;
                input.max = -1.f;
            }

            if (inverted)
                std::swap(input.min, input.max);

            return input;
        }

        case 'h':
        {
            const auto dot = text.find('.');
            if (dot == std::string_view::npos || half || inverted)
                return std::nullopt;

            const auto index = parseUnsigned(text.substr(0, dot));
            const auto mask  = parseUnsigned(text.substr(dot + 1));
            if (!index || !mask)
                return std::nullopt;

            input.type    = sf::priv::GamepadInput::Type::Hat;
            input.index   = *index;
            input.hatMask = *mask;
            return input;
        }

        default:
            return std::nullopt;
    }
}

// Position of an axis input within its range, from 0 to 1, or std::nullopt if outside of the range
std::optional<float> normalize(const sf::priv::GamepadInput& input, float value)
{
    const float position = (value - input.min) / (input.max - input.min);

    if (position < -rangeTolerance || position > 1.f + rangeTolerance)
        return std::nullopt;

    return std::clamp(position, 0.f, 1.f);
}

unsigned int getHat(const sf::priv::GamepadInput& input, const sf::priv::JoystickState& state)
{
    // PovX and PovY can report diagonals as about 70, so anything past halfway counts
    constexpr float threshold = 50.f;

    const float x = state.axes[sf::Joystick::Axis::PovX];
    const float y = input.hatUpIsNegative ? -state.axes[sf::Joystick::Axis::PovY] : state.axes[sf::Joystick::Axis::PovY];

    unsigned int hat = 0;

    if (y > threshold)
        hat |= hatUp;
    if (x > threshold)
        hat |= hatRight;
    if (y < -threshold)
        hat |= hatDown;
    if (x < -threshold)
        hat |= hatLeft;

    return hat;
}
} // namespace


namespace sf::priv
{
////////////////////////////////////////////////////////////
GamepadBindings::GamepadBindings(std::vector<GamepadBinding> bindings) : m_bindings(std::move(bindings))
{
}


////////////////////////////////////////////////////////////
GamepadState GamepadBindings::evaluate(const JoystickState& state) const
{
    GamepadState gamepadState;
    gamepadState.connected = state.connected;

    if (!state.connected)
        return gamepadState;

    // Same rules as SDL: a button is pressed if any of its bindings is,
    // and an axis takes the value of the first binding that isn't 0
    for (const GamepadBinding& binding : m_bindings)
    {
        const GamepadInput& input = binding.input;

        if (binding.isAxis)
        {
            float& output = gamepadState.axes[binding.axis];
            if (output != 0.f)
                continue;

            float value = 0.f;

            switch (input.type)
            {
                case GamepadInput::Type::Button:
                    value = state.buttons[input.index] ? binding.max : 0.f;
                    break;

                case GamepadInput::Type::Axis:
                    if (const auto position = normalize(input, state.axes[input.axis] / 100.f))
                        value = binding.min + *position * (binding.max - binding.min);
                    break;

                case GamepadInput::Type::Hat:
                    value = (getHat(input, state) & input.hatMask) ? binding.max : 0.f;
                    break;
            }

            output = value * 100.f;
        }
        else
        {
            bool& output = gamepadState.buttons[binding.button];
            if (output)
                continue;

            switch (input.type)
            {
                case GamepadInput::Type::Button:
                    output = state.buttons[input.index];
                    break;

                case GamepadInput::Type::Axis:
                    if (const auto position = normalize(input, state.axes[input.axis] / 100.f))
                        output = *position >= 0.5f;
                    break;

                case GamepadInput::Type::Hat:
                    output = (getHat(input, state) & input.hatMask) != 0;
                    break;
            }
        }
    }

    return gamepadState;
}


////////////////////////////////////////////////////////////
bool GamepadBindings::hasButton(Gamepad::Button button) const
{
    return std::any_of(m_bindings.begin(),
                       m_bindings.end(),
                       [button](const GamepadBinding& binding) { return !binding.isAxis && binding.button == button; });
}


////////////////////////////////////////////////////////////
bool GamepadBindings::hasAxis(Gamepad::Axis axis) const
{
    return std::any_of(m_bindings.begin(),
                       m_bindings.end(),
                       [axis](const GamepadBinding& binding) { return binding.isAxis && binding.axis == axis; });
}


////////////////////////////////////////////////////////////
std::optional<GamepadMapping> GamepadMapping::parse(std::string_view line)
{
    line = trim(line);

    if (line.empty() || line.front() == '#')
        return std::nullopt;

    GamepadMapping mapping;
    mapping.m_string = line;

    // Split the line into its comma-separated fields
    std::vector<std::string_view> fields;
    while (!line.empty())
    {
        const auto comma = line.find(',');
        fields.push_back(trim(line.substr(0, comma)));
        line.remove_prefix(comma == std::string_view::npos ? line.size() : comma + 1);
    }

    if (fields.size() < 2)
        return std::nullopt;

    const auto guid = parseGuid(fields[0]);
    if (!guid)
        return std::nullopt;

    mapping.m_guid = *guid;
    mapping.m_name = fields[1];

    // Older SDL versions created GUIDs from DirectInput product GUIDs, which end in "PIDVID"
    static constexpr std::array<std::uint8_t, 6> pidVid{'P', 'I', 'D', 'V', 'I', 'D'};
    if (std::equal(pidVid.begin(), pidVid.end(), guid->begin() + 10))
    {
        mapping.m_bus     = 0x03;
        mapping.m_vendor  = readUint16(*guid, 0);
        mapping.m_product = readUint16(*guid, 2);
    }
    else
    {
        mapping.m_bus = readUint16(*guid, 0);

        if (readUint16(*guid, 4) != 0 && readUint16(*guid, 6) == 0 && readUint16(*guid, 10) == 0)
        {
            // Bytes 14 and 15 hold a driver signature for SDL's own drivers (HIDAPI, XInput, ...),
            // whose layout differs from what the operating system reports
            if ((*guid)[14] != 0)
                return std::nullopt;

            mapping.m_vendor  = readUint16(*guid, 4);
            mapping.m_product = readUint16(*guid, 8);
            mapping.m_version = readUint16(*guid, 12);
        }
        else
        {
            // Without vendor and product ID, SDL stores the start of the device name instead
            for (std::size_t i = 4; i < guid->size() && (*guid)[i] != 0; ++i)
                mapping.m_namePrefix += static_cast<char>((*guid)[i]);
        }
    }

    for (std::size_t i = 2; i < fields.size(); ++i)
    {
        const std::string_view field = fields[i];
        const auto             colon = field.find(':');

        if (colon == std::string_view::npos)
            continue;

        std::string_view       key   = field.substr(0, colon);
        const std::string_view value = field.substr(colon + 1);

        if (key == "platform")
        {
            mapping.m_platform = value;
            continue;
        }

        if (key == "hint")
        {
            // Hints switch between mappings depending on SDL settings ("hint:!NAME:=1" applies
            // when NAME isn't set to 1). SFML has none of these settings, so treat them as unset.
            if (value.empty() || value.front() != '!')
                return std::nullopt;
            continue;
        }

        char half = 0;
        if (!key.empty() && (key.front() == '+' || key.front() == '-'))
        {
            half = key.front();
            key.remove_prefix(1);
        }

        const auto input = parseInput(value);
        if (!input)
            continue;

        GamepadBinding binding;
        binding.input = *input;

        if (const auto button = toButton(key); button && !half)
        {
            binding.button = *button;
        }
        else if (const auto axis = toAxis(key))
        {
            binding.isAxis = true;
            binding.axis   = *axis;

            // Triggers rest at 0, a half axis output ("+leftx") only covers one direction
            if (isTrigger(*axis) || half)
                binding.min = 0.f;

            if (!isTrigger(*axis) && half == '-')
                binding.max = -1.f;
        }
        else
        {
            // "crc", "sdk>=" or anything SFML doesn't support
            continue;
        }

        mapping.m_bindings.push_back(binding);
    }

    return mapping;
}


////////////////////////////////////////////////////////////
const std::string& GamepadMapping::getName() const
{
    return m_name;
}


////////////////////////////////////////////////////////////
const std::string& GamepadMapping::getPlatform() const
{
    return m_platform;
}


////////////////////////////////////////////////////////////
const std::string& GamepadMapping::getString() const
{
    return m_string;
}


////////////////////////////////////////////////////////////
bool GamepadMapping::isForSameDevice(const GamepadMapping& other) const
{
    return m_guid == other.m_guid && m_platform == other.m_platform;
}


////////////////////////////////////////////////////////////
int GamepadMapping::getMatchScore(const Joystick::Identification& identification, const JoystickLayout& layout) const
{
    if (m_vendor == 0 && m_product == 0)
    {
        if (identification.vendorId != 0 || identification.productId != 0 || m_namePrefix.empty())
            return 0;

        return identification.name.toAnsiString().rfind(m_namePrefix, 0) == 0 ? 1 : 0;
    }

    if (m_vendor != identification.vendorId || m_product != identification.productId)
        return 0;

    // SDL only accepts an exact match or a mapping for all versions of the device. Bus and
    // version are less reliable with SFML's backends, so fall back to any mapping for
    // the vendor and product ID, and prefer the closest one.
    int score = 2;

    if (m_bus == layout.bus)
        score += 4;

    if (m_version == layout.version)
        score += 2;
    else if (m_version == 0)
        score += 1;

    return score;
}


////////////////////////////////////////////////////////////
GamepadBindings GamepadMapping::resolve(const JoystickCaps& capabilities, const JoystickLayout& layout) const
{
    std::vector<GamepadBinding> bindings;

    for (GamepadBinding binding : m_bindings)
    {
        GamepadInput& input = binding.input;

        switch (input.type)
        {
            case GamepadInput::Type::Button:
                // SFML numbers the buttons in the same order as SDL
                if (input.index >= capabilities.buttonCount)
                    continue;
                break;

            case GamepadInput::Type::Axis:
                if (input.index >= layout.axes.size() || !layout.axes[input.index] ||
                    !capabilities.axes[*layout.axes[input.index]])
                    continue;
                input.axis = *layout.axes[input.index];
                break;

            case GamepadInput::Type::Hat:
                // SFML only reads the first hat
                if (input.index != 0 || !layout.hasHat)
                    continue;
                input.hatUpIsNegative = layout.hatUpIsNegative;
                break;
        }

        bindings.push_back(binding);
    }

    return GamepadBindings(std::move(bindings));
}


////////////////////////////////////////////////////////////
std::size_t GamepadMappingDatabase::add(std::string_view mappings)
{
    std::size_t count = 0;

    while (!mappings.empty())
    {
        const auto end  = mappings.find_first_of("\r\n");
        const auto line = mappings.substr(0, end);
        mappings.remove_prefix(end == std::string_view::npos ? mappings.size() : end + 1);

        auto mapping = GamepadMapping::parse(line);
        if (!mapping)
            continue;

        if (!mapping->getPlatform().empty() && mapping->getPlatform() != currentPlatform)
            continue;

        // Like SDL, a new mapping for a device replaces the existing one
        const auto existing = std::find_if(m_mappings.begin(),
                                           m_mappings.end(),
                                           [&](const GamepadMapping& other) { return other.isForSameDevice(*mapping); });

        if (existing != m_mappings.end())
            *existing = std::move(*mapping);
        else
            m_mappings.push_back(std::move(*mapping));

        ++count;
    }

    return count;
}


////////////////////////////////////////////////////////////
const GamepadMapping* GamepadMappingDatabase::find(const Joystick::Identification& identification,
                                                   const JoystickLayout&           layout) const
{
    const GamepadMapping* bestMapping = nullptr;
    int                   bestScore   = 0;

    // On equal scores, the mapping added last wins so users can override the database
    for (const GamepadMapping& mapping : m_mappings)
    {
        const int score = mapping.getMatchScore(identification, layout);
        if (score > 0 && score >= bestScore)
        {
            bestMapping = &mapping;
            bestScore   = score;
        }
    }

    return bestMapping;
}

} // namespace sf::priv
