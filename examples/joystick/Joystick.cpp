////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics.hpp>

#include <array>
#include <filesystem>
#include <string>
#include <utility>
#include <vector>

using namespace std::literals;

namespace
{
// Axes labels in as C strings
constexpr std::array axisLabels = {"X", "Y", "Z", "R", "U", "V", "PovX", "PovY"};

// Display for axes
class AxisDisplay : public sf::Drawable, public sf::Transformable
{
public:
    AxisDisplay() = delete;
    AxisDisplay(const sf::Font& font, const std::string& label) : m_label(font)
    {
        m_bounds.setFillColor(sf::Color::Transparent);
        m_bounds.setOutlineColor(sf::Color::White);
        m_bounds.setOutlineThickness(1);
        m_bounds.setOrigin({m_bounds.getLocalBounds().size.x / 2, 0});

        m_indicator.setFillColor(sf::Color::Red);
        m_indicator.setOrigin({m_indicator.getLocalBounds().size.x / 2, 0});

        m_label.setCharacterSize(static_cast<unsigned int>(m_bounds.getSize().x));
        m_label.setPosition({0, m_bounds.getSize().y / 2});
        m_label.setString(label);
        m_label.setOrigin({m_label.getLocalBounds().size / 2.f});
    }

    void setValue(float value)
    {
        // We adjust the range from [-100,100] to [0, height]
        m_indicator.setScale({1, (100 + value) * m_bounds.getLocalBounds().size.y / 200});
    }

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override
    {
        states.transform *= getTransform();
        target.draw(m_bounds, states);
        target.draw(m_indicator, states);
        target.draw(m_label, states);
    }

private:
    sf::Text           m_label;
    sf::RectangleShape m_bounds{{20.f, 50.f}};
    sf::RectangleShape m_indicator{{20.f, 1.f}};
};

// Display for an _actual_ joystick (i.e. two combined axes)
class JoystickDisplay : public sf::Drawable, public sf::Transformable
{
public:
    JoystickDisplay() = delete;
    JoystickDisplay(const sf::Font& font, const std::string& label) : m_label(font)
    {
        m_bounds.setOrigin(m_bounds.getLocalBounds().size / 2.f); // Center origin for simplicity
        m_bounds.setFillColor(sf::Color::Transparent);
        m_bounds.setOutlineColor(sf::Color::White);
        m_bounds.setOutlineThickness(1);

        m_indicator.setFillColor(sf::Color::Red);
        m_indicator.setOrigin(m_indicator.getLocalBounds().size / 2.f);

        m_label.setString(label);
        m_label.setCharacterSize(20);
        m_label.setOrigin(m_label.getLocalBounds().size / 2.f + m_label.getLocalBounds().position);
    }

    void setX(float x)
    {
        m_xValue = x;
        updatePos();
    }

    void setY(float y)
    {
        m_yValue = y;
        updatePos();
    }

    void pressed(bool pressed)
    {
        m_bounds.setFillColor(pressed ? sf::Color::Red : sf::Color::Transparent);
    }

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override
    {
        states.transform *= getTransform();
        target.draw(m_bounds, states);
        target.draw(m_indicator, states);
        target.draw(m_label, states);
    }

private:
    void updatePos()
    {
        auto radius = m_bounds.getRadius();
        m_indicator.setPosition({m_xValue * radius / 100, m_yValue * radius / 100});
    }

    sf::CircleShape m_bounds{50.f};
    sf::CircleShape m_indicator{5.f};
    sf::Text        m_label;
    float           m_xValue{};
    float           m_yValue{};
};

// Display a button state
class ButtonDisplay : public sf::Drawable, public sf::Transformable
{
public:
    ButtonDisplay() = delete;
    ButtonDisplay(const sf::Font& font, const std::string& label) : m_label(font)
    {
        m_label.setString(label);
        m_label.setCharacterSize(static_cast<unsigned int>(m_indicator.getLocalBounds().size.y));
        m_label.setOrigin(m_label.getLocalBounds().size / 2.f + m_label.getLocalBounds().position);

        m_indicator.setOrigin(m_indicator.getLocalBounds().size / 2.f);
        m_indicator.setFillColor(sf::Color::Transparent);
        m_indicator.setOutlineColor(sf::Color::White);
        m_indicator.setOutlineThickness(1);
    }

