////////////////////////////////////////////////////////////
// Imports
////////////////////////////////////////////////////////////
import sfml.graphics;
import sfml.window;

////////////////////////////////////////////////////////////
/// Entry point of application
///
/// \return Application exit code
///
////////////////////////////////////////////////////////////
int main()
{
    sf::RenderWindow window(sf::VideoMode({1280, 720}), "Minimal, complete and verifiable example");
    window.setFramerateLimit(60);

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        window.clear();
        window.display();
    }
}