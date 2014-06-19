#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>


int main(int argc, char *argv[])
{
    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "");

    sf::Texture texture;
    if(!texture.loadFromFile("image.png"))
        return EXIT_FAILURE;

    sf::Sprite image(texture);
    image.setPosition(0, 0);
    image.setOrigin(texture.getSize().x/2, texture.getSize().y/2);

    sf::Music music;
    if(!music.openFromFile("canary.wav"))
        return EXIT_FAILURE;

    music.play();

    while (window.isOpen())
    {
        sf::Event event;

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }

        if (sf::Touch::isDown(0))
        {
            sf::Vector2i position = sf::Touch::getPosition(0);
            image.setPosition(position.x, position.y);
        }

        window.clear(sf::Color::White);
       	window.draw(image);
        window.display();
    }
}