    void pressed(bool pressed)
    {
        m_indicator.setFillColor(pressed ? sf::Color::Red : sf::Color::Transparent);
    }

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override
    {
        states.transform *= getTransform();
        target.draw(m_indicator, states);
        target.draw(m_label, states);
    }

private:
    sf::Text        m_label;
    sf::CircleShape m_indicator{10.f};
};

constexpr sf::Vector2f displaySize{800, 400};

// Display the gamepad layout, using the joystick's gamepad mapping
class GamepadDisplay : public sf::Drawable, public sf::Transformable
{
public:
    GamepadDisplay(const sf::Font& font, unsigned int id) :
        m_leftStick(font, "L"),
        m_rightStick(font, "R"),
        m_leftTrigger(font, "LT"),
        m_rightTrigger(font, "RT"),
        m_label(font),
        m_id(id)
    {
        if (sf::Gamepad::isConnected(id))
            m_label.setString(std::to_string(id) + ": " + sf::Gamepad::getName(id));
        else
            m_label.setString(
                std::to_string(id) + ": " + sf::Joystick::getIdentification(id).name + " (no gamepad mapping)");
        m_label.setCharacterSize(20);

        // Create the rough layout of a typical gamepad
        m_leftStick.setPosition({displaySize.x / 2.f - 100, displaySize.y - 50});
        m_rightStick.setPosition({displaySize.x / 2.f + 100, displaySize.y - 50});

        m_leftTrigger.setPosition({displaySize.x / 2.f - 100, displaySize.y / 5.f});
        m_rightTrigger.setPosition({displaySize.x / 2.f + 100, displaySize.y / 5.f});

        constexpr auto offset = 20.f;

        constexpr sf::Vector2f faceButtonsPos{displaySize.x / 2.f + 250, displaySize.y / 2.f - 50};
        addButton(font, sf::Gamepad::Button::North, "N", faceButtonsPos + sf::Vector2f{0, -offset});
        addButton(font, sf::Gamepad::Button::South, "S", faceButtonsPos + sf::Vector2f{0, offset});
        addButton(font, sf::Gamepad::Button::East, "E", faceButtonsPos + sf::Vector2f{offset, 0});
        addButton(font, sf::Gamepad::Button::West, "W", faceButtonsPos + sf::Vector2f{-offset, 0});

        constexpr sf::Vector2f dPadPos{displaySize.x / 2.f - 250, displaySize.y / 2.f - 50};
        addButton(font, sf::Gamepad::Button::DPadUp, "U", dPadPos + sf::Vector2f{0, -offset});
        addButton(font, sf::Gamepad::Button::DPadDown, "D", dPadPos + sf::Vector2f{0, offset});
        addButton(font, sf::Gamepad::Button::DPadRight, "R", dPadPos + sf::Vector2f{offset, 0});
        addButton(font, sf::Gamepad::Button::DPadLeft, "L", dPadPos + sf::Vector2f{-offset, 0});

        addButton(font, sf::Gamepad::Button::LeftShoulder, "L", {displaySize.x / 2.f - 100, displaySize.y / 5.f + 80.f});
        addButton(font, sf::Gamepad::Button::RightShoulder, "R", {displaySize.x / 2.f + 100, displaySize.y / 5.f + 80.f});

        addButton(font, sf::Gamepad::Button::Back, "-", {displaySize.x / 2 - 20.f, displaySize.y / 2.f});
        addButton(font, sf::Gamepad::Button::Start, "+", {displaySize.x / 2 + 20.f, displaySize.y / 2.f});
        addButton(font, sf::Gamepad::Button::Guide, "G", {displaySize.x / 2.f, displaySize.y / 2.f + 30});
        addButton(font, sf::Gamepad::Button::Misc1, "M", {displaySize.x / 2.f, displaySize.y / 2.f - 30});

        // Show the current state right away, events only report changes
        for (auto i = 0u; i < sf::Gamepad::ButtonCount; ++i)
        {
            const auto button = static_cast<sf::Gamepad::Button>(i);
            setButton(button, sf::Gamepad::isButtonPressed(id, button));
        }

        for (auto i = 0u; i < sf::Gamepad::AxisCount; ++i)
        {
            const auto axis = static_cast<sf::Gamepad::Axis>(i);
            setAxis(axis, sf::Gamepad::getAxisPosition(id, axis));
        }
    }

