
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
};

using Texts = std::unordered_map<std::string, JoystickObject>;
Texts              texts;
std::ostringstream sstr;
float              threshold = 0.1f;

// Axes labels in as strings
const std::array<std::string, 8> axislabels = {"X", "Y", "Z", "R", "U", "V", "PovX", "PovY"};

// Helper to set text entries to a specified value
template <typename T>
void set(const std::string& label, const T& value)
{
    sstr.str("");
    sstr << value;
    texts.at(label).value.setString(sstr.str());
}

// Update joystick identification
void updateIdentification(unsigned int index)
{
    sstr.str("");
    sstr << "Joystick " << index << ":";
    auto& [label, value] = texts.at("ID");
    label.setString(sstr.str());
    value.setString(sf::Joystick::getIdentification(index).name);
}

// Update joystick axes
void updateAxes(unsigned int index)
{
    for (unsigned int j = 0; j < sf::Joystick::AxisCount; ++j)
    {
        if (sf::Joystick::hasAxis(index, static_cast<sf::Joystick::Axis>(j)))
            set(axislabels[j], sf::Joystick::getAxisPosition(index, static_cast<sf::Joystick::Axis>(j)));
    }
}

// Update joystick buttons
void updateButtons(unsigned int index)
{
    for (unsigned int j = 0; j < sf::Joystick::getButtonCount(index); ++j)
    {
        sstr.str("");
        sstr << "Button " << j;

        set(sstr.str(), sf::Joystick::isButtonPressed(index, j));
    }
}

// Helper to update displayed joystick values
void updateValues(unsigned int index)
{
    if (sf::Joystick::isConnected(index))
    {
        // Update the label-value sf::Text objects based on the current joystick state
        updateIdentification(index);
        updateAxes(index);
        updateButtons(index);
    }
}
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
    sf::RenderWindow window(sf::VideoMode({400, 775}), "Joystick", sf::Style::Close);
    window.setVerticalSyncEnabled(true);

    // Load the text font
    sf::Font font;
    if (!font.loadFromFile("resources/tuffy.ttf"))
        return EXIT_FAILURE;

    // Set up our string conversion parameters
    sstr.precision(2);
    sstr.setf(std::ios::fixed | std::ios::boolalpha);

    // Set up our joystick identification sf::Text objects
    {
        auto [it, success]   = texts.emplace("ID", JoystickObject{{font, "<Not Connected>"}, {font}});
        auto& [label, value] = it->second;
        label.setPosition({5.f, 5.f});
        value.setPosition({80.f, 5.f});
    }

    // Set up our threshold sf::Text objects
    sstr.str("");
    sstr << threshold << "  (Change with up/down arrow keys)";
    {
        auto [it, success]   = texts.emplace("Threshold", JoystickObject{{font, "Threshold:"}, {font, sstr.str()}});
        auto& [label, value] = it->second;
        label.setPosition({5.f, 5.f + 2 * font.getLineSpacing(14)});
        value.setPosition({80.f, 5.f + 2 * font.getLineSpacing(14)});
    }

    // Set up our label-value sf::Text objects
    for (unsigned int i = 0; i < sf::Joystick::AxisCount; ++i)
    {
        auto [it, success]   = texts.emplace(axislabels[i], JoystickObject{{font, axislabels[i] + ":"}, {font, "N/A"}});
        auto& [label, value] = it->second;
        label.setPosition({5.f, 5.f + (static_cast<float>(i + 4) * font.getLineSpacing(14))});
        value.setPosition({80.f, 5.f + (static_cast<float>(i + 4) * font.getLineSpacing(14))});
    }

    for (unsigned int i = 0; i < sf::Joystick::ButtonCount; ++i)
    {
        sstr.str("");
        sstr << "Button " << i;
        auto [it, success]   = texts.emplace(sstr.str(), JoystickObject{{font, sstr.str() + ":"}, {font, "N/A"}});
        auto& [label, value] = it->second;
        label.setPosition({5.f, 5.f + (static_cast<float>(sf::Joystick::AxisCount + i + 4) * font.getLineSpacing(14))});
        value.setPosition({80.f, 5.f + (static_cast<float>(sf::Joystick::AxisCount + i + 4) * font.getLineSpacing(14))});
    }

    for (auto& [label, joystickObject] : texts)
    {
        joystickObject.label.setCharacterSize(14);
        joystickObject.value.setCharacterSize(14);
    }

    // Update initially displayed joystick values if a joystick is already connected on startup
    for (unsigned int i = 0; i < sf::Joystick::Count; ++i)
    {
        if (sf::Joystick::isConnected(i))
        {
            updateValues(i);
            break;
        }
    }

    while (window.isOpen())
    {
        // Handle events
        for (sf::Event event; window.pollEvent(event);)
        {
            // Window closed or escape key pressed: exit
            if ((event.type == sf::Event::Closed) ||
                ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape)))
            {
                window.close();
                break;
            }
            else if ((event.type == sf::Event::JoystickButtonPressed) || (event.type == sf::Event::JoystickButtonReleased) ||
                     (event.type == sf::Event::JoystickMoved) || (event.type == sf::Event::JoystickConnected))
            {
                // Update displayed joystick values
                updateValues(event.joystickConnect.joystickId);
            }
            else if (event.type == sf::Event::JoystickDisconnected)
            {
                // Reset displayed joystick values to empty
                for (auto& [label, joystickObject] : texts)
                    joystickObject.value.setString("N/A");

                auto& [label, value] = texts.at("ID");
                label.setString("<Not Connected>");
                value.setString("");

                sstr.str("");
                sstr << threshold << "  (Change with up/down arrow keys)";

                texts.at("Threshold").value.setString(sstr.str());
            }
        }

        // Update threshold if the user wants to change it
        float newThreshold = threshold;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            newThreshold += 0.1f;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            newThreshold -= 0.1f;

        newThreshold = std::clamp(newThreshold, 0.1f, 100.0f);

        if (newThreshold != threshold)
        {
            threshold = newThreshold;
            window.setJoystickThreshold(threshold);

            sstr.str("");
            sstr << threshold << "  (Change with up/down arrow keys)";

            texts.at("Threshold").value.setString(sstr.str());
        }

        // Clear the window
        window.clear();

        // Draw the label-value sf::Text objects
        for (const auto& [label, joystickObject] : texts)
        {
            window.draw(joystickObject.label);
            window.draw(joystickObject.value);
        }

        // Display things on screen
        window.display();
    }
}
