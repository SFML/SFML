////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics.hpp>

#include <string>

namespace
{
// Axes labels in as C strings
const std::string axisLabels[] = {"X", "Y", "Z", "R", "U", "V", "PovX", "PovY"};

// Display for triggers
class TriggerDisplay : public sf::Drawable, public sf::Transformable
{
public:
    TriggerDisplay() = delete;
    TriggerDisplay(const sf::Font& font, sf::Joystick::Axis axis) : m_label(font), m_axis(axis)
    {
        m_bounds.setFillColor(sf::Color::Transparent);
        m_bounds.setOutlineColor(sf::Color::White);
        m_bounds.setOutlineThickness(1);
        m_bounds.setOrigin({m_bounds.getLocalBounds().size.x / 2, 0});

        m_indicator.setFillColor(sf::Color::Red);
        m_indicator.setOrigin({m_indicator.getLocalBounds().size.x / 2, 0});

        m_label.setCharacterSize(int(m_bounds.getSize().x));
        m_label.setPosition({0, m_bounds.getSize().y / 2});
        m_label.setString(axisLabels[int(axis)]);
        m_label.setOrigin({m_label.getLocalBounds().size / 2.f});
    }

    void setValue(float value)
    {
        // We adjust the range from [-100,100] to [0, height]
        m_indicator.setScale({1, (100 + value) * m_bounds.getLocalBounds().size.y / 200});
    }

    sf::Joystick::Axis getAxis()
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
        m_label(font),
        m_button(button)
    {
        m_bounds.setOrigin(m_bounds.getLocalBounds().size / 2.f); // Center origin for simplicity
        m_bounds.setFillColor(sf::Color::Transparent);
        m_bounds.setOutlineColor(sf::Color::White);
        m_bounds.setOutlineThickness(1);

        m_indicator.setFillColor(sf::Color::Red);
        m_indicator.setOrigin(m_indicator.getLocalBounds().size / 2.f);

        m_label.setString(axisLabels[int(xAxis)] + "/" + axisLabels[int(yAxis)]);
        m_label.setCharacterSize(20);
        m_label.setOrigin(m_label.getLocalBounds().size / 2.f + m_label.getLocalBounds().position);
    }

    void setX(float x)
    {
        xValue = x;
        updatePos();
    }

    void setY(float y)
    {
        yValue = y;
        updatePos();
    }

    sf::Joystick::Axis getXAxis()
    {
        return m_xAxis;
    }

    sf::Joystick::Axis getYAxis()
    {
        return m_yAxis;
    }

    void pressed(bool pressed)
    {
        m_bounds.setFillColor(pressed ? sf::Color::Red : sf::Color::Transparent);
    }

    std::optional<unsigned int> getButton()
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
        m_indicator.setPosition({xValue * radius / 100, yValue * radius / 100});
    }
    sf::Joystick::Axis          m_xAxis;
    sf::Joystick::Axis          m_yAxis;
    std::optional<unsigned int> m_button;
    sf::CircleShape             m_bounds{50.f};
    sf::CircleShape             m_indicator{5.f};
    sf::Text                    m_label;
    float                       xValue;
    float                       yValue;
};

// Display a button state
class ButtonDisplay : public sf::Drawable, public sf::Transformable
{
public:
    ButtonDisplay() = delete;
    ButtonDisplay(const sf::Font& font, int button) : m_label(font), m_button(button)
    {
        m_label.setString(std::to_string(button));
        m_label.setCharacterSize(int(m_indicator.getLocalBounds().size.y));
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

    unsigned int getButton()
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
    int             m_button;
    sf::CircleShape m_indicator{10.f};
};

constexpr sf::Vector2f displaySize{600, 300};

// Display a basic gamepad layout
class GamepadDisplay : public sf::Drawable, public sf::Transformable
{
public:
    GamepadDisplay(const sf::Font& font, int id) : m_label(font), m_id(id)
    {
        m_label.setString(std::to_string(id) + ": " + sf::Joystick::getIdentification(id).name);
        m_label.setCharacterSize(20);

        // Create the rough layout of a typical gamepad
        m_joysticks.emplace_back(font, sf::Joystick::Axis::X, sf::Joystick::Axis::Y, 10); // Left stick
        m_joysticks.back().setPosition({displaySize.x / 2.f - 100, displaySize.y - 50});
        m_joysticks.emplace_back(font, sf::Joystick::Axis::Z, sf::Joystick::Axis::R, 11); // Right stick
        m_joysticks.back().setPosition({displaySize.x / 2.f + 100, displaySize.y - 50});


        const sf::Vector2f mainButtonPos{displaySize.x / 2.f + 250, displaySize.y / 2.f - 50};
        const auto         offset = 20.f;
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

        m_outline.setFillColor(sf::Color::Transparent);
        m_outline.setOutlineColor(sf::Color::White);
        m_outline.setOutlineThickness(1);
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
        target.draw(m_outline, states);
        target.draw(m_label, states);
    }

    unsigned int getId()
    {
        return m_id;
    }

private:
    std::vector<JoystickDisplay> m_joysticks;
    std::vector<TriggerDisplay>  m_triggers;
    std::vector<ButtonDisplay>   m_buttons;
    sf::RectangleShape           m_outline{displaySize};
    sf::Text                     m_label;
    unsigned int                 m_id;
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
    constexpr sf::Vector2f windowSize{displaySize.x, displaySize.y * 2};
    sf::RenderWindow       window(sf::VideoMode(sf::Vector2u(windowSize)), "Joystick");
    window.setVerticalSyncEnabled(true);

    // Open the text font
    const sf::Font font("resources/tuffy.ttf");

    std::vector<GamepadDisplay> gamepads;
    for (auto i = 0; i < sf::Joystick::Count; i++)
    {
        if (sf::Joystick::isConnected(i))
        {
            gamepads.emplace_back(font, i);
        }
    }

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
            else if(const auto* connected = event->getIf<sf::Event::JoystickConnected>())
            {
                gamepads.emplace_back(font, connected->joystickId);
            }
            else if (const auto* disconnected = event->getIf<sf::Event::JoystickDisconnected>())
            {
                gamepads.erase(std::remove_if(gamepads.begin(), gamepads.end(), [disconnected](GamepadDisplay gamepad){
                    return gamepad.getId() == disconnected->joystickId;}));
            }

            for (auto& gamepad : gamepads)
            {
                gamepad.handleEvent(event);
            }
        }

        // Clear the window
        window.clear();

        sf::Transform t;
        for (const auto& gamepad : gamepads)
        {
            window.draw(gamepad, t);
            t.translate({0, displaySize.y});
        }

        // Display things on screen
        window.display();
    }
}