    void handleEvent(const sf::Event& event)
    {
        if (const auto* gamepadButtonPressed = event.getIf<sf::Event::GamepadButtonPressed>())
        {
            if (gamepadButtonPressed->joystickId == m_id)
                setButton(gamepadButtonPressed->button, true);
        }
        else if (const auto* gamepadButtonReleased = event.getIf<sf::Event::GamepadButtonReleased>())
        {
            if (gamepadButtonReleased->joystickId == m_id)
                setButton(gamepadButtonReleased->button, false);
        }
        else if (const auto* gamepadMoved = event.getIf<sf::Event::GamepadMoved>())
        {
            if (gamepadMoved->joystickId == m_id)
                setAxis(gamepadMoved->axis, gamepadMoved->position);
        }
    }

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override
    {
        states.transform *= getTransform();
        target.draw(m_leftStick, states);
        target.draw(m_rightStick, states);
        target.draw(m_leftTrigger, states);
        target.draw(m_rightTrigger, states);
        for (const auto& [button, display] : m_buttons)
        {
            target.draw(display, states);
        }
        target.draw(m_label, states);
    }

private:
    void addButton(const sf::Font& font, sf::Gamepad::Button button, const std::string& label, sf::Vector2f position)
    {
        ButtonDisplay display(font, label);
        display.setPosition(position);
        m_buttons.emplace_back(button, display);
    }

    void setButton(sf::Gamepad::Button button, bool pressed)
    {
        if (button == sf::Gamepad::Button::LeftStick)
        {
            m_leftStick.pressed(pressed);
            return;
        }

        if (button == sf::Gamepad::Button::RightStick)
        {
            m_rightStick.pressed(pressed);
            return;
        }

        for (auto& [displayedButton, display] : m_buttons)
        {
            if (displayedButton == button)
                display.pressed(pressed);
        }
    }

    void setAxis(sf::Gamepad::Axis axis, float position)
    {
        switch (axis)
        {
            case sf::Gamepad::Axis::LeftX:
                m_leftStick.setX(position);
                break;
            case sf::Gamepad::Axis::LeftY:
                m_leftStick.setY(position);
                break;
            case sf::Gamepad::Axis::RightX:
                m_rightStick.setX(position);
                break;
            case sf::Gamepad::Axis::RightY:
                m_rightStick.setY(position);
                break;
            // Triggers range from 0 to 100, the display from -100 to 100
            case sf::Gamepad::Axis::LeftTrigger:
                m_leftTrigger.setValue(position * 2 - 100);
                break;
            case sf::Gamepad::Axis::RightTrigger:
                m_rightTrigger.setValue(position * 2 - 100);
                break;
        }
    }

