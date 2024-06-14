//
// Disclaimer:
// ----------
//
// This code will work only if you selected window, graphics and audio.
//
// Note that the "Run Script" build phase will copy the required frameworks
// or dylibs to your application bundle so you can execute it on any OS X
// computer.
//
// Your resource files (images, sounds, fonts, ...) are also copied to your
// application bundle. To get the path to these resources, use the helper
// function `resourcePath()` from ResourcePath.hpp
//

#include <SFML/Graphics.hpp>

#include <SFML/Audio.hpp>

// Here is a small helper for you! Have a look.
#include "ResourcePath.hpp"

int main()
{
    // Create the main window
    sf::RenderWindow window(sf::VideoMode({800, 600}), "SFML window");

    // Set the Icon
    const auto icon = sf::Image::loadFromFile(resourcePath() / "icon.png").value();
    window.setIcon(icon);

    // Load a sprite to display
    const auto texture = sf::Texture::loadFromFile(resourcePath() / "background.jpg").value();
    sf::Sprite sprite(texture);

    // Create a graphical text to display
    const auto font = sf::Font::loadFromFile(resourcePath() / "tuffy.ttf").value();
    sf::Text   text(font, "Hello SFML", 50);
    text.setFillColor(sf::Color::Black);

    // Load a music to play
    sf::Music music;
    if (!music.openFromFile(resourcePath() / "doodle_pop.ogg"))
    {
        return EXIT_FAILURE;
    }

    // Play the music
    music.play();

    // Start the game loop
    while (window.isOpen())
    {
        // Process events
        while (const auto event = window.pollEvent())
        {
            // Window closed or escape key pressed: exit
            if (event.is<sf::Event::Closed>() || (event.is<sf::Event::KeyPressed>() &&
                                                  event.getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::Escape))
            {
                window.close();
                break;
            }
        }

        // Clear screen
        window.clear();

        // Draw the sprite
        window.draw(sprite);

        // Draw the string
        window.draw(text);

        // Update the window
        window.display();
    }
}
