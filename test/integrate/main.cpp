#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

int main()
{
    sf::RenderWindow window(sf::VideoMode({800, 600}), "SFML window");

    const sf::Texture texture("cute_image.jpg");
    sf::Sprite sprite(texture);

    const sf::Font font("arial.ttf");
    sf::Text text(font, "Hello SFML", 50);

    sf::Music music("nice_music.ogg");
    music.play();

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        window.clear();
        window.draw(sprite);
        window.draw(text);
        window.display();
    }
}