    JoystickDisplay                                            m_leftStick;
    JoystickDisplay                                            m_rightStick;
    AxisDisplay                                                m_leftTrigger;
    AxisDisplay                                                m_rightTrigger;
    std::vector<std::pair<sf::Gamepad::Button, ButtonDisplay>> m_buttons;
    sf::Text                                                   m_label;
    unsigned int                                               m_id;
};

// Display the raw joystick state
class GenericDisplay : public sf::Drawable, public sf::Transformable
{
public:
    GenericDisplay(const sf::Font& font, unsigned int id) : m_label(font), m_id(id)
    {
        m_label.setString(std::to_string(id) + ": " + sf::Joystick::getIdentification(id).name);
        m_label.setCharacterSize(20);

        // A row of axes
        constexpr auto axisSpacing = displaySize.x / (sf::Joystick::AxisCount + 1);
        for (auto i = 0u; i < sf::Joystick::AxisCount; ++i)
        {
            AxisDisplay display{font, axisLabels[i]};
            display.setPosition({axisSpacing + static_cast<float>(i) * axisSpacing, displaySize.y * 0.25f});
            display.setValue(sf::Joystick::getAxisPosition(id, static_cast<sf::Joystick::Axis>(i)));
            m_axisDisplays.emplace_back(display);
        }

        // and two rows of buttons
        constexpr auto buttonSpacing = displaySize.x / (sf::Joystick::ButtonCount / 2.f + 1);
        for (auto i = 0u; i < sf::Joystick::ButtonCount; ++i)
        {
            ButtonDisplay display{font, std::to_string(i)};
            display.setPosition({buttonSpacing + static_cast<float>(i % sf::Joystick::ButtonCount) / 2.f * buttonSpacing,
                                 displaySize.y * 0.75f + static_cast<float>(i % 2 * 50)});
            display.pressed(sf::Joystick::isButtonPressed(id, i));
            m_buttonDisplays.emplace_back(display);
        }
    }

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override
    {
        states.transform *= getTransform();
        target.draw(m_label, states);
        for (const auto& axis : m_axisDisplays)
        {
            target.draw(axis, states);
        }

        for (const auto& button : m_buttonDisplays)
        {
            target.draw(button, states);
        }
    }

    void handleEvent(const sf::Event& event)
    {
        if (const auto* joystickButtonPressed = event.getIf<sf::Event::JoystickButtonPressed>())
        {
            if (joystickButtonPressed->joystickId == m_id)
                m_buttonDisplays[joystickButtonPressed->button].pressed(true);
        }
        else if (const auto* joystickButtonReleased = event.getIf<sf::Event::JoystickButtonReleased>())
        {
            if (joystickButtonReleased->joystickId == m_id)
                m_buttonDisplays[joystickButtonReleased->button].pressed(false);
        }
        else if (const auto* joystickMoved = event.getIf<sf::Event::JoystickMoved>())
        {
            if (joystickMoved->joystickId == m_id)
                m_axisDisplays[static_cast<std::size_t>(joystickMoved->axis)].setValue(joystickMoved->position);
        }
    }

private:
    sf::Text                   m_label;
    std::vector<AxisDisplay>   m_axisDisplays;
    std::vector<ButtonDisplay> m_buttonDisplays;
    unsigned int               m_id;
};

} // namespace

