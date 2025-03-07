////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics.hpp>

#include <vector>


////////////////////////////////////////////////////////////
/// Entry point of application
///
/// \return Application exit code
///
////////////////////////////////////////////////////////////
int main()
{
    // Create the main window
    sf::RenderWindow window(sf::VideoMode({800u, 600u}), "SFML Raw Mouse Input", sf::Style::Titlebar | sf::Style::Close);
    window.setVerticalSyncEnabled(true);

    // Open the application font
    const sf::Font font("resources/tuffy.ttf");

    // Create the mouse position text
    sf::Text mousePosition(font, "", 20);
    mousePosition.setPosition({400.f, 300.f});
    mousePosition.setFillColor(sf::Color::White);

    // Create the mouse raw movement text
    sf::Text mouseRawMovement(font, "", 20);
    mouseRawMovement.setFillColor(sf::Color::White);

    std::vector<std::string> log;

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
                break;
            }

            static const auto vec2ToString = [](const sf::Vector2i vec2)
            { return '(' + std::to_string(vec2.x) + ", " + std::to_string(vec2.y) + ')'; };

            if (const auto* const mouseMoved = event->getIf<sf::Event::MouseMoved>())
                mousePosition.setString("Mouse Position: " + vec2ToString(mouseMoved->position));

            if (const auto* const mouseMovedRaw = event->getIf<sf::Event::MouseMovedRaw>())
            {
                log.emplace_back("Mouse Movement: " + vec2ToString(mouseMovedRaw->delta));

                if (log.size() > 24u)
                    log.erase(log.begin());
            }
        }

        window.clear();
        window.draw(mousePosition);

        for (std::size_t i = 0; i < log.size(); ++i)
        {
            mouseRawMovement.setPosition({50.f, static_cast<float>(i * 20) + 50.f});
            mouseRawMovement.setString(log[i]);
            window.draw(mouseRawMovement);
        }

        window.display();
    }
}
