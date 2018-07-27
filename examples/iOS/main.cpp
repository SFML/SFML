////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Main.hpp>


////////////////////////////////////////////////////////////
/// Entry point of application
///
/// \return Application exit code
///
////////////////////////////////////////////////////////////
int main()
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
    
    sf::View view = window.getDefaultView();
    
    while (window.isOpen())
    {
        sf::Event event;
        
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
                case sf::Event::Closed:
                    window.close();
                    break;
                case sf::Event::Resized:
                    view.setSize(event.size.width, event.size.height);
                    view.setCenter(event.size.width/2, event.size.height/2);
                    window.setView(view);
                    break;
                case sf::Event::TouchBegan:
                    if (event.touch.finger == 0)
                    {
                        image.setPosition(event.touch.x, event.touch.y);
                    }
                    break;
            }
        }
        
        window.clear(sf::Color::White);
        window.draw(image);
        window.display();
    }
}