////////////////////////////////////////////////////////////
/// Entry point of application
///
/// \return Application exit code
///
////////////////////////////////////////////////////////////
int main()
{
    // SFML comes with a copy of the SDL gamepad database, but a newer one can be downloaded next to the example
    // (https://github.com/mdqinc/SDL_GameControllerDB/blob/master/gamecontrollerdb.txt)
    const std::filesystem::path mappingsPath = "gamecontrollerdb.txt";
    if (std::filesystem::exists(mappingsPath))
        sf::Gamepad::addMappingsFromFile(mappingsPath);

    // Create the window of the application
    constexpr sf::Vector2f windowSize{displaySize.x, displaySize.y + 200}; // + 200 for stuff at the top
    sf::RenderWindow       window(sf::VideoMode(sf::Vector2u(windowSize)), "Joystick");
    window.setVerticalSyncEnabled(true);

    // Open the text font
    const sf::Font font("resources/tuffy.ttf");

    // Display for currently selected joystick - a row of numbers along the top
    // Red if disconnected, yellow if connected, green if usable as a gamepad, white box for current selection
    auto                  joystickIndex = 0u;
    std::vector<sf::Text> indexIndicators;
    constexpr auto        spacer = displaySize.x / (sf::Joystick::Count + 1); // +1 to account for padding at the end
    for (auto i = 0u; i < sf::Joystick::Count; ++i)
    {
        sf::Text indicator(font, std::to_string(i));
        indicator.setPosition({spacer + static_cast<float>(i) * spacer, 60});
        indicator.setOrigin(indicator.getLocalBounds().size / 2.f + indicator.getLocalBounds().position);
        indexIndicators.emplace_back(indicator);
    }
    sf::RectangleShape selectedIndicator(indexIndicators[joystickIndex].getGlobalBounds().size * 2.f);
    selectedIndicator.setPosition(indexIndicators[joystickIndex].getPosition());
    selectedIndicator.setOutlineThickness(3);
    selectedIndicator.setFillColor(sf::Color::Transparent);
    selectedIndicator.setOrigin(selectedIndicator.getLocalBounds().size / 2.f + selectedIndicator.getLocalBounds().position);

    auto updateIndicators = [&]
    {
        for (auto i = 0u; i < indexIndicators.size(); ++i)
        {
            if (sf::Gamepad::isConnected(i))
            {
                indexIndicators[i].setFillColor(sf::Color::Green);
            }
            else if (sf::Joystick::isConnected(i))
            {
                indexIndicators[i].setFillColor(sf::Color::Yellow);
            }
            else
            {
                indexIndicators[i].setFillColor(sf::Color::Red);
            }
        }
        selectedIndicator.setPosition(indexIndicators[joystickIndex].getPosition());
    };
    updateIndicators();

    // Instruction text
    sf::Text instructions(font, "Press 0-7 to select joystick, tab to switch between gamepad/generic displays", 20);
    instructions.setPosition({displaySize.x / 2, 10});
    instructions.setOrigin(instructions.getLocalBounds().position + instructions.getLocalBounds().size / 2.f);

    // Text when a controller isn't connected
    sf::Text notConnected(font, "Joystick not connected");
    notConnected.setPosition(displaySize / 2.f);
    notConnected.setOrigin(notConnected.getLocalBounds().position + notConnected.getLocalBounds().size / 2.f);

    // The actual displays - toggle between generic and gamepad with tab
    bool           showingGamepad = true;
    GamepadDisplay gamepadDisplay(font, joystickIndex);
    GenericDisplay genericDisplay(font, joystickIndex);
    auto           updateDisplay = [&]
    {
        constexpr auto displayOffset = 150;
        gamepadDisplay               = GamepadDisplay(font, joystickIndex);
        gamepadDisplay.setPosition({0, displayOffset});

        genericDisplay = GenericDisplay(font, joystickIndex);
        genericDisplay.setPosition({0, displayOffset});
    };
    updateDisplay();

    while (window.isOpen())
    {
        // Handle events
        while (const std::optional event = window.pollEvent())
        {
            // Window closed or escape key pressed: exit
            if (event->is<sf::Event::Closed>() ||
                (event->is<sf::Event::KeyPressed>() &&
                 event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::Escape))
            {
                window.close();
                break;
            }
            if (const auto* resized = event->getIf<sf::Event::Resized>())
            {
                window.setView(sf::View(sf::FloatRect({}, sf::Vector2f(resized->size))));
            }
            else if (event->is<sf::Event::JoystickConnected>() || event->is<sf::Event::JoystickDisconnected>() ||
                     event->is<sf::Event::GamepadConnected>() || event->is<sf::Event::GamepadDisconnected>())
            {
                updateDisplay();
                updateIndicators();
            }
            else if (const auto* keyPress = event->getIf<sf::Event::KeyPressed>())
            {
                if (keyPress->code >= sf::Keyboard::Key::Num0 && keyPress->code <= sf::Keyboard::Key::Num7)
                {
                    joystickIndex = static_cast<unsigned int>(keyPress->code) -
                                    static_cast<unsigned int>(sf::Keyboard::Key::Num0);
                    updateDisplay();
                    updateIndicators();
                }
                else if (keyPress->code == sf::Keyboard::Key::Tab)
                {
                    showingGamepad = !showingGamepad;
                }
            }
            else
            {
                gamepadDisplay.handleEvent(*event);
                genericDisplay.handleEvent(*event);
            }
        }

        window.clear();

        window.draw(instructions);

        window.draw(selectedIndicator);
        for (const auto& indicator : indexIndicators)
        {
            window.draw(indicator);
        }

        if (!sf::Joystick::isConnected(joystickIndex))
        {
            window.draw(notConnected);
        }
        else if (showingGamepad)
        {
            window.draw(gamepadDisplay);
        }
        else
        {
            window.draw(genericDisplay);
        }

        // Display things on screen
        window.display();
    }
}
