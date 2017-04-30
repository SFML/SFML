////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <algorithm>

////////////////////////////////////////////////////////////
/// Entry point of application
///
/// \return Application exit code
///
////////////////////////////////////////////////////////////
int main()
{
    // Create the main window
    sf::RenderWindow window(sf::VideoMode(640, 480), "Drop image files here!");

    // If the platform doesn't support file dropping, abort
    if (!window.setFileDroppingEnabled(true))
    {
        std::cerr << "Platform not supported!\n";
        return EXIT_FAILURE;
    }

    // The texture for displaying dropped images
    sf::Texture texture;
    texture.setSmooth(true);

    // The sprite to draw the texture with
    sf::Sprite sprite;
    sprite.setTexture(texture);

    // Start the game loop
    while (window.isOpen())
    {
        // If this flag is set, we have to realign the view and the sprite
        bool realign = false;

        // Process events
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Close event: close the window
            if (event.type == sf::Event::Closed)
                window.close();

            // Resize event: request realignment
            if (event.type == sf::Event::Resized)
                realign = true;

            // Files dropped event: try to load as image, set the title, and request realignment
            if (event.type == sf::Event::FilesDropped)
            {
                for (unsigned int i = 0; i < event.droppedFiles.count; ++i)
                {
                    if (texture.loadFromFile(event.droppedFiles.files[i]))
                    {
                        window.setTitle(event.droppedFiles.files[i]);
                        realign = true;
                        break;
                    }
                }
            }
        }

        // Align the sprite tidily into the center of the window and reset the view
        if (realign)
        {
            window.setView(sf::View(sf::FloatRect(0.f, 0.f, static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y))));
            sprite.setTextureRect(sf::IntRect(0, 0, texture.getSize().x, texture.getSize().y));
            sprite.setOrigin(texture.getSize().x / 2.f, texture.getSize().y / 2.f);
            sprite.setPosition(window.getSize().x / 2.f, window.getSize().y / 2.f);
            float scalingFactor = 1.f;
            if (texture.getSize().x > window.getSize().x || texture.getSize().y > window.getSize().y)
            {
                scalingFactor = std::min(
                    static_cast<float>(window.getSize().x) / static_cast<float>(texture.getSize().x),
                    static_cast<float>(window.getSize().y) / static_cast<float>(texture.getSize().y));
            }
            sprite.setScale(scalingFactor, scalingFactor);
        }

        // Display the image
        window.clear();
        window.draw(sprite);
        window.display();
    }

    return EXIT_SUCCESS;
}
