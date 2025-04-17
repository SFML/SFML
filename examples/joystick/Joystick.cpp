////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics.hpp>

#include <algorithm>
#include <array>
#include <sstream>
#include <string>
#include <unordered_map>


namespace
{
struct JoystickObject
{
    sf::Text label;
    sf::Text value;

    void draw(sf::RenderWindow& window, const sf::RenderStates& states = sf::RenderStates::Default) const
    {
        window.draw(label, states);
        window.draw(value, states);
    }
};

using Texts = std::unordered_map<std::string, JoystickObject>;

auto getStream()
{
    std::ostringstream sstr;
    // Set up our string conversion parameters
    sstr.precision(2);
    sstr.setf(std::ios::fixed | std::ios::boolalpha);
    return sstr;
}

// Axes labels in as strings
const std::array<std::string, 8> axislabels = {"X", "Y", "Z", "R", "U", "V", "PovX", "PovY"};

struct JoystickDisplay
{
    unsigned int index{};
    bool         present{};
    Texts        texts;

    JoystickDisplay() = default;

    JoystickDisplay(unsigned int idx, const sf::Font& font)
    {
        index = idx;

        {
            const auto [it, success] = texts.try_emplace("ID", JoystickObject{{font, "<Not Connected>"}, {font}});
            auto& [label, value]     = it->second;
            label.setPosition({5.f, 5.f + 2 * font.getLineSpacing(14)});
            value.setPosition({80.f, 5.f + 2 * font.getLineSpacing(14)});
        }

        for (unsigned int i = 0; i < sf::Joystick::AxisCount; ++i)
        {
            const auto [it, success] = texts.try_emplace(axislabels[i],
                                                         JoystickObject{{font, axislabels[i] + ":"}, {font, "N/A"}});
            auto& [label, value]     = it->second;
            label.setPosition({5.f, 5.f + (static_cast<float>(i + 4) * font.getLineSpacing(14))});
            value.setPosition({80.f, 5.f + (static_cast<float>(i + 4) * font.getLineSpacing(14))});
        }

        auto sstr = getStream();
        for (unsigned int i = 0; i < sf::Joystick::ButtonCount; ++i)
        {
            sstr.str("");
            sstr << "Button " << i;
            const auto [it, success] = texts.try_emplace(sstr.str(),
                                                         JoystickObject{{font, sstr.str() + ":"}, {font, "N/A"}});
            auto& [label, value]     = it->second;
            label.setPosition({5.f, 5.f + (static_cast<float>(sf::Joystick::AxisCount + i + 4) * font.getLineSpacing(14))});
            value.setPosition({80.f, 5.f + (static_cast<float>(sf::Joystick::AxisCount + i + 4) * font.getLineSpacing(14))});
        }

        for (auto& [label, joystickObject] : texts)
        {
            joystickObject.label.setCharacterSize(14);
            joystickObject.value.setCharacterSize(14);
        }
    }

    // Helper to set text entries to a specified value
    template <typename T>
    void set(const std::string& label, const T& value, std::ostringstream& sstr)
    {
        sstr.str("");
        sstr << value;
        texts.at(label).value.setString(sstr.str());
    }

    // Update joystick identification
    void updateIdentification()
    {
        auto sstr = getStream();
        sstr << "Joystick " << index << ":";
        auto& [label, value] = texts.at("ID");
        label.setString(sstr.str());
        value.setString(sf::Joystick::getIdentification(index).name);
    }

    // Update joystick axes
    void updateAxes()
    {
        auto sstr = getStream();
        for (unsigned int j = 0; j < sf::Joystick::AxisCount; ++j)
        {
            if (sf::Joystick::hasAxis(index, static_cast<sf::Joystick::Axis>(j)))
                set(axislabels[j], sf::Joystick::getAxisPosition(index, static_cast<sf::Joystick::Axis>(j)), sstr);
        }
    }

    // Update joystick buttons
    void updateButtons()
    {
        auto sstr = getStream();
        for (unsigned int j = 0; j < sf::Joystick::getButtonCount(index); ++j)
        {
            sstr.str("");
            sstr << "Button " << j;

            auto buttonName = sstr.str();
            set(buttonName, sf::Joystick::isButtonPressed(index, j), sstr);
        }
    }

    void updateValues()
    {
        present = sf::Joystick::isConnected(index);
        if (present)
        {
            // Update the label-value sf::Text objects based on the current joystick state
            updateIdentification();
            updateAxes();
            updateButtons();
        }
    }

