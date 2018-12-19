
////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics.hpp>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <string>
#include <map>


namespace
{
    struct JoystickObject
    {
        sf::Text label;
        sf::Text value;
    };

    typedef std::map<std::string, JoystickObject> Texts;
    Texts texts;
    std::ostringstream sstr;
    float threshold = 0.1f;

    // Axes labels in as C strings
    const char* axislabels[] = {"X", "Y", "Z", "R", "U", "V", "PovX", "PovY"};

    // Helper to set text entries to a specified value
    template<typename T>
    void set(const char* label, const T& value)
    {
        sstr.str("");
        sstr << value;
        texts[label].value.setString(sstr.str());
    }

    // Update joystick identification
    void updateIdentification(unsigned int index)
    {
        sstr.str("");
        sstr << "Joystick " << index << ":";
        texts["ID"].label.setString(sstr.str());
        texts["ID"].value.setString(sf::Joystick::getIdentification(index).name);
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

            set(sstr.str().c_str(), sf::Joystick::isButtonPressed(index, j));
        }
    }

    // Helper to update displayed joystick values
    void updateValues(unsigned int index)
    {
        if (sf::Joystick::isConnected(index)) {
            // Update the label-value sf::Text objects based on the current joystick state
            updateIdentification(index);
            updateAxes(index);
            updateButtons(index);
        }
    }
}


////////////////////////////////////////////////////////////
/// Entry point of application
///
/// \return Application exit code
///
////////////////////////////////////////////////////////////
int main()
{
    // Create the window of the application
    sf::RenderWindow window(sf::VideoMode(400, 680), "Joystick", sf::Style::Close);
    window.setVerticalSyncEnabled(true);

    // Load the text font
    sf::Font font;
    if (!font.loadFromFile("resources/sansation.ttf"))
        return EXIT_FAILURE;

    // Set up our string conversion parameters
    sstr.precision(2);
    sstr.setf(std::ios::fixed | std::ios::boolalpha);

    // Set up our joystick identification sf::Text objects
    texts["ID"].label.setPosition(5.f, 5.f);
    texts["ID"].value.setPosition(80.f, 5.f);

    texts["ID"].label.setString("<Not Connected>");
    texts["ID"].value.setString("");

    // Set up our threshold sf::Text objects
    sstr.str("");
    sstr << threshold << "  (Change with up/down arrow keys)";

    texts["Threshold"].label.setPosition(5.f, 5.f + 2 * font.getLineSpacing(14));
    texts["Threshold"].value.setPosition(80.f, 5.f + 2 * font.getLineSpacing(14));

    texts["Threshold"].label.setString("Threshold:");
    texts["Threshold"].value.setString(sstr.str());

    // Set up our label-value sf::Text objects
    for (unsigned int i = 0; i < sf::Joystick::AxisCount; ++i)
    {
        JoystickObject& object = texts[axislabels[i]];

        object.label.setPosition(5.f, 5.f + ((i + 4) * font.getLineSpacing(14)));
        object.label.setString(std::string(axislabels[i]) + ":");

        object.value.setPosition(80.f, 5.f + ((i + 4) * font.getLineSpacing(14)));
        object.value.setString("N/A");
    }

    for (unsigned int i = 0; i < sf::Joystick::ButtonCount; ++i)
    {
        sstr.str("");
        sstr << "Button " << i;
        JoystickObject& object = texts[sstr.str()];

        object.label.setPosition(5.f, 5.f + ((sf::Joystick::AxisCount + i + 4) * font.getLineSpacing(14)));
        object.label.setString(sstr.str() + ":");

        object.value.setPosition(80.f, 5.f + ((sf::Joystick::AxisCount + i + 4) * font.getLineSpacing(14)));
        object.value.setString("N/A");
    }

    for (Texts::iterator it = texts.begin(); it != texts.end(); ++it)
    {
        it->second.label.setFont(font);
        it->second.label.setCharacterSize(14);
        it->second.label.setFillColor(sf::Color::White);

        it->second.value.setFont(font);
        it->second.value.setCharacterSize(14);
        it->second.value.setFillColor(sf::Color::White);
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
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Window closed or escape key pressed: exit
            if ((event.type == sf::Event::Closed) ||
               ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape)))
            {
                window.close();
                break;
            }
            else if ((event.type == sf::Event::JoystickButtonPressed) ||
                     (event.type == sf::Event::JoystickButtonReleased) ||
                     (event.type == sf::Event::JoystickMoved) ||
                     (event.type == sf::Event::JoystickConnected))
            {
                // Update displayed joystick values
                updateValues(event.joystickConnect.joystickId);
            }
            else if (event.type == sf::Event::JoystickDisconnected)
            {
                // Reset displayed joystick values to empty
                for (Texts::iterator it = texts.begin(); it != texts.end(); ++it)
                    it->second.value.setString("N/A");

                texts["ID"].label.setString("<Not Connected>");
                texts["ID"].value.setString("");

                sstr.str("");
                sstr << threshold << "  (Change with up/down arrow keys)";

                texts["Threshold"].value.setString(sstr.str());
            }
        }

        // Update threshold if the user wants to change it
        float newThreshold = threshold;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            newThreshold += 0.1f;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            newThreshold -= 0.1f;

        newThreshold = std::min(std::max(newThreshold, 0.1f), 100.0f);

        if (newThreshold != threshold)
        {
            threshold = newThreshold;
            window.setJoystickThreshold(threshold);

            sstr.str("");
            sstr << threshold << "  (Change with up/down arrow keys)";

            texts["Threshold"].value.setString(sstr.str());
        }

        // Clear the window
        window.clear();

        // Draw the label-value sf::Text objects
        for (Texts::const_iterator it = texts.begin(); it != texts.end(); ++it)
        {
            window.draw(it->second.label);
            window.draw(it->second.value);
        }

        // Display things on screen
        window.display();
    }
}
