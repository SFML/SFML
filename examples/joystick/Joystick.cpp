////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics.hpp>

#include <string>

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
    AxisDisplay(const sf::Font& font, sf::Joystick::Axis axis) : m_label(font), m_axis(axis)
    {
        m_bounds.setFillColor(sf::Color::Transparent);
        m_bounds.setOutlineColor(sf::Color::White);
        m_bounds.setOutlineThickness(1);
        m_bounds.setOrigin({m_bounds.getLocalBounds().size.x / 2, 0});

        m_indicator.setFillColor(sf::Color::Red);
        m_indicator.setOrigin({m_indicator.getLocalBounds().size.x / 2, 0});

        m_label.setCharacterSize(static_cast<unsigned int>(m_bounds.getSize().x));
        m_label.setPosition({0, m_bounds.getSize().y / 2});
        m_label.setString(axisLabels[static_cast<unsigned int>(axis)]);
        m_label.setOrigin({m_label.getLocalBounds().size / 2.f});
    }

    void setValue(float value)
    {
        // We adjust the range from [-100,100] to [0, height]
        m_indicator.setScale({1, (100 + value) * m_bounds.getLocalBounds().size.y / 200});
    }

    sf::Joystick::Axis getAxis() const
    {
        return m_axis;
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
    sf::Joystick::Axis m_axis;
};

