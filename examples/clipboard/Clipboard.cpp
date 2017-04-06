
////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Window/Clipboard.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>


////////////////////////////////////////////////////////////
/// Entry point of application
///
/// \return Application exit code
///
////////////////////////////////////////////////////////////
int main()
{
    // Create the window of the application
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Clipboard");

    // Load the font file
    sf::Font font;

    if(!font.loadFromFile("resources/sansation.ttf"))
    {
        std::cerr << "Failed to load font file." << std::endl;
        return EXIT_FAILURE;
    }

    // Setup strings for later use
    sf::String str_a = "Hello World!";
    sf::String str_b = "Bonjour le Monde!";

    sf::String tut_str = "Press 1 to copy \"" + str_a + "\" to clipboard\n";
    tut_str += "Press 2 to copy \"" + str_b + "\" to clipboard\n";
    tut_str += "Press Ctrl+V to display clipboard text below!";

    // Create our text objects to be displayed
    sf::Text tutorial(tut_str, font);
    tutorial.setFillColor(sf::Color::Cyan);

    sf::Text text("[Pasted Text From Clipboard]", font, 42);
    text.move(0, 300);


    while(window.isOpen())
    {
        // Handle events
        sf::Event event;
        while(window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed)
                window.close();
            if(event.type == sf::Event::KeyPressed)
            {
                // If [Ctrl+V] was pressed then get the clipboard string
                if(event.key.control && event.key.code == sf::Keyboard::V)
                    text.setString(sf::Clipboard::getString());
                // If Number 1 or 2 is pressed set the keyboard text
                else if(event.key.code == sf::Keyboard::Num1)
                    sf::Clipboard::setString(str_a);
                else if(event.key.code == sf::Keyboard::Num2)
                    sf::Clipboard::setString(str_b);
            }
        }

        // Clear the window
        window.clear();

        // Draw our text
        window.draw(tutorial);
        window.draw(text);

        // Display it to the window
        window.display();
    }

    return EXIT_SUCCESS;
}