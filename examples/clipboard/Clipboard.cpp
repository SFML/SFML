////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <iostream>
#include <SFML/Graphics.hpp>


////////////////////////////////////////////////////////////
/// Entry point of application
///
/// \return Application exit code
///
////////////////////////////////////////////////////////////
int main()
{
    // Load a font for text rendering
    sf::Font font;
    if(!font.loadFromFile("resources/sansation.ttf"))
        return EXIT_FAILURE;

    // Create a text object using the font we've just loaded
    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(16);
    text.setColor(sf::Color::White);
    text.setPosition(0.f, 0.f);

    // Create the main window
    sf::RenderWindow window(sf::VideoMode(640, 64), "SFML clipboard example");

    // Start the game loop
    while (window.isOpen())
    {

        // Process events
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Close window: exit
            if (event.type == sf::Event::Closed)
                window.close();

            // Escape key: exit
            if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape))
                window.close();

            // C key: set the clipboard to "SFMLSFMLSFML"
            if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::C))
                window.setClipboard("SFMLSFMLSFML");
        }

        // Update the text string with the clipboard's contents
        text.setString( "Current clipboard contents: \"" + window.getClipboard() + "\"\n" +
                        "Press the 'C' key to set the clipboard to \"SFMLSFMLSFML\"." );

        // Clear the window of previously drawn content
        window.clear();

        // Draw our text
        window.draw(text);

        // Finally, display the rendered frame on screen
        window.display();
    }

    return EXIT_SUCCESS;
}
