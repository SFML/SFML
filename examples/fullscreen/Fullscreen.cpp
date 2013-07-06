
////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics.hpp>
#include <algorithm>


////////////////////////////////////////////////////////////
/// Entry point of application
///
/// \return Application exit code
///
////////////////////////////////////////////////////////////
int main()
{
	// Desktop size
	sf::VideoMode mode = sf::VideoMode::getDesktopMode();

    // Define some constants
    const float ballRadius = 20.f;
    bool hasFocus = true;

    // Create the window of the application
    sf::RenderWindow window(mode, "SFML Fullscreen", sf::Style::Fullscreen | sf::Style::FullscreenWM);
    window.setVerticalSyncEnabled(true);

    // Create the key ball
    sf::CircleShape keyBall;
    keyBall.setRadius(ballRadius - 3);
    keyBall.setOutlineThickness(3);
    keyBall.setOutlineColor(sf::Color::Black);
    keyBall.setFillColor(sf::Color::Blue);
    keyBall.setOrigin(ballRadius / 2.f, ballRadius / 2.f);
    keyBall.setPosition(mode.width / 2, mode.height / 2);
    
    // Create the mouse ball
    sf::CircleShape mouseBall;
    mouseBall.setRadius(ballRadius - 3);
    mouseBall.setOutlineThickness(3);
    mouseBall.setOutlineColor(sf::Color::Black);
    mouseBall.setFillColor(sf::Color::Red);
    mouseBall.setOrigin(ballRadius / 2.f, ballRadius / 2.f);
    mouseBall.setPosition(mode.width / 2, mode.height / 2);

    // Load the text font
    sf::Font font;
    if (!font.loadFromFile("resources/sansation.ttf"))
        return EXIT_FAILURE;

    // Initialize the pause message
    sf::Text msg;
    msg.setFont(font);
    msg.setCharacterSize(30);
    msg.setPosition(20.f, 100.f);
    msg.setColor(sf::Color::White);
    msg.setString("This example is used to show the new Window Manager based fullscreen mode\nAs you can see, keyboard events works (you can move the ball using the direction keys).\nThe mouse events work too.\n\nYou can also alt-tab the application, change the volume using your hot / special keys,\n   close the window using ALT+F4, switch desktop...");

    // Define the paddles properties
    sf::Clock AITimer;
    const float speed   = 200.f;

    sf::Clock clock;
    while (window.isOpen())
    {
        // Handle events
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Window closed or escape key pressed: exit
            if ((event.type == sf::Event::Closed) || 
               ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape)))
            {
                window.close();
                break;
            }
            else if(event.type == sf::Event::GainedFocus)
            {
				hasFocus = true;
			}
			else if(event.type == sf::Event::LostFocus)
			{
				hasFocus = false;
			}
        }

		// Key ball
		if(hasFocus)
		{
			float deltaTime = clock.restart().asSeconds();

			int xDirection = 0;
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
				xDirection = 1;
			else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
				xDirection = -1;
			
			int yDirection = 0;
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
				yDirection = 1;
			else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
				yDirection = -1;
			
			keyBall.move(speed * deltaTime * xDirection, speed * deltaTime * yDirection);
			
			sf::Vector2f pos = keyBall.getPosition();
			pos.x = std::max(pos.x, 0.f);
			pos.x = std::min(pos.x, float(mode.width));
			
			pos.y = std::max(pos.y, 0.f);
			pos.y = std::min(pos.y, float(mode.height));
			
			keyBall.setPosition(pos);
			
			// Mouse ball
			sf::Vector2i mousePos = sf::Mouse::getPosition();
			mouseBall.setPosition(mousePos.x, mousePos.y);
		}

        // Clear the window
        window.clear(sf::Color(50, 200, 50));

		// Draw the paddles and the ball
		window.draw(keyBall);
		window.draw(mouseBall);
		window.draw(msg);

        // Display things on screen
        window.display();
    }

    return EXIT_SUCCESS;
}
