
////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>

////////////////////////////////////////////////////////////
/// Entry point of application
///
/// \return Application exit code
///
////////////////////////////////////////////////////////////
int main()
{
    // Check that the system can use shaders
    if (sf::Shader::IsAvailable() == false)
    {
        std::cerr << "Sorry, your system does not support shaders.";
        return EXIT_FAILURE;
    }

    // Create the main window
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Wobbling sprites");

    // Load a background texture to display
    sf::Texture backgroundTexture;
    if (!backgroundTexture.LoadFromFile("resources/background.jpg"))
        return EXIT_FAILURE;
    sf::Sprite background(backgroundTexture);

    // Load a sprite which we'll move into the scene
    sf::Texture entityTexture;
    if (!entityTexture.LoadFromFile("resources/sprite.png"))
        return EXIT_FAILURE;
    sf::Sprite entity(entityTexture);

    // Load shaders
    sf::Shader wobblingShader;
    wobblingShader.LoadFromFile("resources/fragment.sfx", "resources/vertex.sfx");
    wobblingShader.SetCurrentTexture("texture");
    wobblingShader.SetParameter("motion", 0, 0);
    
    // Animate the place
    float entityX = 300;
    float entityY = 200;
    entity.SetPosition(entityX, entityY);

    // Create a clock to measure the total time elapsed
    sf::Clock clock;

    // Start the game loop
    while (window.IsOpened())
    {
        // Process events
        sf::Event event;
        while (window.PollEvent(event))
        {
            // Close window : exit
            if (event.Type == sf::Event::Closed)
                window.Close();

            if (event.Type == sf::Event::KeyPressed)
            {
                // Escape key : exit
                if (event.Key.Code == sf::Keyboard::Escape)
                    window.Close();
            }
        }
        
        if(sf::Mouse::IsButtonPressed(sf::Mouse::Left))
        {
            sf::Vector2i mousePosition = sf::Mouse::GetPosition(window);
            
            sf::Vector2f movement(0.005 * (mousePosition.x - entity.GetPosition().x) * clock.GetElapsedTime(), \
                0.005 * (mousePosition.y - entity.GetPosition().y) * clock.GetElapsedTime());
            entity.Move(movement);
            wobblingShader.SetParameter("motion", movement.x / 100, movement.y / 100);
        }
        
        clock.Reset();

        // Draw the sprites
        window.Clear();
        window.Draw(background);
        window.Draw(entity, wobblingShader);
        window.Display();
    }

    return EXIT_SUCCESS;
}