// Display for an _actual_ joystick (i.e. two combined axes)
class JoystickDisplay : public sf::Drawable, public sf::Transformable
{

public:
    JoystickDisplay() = delete;
    JoystickDisplay(const sf::Font&    font,
                    sf::Joystick::Axis xAxis,
                    sf::Joystick::Axis yAxis,
                    std::optional<int> button = std::nullopt) :
        m_xAxis(xAxis),
        m_yAxis(yAxis),
        m_button(button),
        m_label(font)
    {
        m_bounds.setOrigin(m_bounds.getLocalBounds().size / 2.f); // Center origin for simplicity
        m_bounds.setFillColor(sf::Color::Transparent);
        m_bounds.setOutlineColor(sf::Color::White);
        m_bounds.setOutlineThickness(1);

        m_indicator.setFillColor(sf::Color::Red);
        m_indicator.setOrigin(m_indicator.getLocalBounds().size / 2.f);

        m_label.setString(
            axisLabels[static_cast<unsigned int>(xAxis)] + "/"s + axisLabels[static_cast<unsigned int>(yAxis)]);
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

    sf::Joystick::Axis getXAxis() const
    {
        return m_xAxis;
    }

    sf::Joystick::Axis getYAxis() const
    {
        return m_yAxis;
    }

    void pressed(bool pressed)
    {
        m_bounds.setFillColor(pressed ? sf::Color::Red : sf::Color::Transparent);
    }

    std::optional<unsigned int> getButton() const
    {
        return m_button;
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
    sf::Joystick::Axis          m_xAxis;
    sf::Joystick::Axis          m_yAxis;
    std::optional<unsigned int> m_button;
    sf::CircleShape             m_bounds{50.f};
    sf::CircleShape             m_indicator{5.f};
    sf::Text                    m_label;
    float                       m_xValue{};
    float                       m_yValue{};
};

// Display a button state
class ButtonDisplay : public sf::Drawable, public sf::Transformable
{
public:
    ButtonDisplay() = delete;
    ButtonDisplay(const sf::Font& font, unsigned int button) : m_label(font), m_button(button)
    {
        m_label.setString(std::to_string(button));
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

    unsigned int getButton() const
    {
        return m_button;
    }

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override
    {
        states.transform *= getTransform();
        target.draw(m_indicator, states);
        target.draw(m_label, states);
    }

private:
    sf::Text        m_label;
    unsigned int    m_button;
    sf::CircleShape m_indicator{10.f};
};

constexpr sf::Vector2f displaySize{800, 400};

// Display a basic gamepad layout
class GamepadDisplay : public sf::Drawable, public sf::Transformable
{
public:
    GamepadDisplay(const sf::Font& font, unsigned int id) : m_label(font), m_id(id)
    {
        m_label.setString(std::to_string(id) + ": " + sf::Joystick::getIdentification(id).name);
        m_label.setCharacterSize(20);

        // Create the rough layout of a typical gamepad
        m_joysticks.emplace_back(font, sf::Joystick::Axis::X, sf::Joystick::Axis::Y, 10); // Left stick
        m_joysticks.back().setPosition({displaySize.x / 2.f - 100, displaySize.y - 50});
        m_joysticks.emplace_back(font, sf::Joystick::Axis::Z, sf::Joystick::Axis::R, 11); // Right stick
        m_joysticks.back().setPosition({displaySize.x / 2.f + 100, displaySize.y - 50});


        constexpr sf::Vector2f mainButtonPos{displaySize.x / 2.f + 250, displaySize.y / 2.f - 50};
        constexpr auto         offset = 20.f;
        m_buttons.emplace_back(font, 3); // North button
        m_buttons.back().setPosition(mainButtonPos + sf::Vector2f{0, -offset});
        m_buttons.emplace_back(font, 1); // South button
        m_buttons.back().setPosition(mainButtonPos + sf::Vector2f{0, offset});
        m_buttons.emplace_back(font, 2); // East button
        m_buttons.back().setPosition(mainButtonPos + sf::Vector2f{offset, 0});
        m_buttons.emplace_back(font, 0); // West button
        m_buttons.back().setPosition(mainButtonPos + sf::Vector2f{-offset, 0});

        m_triggers.emplace_back(font, sf::Joystick::Axis::V); // Right trigger
        m_triggers.back().setPosition({displaySize.x / 2.f + 100, displaySize.y / 5.f});
        m_triggers.emplace_back(font, sf::Joystick::Axis::U); // Left trigger
        m_triggers.back().setPosition({displaySize.x / 2.f - 100, displaySize.y / 5.f});

        m_buttons.emplace_back(font, 7); // Right trigger
        m_buttons.back().setPosition(m_triggers[0].getPosition() + sf::Vector2f{0, -30});
        m_buttons.emplace_back(font, 6); // Left trigger
        m_buttons.back().setPosition(m_triggers[1].getPosition() + sf::Vector2f{0, -30});

        m_buttons.emplace_back(font, 5); // Left shoulder
        m_buttons.back().setPosition({displaySize.x / 2.f + 100, displaySize.y / 5.f + 80.f});
        m_buttons.emplace_back(font, 4); // Right shoulder
        m_buttons.back().setPosition({displaySize.x / 2.f - 100, displaySize.y / 5.f + 80.f});

        m_joysticks.emplace_back(font, sf::Joystick::Axis::PovX, sf::Joystick::Axis::PovY); // dpad
        m_joysticks.back().setPosition({displaySize.x / 2.f - 250, displaySize.y / 2.f - 50});

        m_buttons.emplace_back(font, 8); // Back button
        m_buttons.back().setPosition({displaySize.x / 2 - 20.f, displaySize.y / 2.f});
        m_buttons.emplace_back(font, 9); // Start button
        m_buttons.back().setPosition({displaySize.x / 2 + 20.f, displaySize.y / 2.f});

        m_buttons.emplace_back(font, 12); // Guide button
        m_buttons.back().setPosition({displaySize.x / 2.f, displaySize.y / 2.f + 30});
    }

    void handleEvent(std::optional<sf::Event> event)
    {
        if (const auto* joystickButtonPressed = event->getIf<sf::Event::JoystickButtonPressed>())
        {
            if (joystickButtonPressed->joystickId == m_id)
            {
                for (auto& button : m_buttons)
                {
                    if (button.getButton() == joystickButtonPressed->button)
                    {
                        button.pressed(true);
                        return;
                    }
                }

                for (auto& joystick : m_joysticks)
                {
                    if (joystick.getButton() == joystickButtonPressed->button)
                    {
                        joystick.pressed(true);
                        return;
                    }
                }
            }
        }
        else if (const auto* joystickButtonReleased = event->getIf<sf::Event::JoystickButtonReleased>())
        {
            if (joystickButtonReleased->joystickId == m_id)
            {
                for (auto& button : m_buttons)
                {
                    if (button.getButton() == joystickButtonReleased->button)
                    {
                        button.pressed(false);
                        return;
                    }
                }

                for (auto& joystick : m_joysticks)
                {
                    if (joystick.getButton() == joystickButtonReleased->button)
                    {
                        joystick.pressed(false);
                        return;
                    }
                }
            }
        }
        else if (const auto* joystickMoved = event->getIf<sf::Event::JoystickMoved>())
        {
            if (joystickMoved->joystickId == m_id)
            {
                for (auto& joystick : m_joysticks)
                {
                    if (joystick.getXAxis() == joystickMoved->axis)
                    {
                        joystick.setX(joystickMoved->position);
                        return;
                    }
                    if (joystick.getYAxis() == joystickMoved->axis)
                    {
                        joystick.setY(joystickMoved->position);
                        return;
                    }
                }

                for (auto& trigger : m_triggers)
                {
                    if (trigger.getAxis() == joystickMoved->axis)
                    {
                        trigger.setValue(joystickMoved->position);
                        return;
                    }
                }
            }
        }
    }

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override
    {
        states.transform *= getTransform();
        for (const auto& joystick : m_joysticks)
        {
            target.draw(joystick, states);
        }
        for (const auto& trigger : m_triggers)
        {
            target.draw(trigger, states);
        }
        for (const auto& button : m_buttons)
        {
            target.draw(button, states);
        }
        target.draw(m_label, states);
    }

private:
    std::vector<JoystickDisplay> m_joysticks;
    std::vector<AxisDisplay>     m_triggers;
    std::vector<ButtonDisplay>   m_buttons;
    sf::Text                     m_label;
    unsigned int                 m_id;
};

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
            AxisDisplay display{font, static_cast<sf::Joystick::Axis>(i)};
            display.setPosition({axisSpacing + static_cast<float>(i) * axisSpacing, displaySize.y * 0.25f});
            m_axisDisplays.emplace_back(display);
        }

        // and two rows of buttons
        constexpr auto buttonSpacing = displaySize.x / (sf::Joystick::ButtonCount / 2.f + 1);
        for (auto i = 0u; i < sf::Joystick::ButtonCount; ++i)
        {
            ButtonDisplay display{font, i};
            display.setPosition({buttonSpacing + static_cast<float>(i % sf::Joystick::ButtonCount) / 2.f * buttonSpacing,
                                 displaySize.y * 0.75f + static_cast<float>(i % 2 * 50)});
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

    void handleEvent(std::optional<sf::Event> event)
    {
        if (const auto* joystickButtonPressed = event->getIf<sf::Event::JoystickButtonPressed>())
        {
            if (joystickButtonPressed->joystickId == m_id)
            {
                for (auto& button : m_buttonDisplays)
                {
                    if (button.getButton() == joystickButtonPressed->button)
                    {
                        button.pressed(true);
                        return;
                    }
                }
            }
        }
        else if (const auto* joystickButtonReleased = event->getIf<sf::Event::JoystickButtonReleased>())
        {
            if (joystickButtonReleased->joystickId == m_id)
            {
                for (auto& button : m_buttonDisplays)
                {
                    if (button.getButton() == joystickButtonReleased->button)
                    {
                        button.pressed(false);
                        return;
                    }
                }
            }
        }
        else if (const auto* joystickMoved = event->getIf<sf::Event::JoystickMoved>())
        {
            if (joystickMoved->joystickId == m_id)
            {
                for (auto& trigger : m_axisDisplays)
                {
                    if (trigger.getAxis() == joystickMoved->axis)
                    {
                        trigger.setValue(joystickMoved->position);
                        return;
                    }
                }
            }
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
    // Create the window of the application
    constexpr sf::Vector2f windowSize{displaySize.x, displaySize.y + 200}; // + 200 for stuff at the top
    sf::RenderWindow       window(sf::VideoMode(sf::Vector2u(windowSize)), "Joystick");
    window.setVerticalSyncEnabled(true);

    // Open the text font
    const sf::Font font("resources/tuffy.ttf");

    // Display for currently selected joystick - a row of numbers along the top
    // Red if disconnected, green if connected, white box for current selection
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
            if (sf::Joystick::isConnected(i))
            {
                indexIndicators[i].setFillColor(sf::Color::Green);
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
            else if (event->is<sf::Event::JoystickConnected>() || event->is<sf::Event::JoystickDisconnected>())
            {
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
            else if (event->is<sf::Event::JoystickButtonPressed>() || event->is<sf::Event::JoystickButtonReleased>() ||
                     event->is<sf::Event::JoystickMoved>())
            {
                gamepadDisplay.handleEvent(event);
                genericDisplay.handleEvent(event);
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
