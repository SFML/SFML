
////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics.hpp>
#include <algorithm>
#include <sstream>


////////////////////////////////////////////////////////////
/// Entry point of application
///
/// \return Application exit code
///
////////////////////////////////////////////////////////////
sf::RenderWindow *createRenderWindw(bool isFullscreen, sf::VideoMode mode, const sf::Image &icon)
{
	long flag = sf::Style::Fullscreen;
	
	if(!isFullscreen) // Not in full screen -> window is half the size of the screen
	{
		mode.width /= 2;
		mode.height /= 2;
		
		flag = sf::Style::Default;
	}
	
    sf::RenderWindow *window = new sf::RenderWindow(mode, "SFML Fullscreen", flag);
    window->setVerticalSyncEnabled(false);
    window->setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
    window->setTitle("AAAAA");
    
    return window;
}

int main()
{
	// Desktop size
	sf::VideoMode mode = sf::VideoMode::getDesktopMode();
	bool isFullscreen = true;
	
    // Icon & image
    sf::Image image;
    if(! image.loadFromFile("resources/icon.png"))
		return EXIT_FAILURE;

    // Load the text font
    sf::Font font;
    if (!font.loadFromFile("resources/sansation.ttf"))
        return EXIT_FAILURE;
	
    // Create the window of the application
    sf::RenderWindow *window = createRenderWindw(isFullscreen, mode, image);

    // Define some constants
    const float ballRadius = 20.f;
    bool hasFocus = true;

    // Create the key ball
    sf::CircleShape keyBall;
    keyBall.setRadius(ballRadius - 3);
    keyBall.setOutlineThickness(3);
    keyBall.setOutlineColor(sf::Color::Black);
    keyBall.setFillColor(sf::Color::Blue);
    keyBall.setOrigin(ballRadius / 2.f, ballRadius / 2.f);
    keyBall.setPosition(sf::Vector2f(window->getSize()) / 2.f);
    
    // Create the mouse ball
    sf::CircleShape mouseBall;
    mouseBall.setRadius(ballRadius - 3);
    mouseBall.setOutlineThickness(3);
    mouseBall.setOutlineColor(sf::Color::Black);
    mouseBall.setFillColor(sf::Color::Red);
    mouseBall.setOrigin(ballRadius / 2.f, ballRadius / 2.f);
    mouseBall.setPosition(sf::Vector2f(window->getSize()) / 2.f);

    // Initialize the pause message
    sf::Text msg;
    msg.setFont(font);
    msg.setCharacterSize(15);
    msg.setPosition(20.f, 100.f);
    msg.setColor(sf::Color::White);
    msg.setString("This example is used to show the new Window Manager based fullscreen mode\nAs you can see, keyboard events works (you can move the ball using the direction keys).\nThe mouse events work too.\n\nYou can also alt-tab the application, change the volume using your hot / special keys,\n   close the window using ALT+F4, switch desktop...\n\nPress <Escape> to quit or <Tab> to switch to fullscreen or windowed mode.");
    
    // FPS
    sf::Text fpsText;
    fpsText.setFont(font);
    fpsText.setCharacterSize(20);
    fpsText.setPosition(5.f, 5.f);
    fpsText.setColor(sf::Color::White);
    fpsText.setString("...");

    // Define the paddles properties
    sf::Clock AITimer;
    const float speed   = 200.f;
    
    // FPS
    int frameCount = 0;
    float time = 0.f;

    sf::Clock clock;
    while (window->isOpen())
    {
        // Handle events
        sf::Event event;
        while (window->pollEvent(event))
        {
            // Window closed or escape key pressed: exit
            if ((event.type == sf::Event::Closed) || 
               ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape)))
            {
                window->close();
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
			else if((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Tab))
			{
				isFullscreen = !isFullscreen;
				
				delete window;
				window = createRenderWindw(isFullscreen, mode, image);
				
				// Move objects
				keyBall.setPosition(sf::Vector2f(window->getSize()) / 2.f);
				mouseBall.setPosition(sf::Vector2f(window->getSize()) / 2.f);
			}
        }

		// Frame time
		float deltaTime = clock.restart().asSeconds();
		
		// FPS - We calculate the current frame rate every 2 seconds to have readable results
		frameCount++;
		time += deltaTime;
		
		if(time > 2.f)
		{
			std::stringstream strstrm;
			strstrm << frameCount / time;
			fpsText.setString(strstrm.str());
			
			time = 0;
			frameCount = 0;
		}

		// Key ball
		if(hasFocus)
		{
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
			pos.x = std::min(pos.x, float(window->getSize().x));
			
			pos.y = std::max(pos.y, 0.f);
			pos.y = std::min(pos.y, float(window->getSize().y));
			
			keyBall.setPosition(pos);
			
			// Mouse ball
			sf::Vector2i mousePos = sf::Mouse::getPosition(*window);
			mouseBall.setPosition(mousePos.x, mousePos.y);
		}

        // Clear the window
        window->clear(sf::Color(50, 200, 50));
        
		// Draw the paddles and the ball
		window->draw(keyBall);
		window->draw(mouseBall);
		window->draw(msg);
		window->draw(fpsText);

        // Display things on screen
        window->display();
    }
    
    delete window;

    return EXIT_SUCCESS;
}