    void clearValues()
    {
        present = false;

        // Reset displayed joystick values to empty
        for (auto& [label, joystickObject] : texts)
            joystickObject.value.setString("N/A");

        auto& [label, value] = texts.at("ID");
        label.setString("<Not Connected>");
        value.setString("");
    }

    void draw(sf::RenderWindow& window, const sf::RenderStates& states = sf::RenderStates::Default) const
    {
        for (const auto& [_, joystickObject] : texts)
        {
            joystickObject.draw(window, states);
        }
    }
};
std::array<JoystickDisplay, sf::Joystick::Count> joysticks;
} // namespace


////////////////////////////////////////////////////////////
/// Entry point of application
///
/// \return Application exit code
///
////////////////////////////////////////////////////////////
int main()
{
    // Open the text font
    const sf::Font font("resources/tuffy.ttf");

    // Update initially displayed joystick values if a joystick is already connected on startup
    unsigned int presentCount = 0;
    for (unsigned int i = 0; i < sf::Joystick::Count; ++i)
    {
        joysticks[i] = JoystickDisplay{i, font};
        joysticks[i].updateValues();
        if (joysticks[i].present)
        {
            presentCount++;
        }
    }

    // Create the window of the application
    sf::RenderWindow window(sf::VideoMode({400 * std::max(presentCount, 1u), 775}), "Joystick", sf::Style::Close);
    window.setVerticalSyncEnabled(true);

    float          threshold = 0.1f;
    JoystickObject thresholdDisplay{{font, "Threshold:"}, {font, ""}};
    thresholdDisplay.label.setPosition({5.f, 5.f});
    thresholdDisplay.value.setPosition({80.f, 5.f});
    thresholdDisplay.label.setCharacterSize(14);
    thresholdDisplay.value.setCharacterSize(14);

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
            else if (const auto* joystickButtonPressed = event->getIf<sf::Event::JoystickButtonPressed>())
            {
                joysticks[joystickButtonPressed->joystickId].updateValues();
            }
            else if (const auto* joystickButtonReleased = event->getIf<sf::Event::JoystickButtonReleased>())
            {
                joysticks[joystickButtonReleased->joystickId].updateValues();
            }
            else if (const auto* joystickMoved = event->getIf<sf::Event::JoystickMoved>())
            {
                joysticks[joystickMoved->joystickId].updateValues();
            }
            else if (const auto* joystickConnected = event->getIf<sf::Event::JoystickConnected>())
            {
                joysticks[joystickConnected->joystickId].updateValues();
            }
            else if (const auto* joystickDisconnected = event->getIf<sf::Event::JoystickDisconnected>())
            {
                joysticks[joystickDisconnected->joystickId].clearValues();
            }
        }

        // Update threshold if the user wants to change it
        float newThreshold = threshold;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
            newThreshold += 0.1f;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
            newThreshold -= 0.1f;

        newThreshold = std::clamp(newThreshold, 0.1f, 100.0f);

        // Update the threshold if it has changed, or if this is the first time
        if (newThreshold != threshold || thresholdDisplay.value.getString().isEmpty())
        {
            threshold = newThreshold;
            window.setJoystickThreshold(threshold);

            auto sstr = getStream();
            sstr << threshold << "  (Change with up/down arrow keys)";

            thresholdDisplay.value.setString(sstr.str());
        }

        // Clear the window
        window.clear();

        // Display the current threshold
        thresholdDisplay.draw(window);

        unsigned int  newPresent = 0;
        sf::Transform transform  = sf::Transform::Identity;
        for (auto& joy : joysticks)
        {
            if (joy.present)
            {
                // Draw the values from this joystick
                joy.draw(window, transform);
                // Move the transform to the right for the next joystick
                transform.translate(sf::Vector2f{400, 0});
                // Keep track of the number of joysticks that we found
                newPresent++;
            }
        }

        // Did the number of joysticks change?
        if (newPresent != presentCount)
        {
            // Yep, resize the window to take the new number of joysticks into account
            presentCount = newPresent;
            window.setSize({400 * std::max(presentCount, 1u), 775});
        }

        // If no joystick is present, draw the first one
        if (presentCount == 0)
        {
            joysticks[0].draw(window);
        }

        // Display things on screen
        window.display();
    }
}
