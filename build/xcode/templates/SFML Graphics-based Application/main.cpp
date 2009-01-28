
////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics.hpp>


////////////////////////////////////////////////////////////
/// Entry point of application
///
/// \return Application exit code
///
////////////////////////////////////////////////////////////
int main()
{
    // Create main window
    sf::RenderWindow App(sf::VideoMode(640, 480), "SFML Graphics");

    // Start game loop
    while (App.IsOpened())
    {
        // Process events
        sf::Event Event;
        while (App.GetEvent(Event))
        {
            // Close window : exit
            if (Event.Type == sf::Event::Closed)
                App.Close();
        }

        // Clear screen
        App.Clear();

        // Draw apredefined shape
        App.Draw(sf::Shape::Circle(200, 200, 100, sf::Color::Yellow, 10, sf::Color::Blue));

        // Finally, display the rendered frame on screen
        App.Display();
    }

    return EXIT_SUCCESS;
}
