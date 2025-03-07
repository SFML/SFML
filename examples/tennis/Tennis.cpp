////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics.hpp>

#include <SFML/Audio.hpp>

#include <filesystem>
#include <random>
#include <string>

#include <cmath>
#include <cstdlib>

#ifdef SFML_SYSTEM_IOS
#include <SFML/Main.hpp>
#endif

namespace
{
std::filesystem::path resourcesDir()
{
#ifdef SFML_SYSTEM_IOS
    return "";
#else
    return "resources";
#endif
}
} // namespace

////////////////////////////////////////////////////////////
/// Entry point of application
///
/// \return Application exit code
///
////////////////////////////////////////////////////////////
int main()
{
    std::random_device rd;
    std::mt19937       rng(rd());

    // Define some constants
    const float        gameWidth  = 800;
    const float        gameHeight = 600;
    const sf::Vector2f paddleSize(25, 100);
    const float        ballRadius = 10.f;

    // Create the window of the application
    sf::RenderWindow window(sf::VideoMode({static_cast<unsigned int>(gameWidth), static_cast<unsigned int>(gameHeight)}, 32),
                            "SFML Tennis",
                            sf::Style::Titlebar | sf::Style::Close);
    window.setVerticalSyncEnabled(true);

    // Load the sounds used in the game
    const sf::SoundBuffer ballSoundBuffer(resourcesDir() / "ball.wav");
    sf::Sound             ballSound(ballSoundBuffer);

    // Create the SFML logo texture:
    const sf::Texture sfmlLogoTexture(resourcesDir() / "sfml_logo.png");
    sf::Sprite        sfmlLogo(sfmlLogoTexture);
    sfmlLogo.setPosition({170.f, 50.f});

    // Create the left paddle
    sf::RectangleShape leftPaddle;
    leftPaddle.setSize(paddleSize - sf::Vector2f(3, 3));
    leftPaddle.setOutlineThickness(3);
    leftPaddle.setOutlineColor(sf::Color::Black);
    leftPaddle.setFillColor(sf::Color(100, 100, 200));
    leftPaddle.setOrigin(paddleSize / 2.f);

    // Create the right paddle
    sf::RectangleShape rightPaddle;
    rightPaddle.setSize(paddleSize - sf::Vector2f(3, 3));
    rightPaddle.setOutlineThickness(3);
    rightPaddle.setOutlineColor(sf::Color::Black);
    rightPaddle.setFillColor(sf::Color(200, 100, 100));
    rightPaddle.setOrigin(paddleSize / 2.f);

    // Create the ball
    sf::CircleShape ball;
    ball.setRadius(ballRadius - 3);
    ball.setOutlineThickness(2);
    ball.setOutlineColor(sf::Color::Black);
    ball.setFillColor(sf::Color::White);
    ball.setOrigin({ballRadius / 2.f, ballRadius / 2.f});

    // Open the text font
    const sf::Font font(resourcesDir() / "tuffy.ttf");

    // Initialize the pause message
    sf::Text pauseMessage(font);
    pauseMessage.setCharacterSize(40);
    pauseMessage.setPosition({170.f, 200.f});
    pauseMessage.setFillColor(sf::Color::White);

#ifdef SFML_SYSTEM_IOS
    pauseMessage.setString("Welcome to SFML Tennis!\nTouch the screen to start the game.");
#else
    pauseMessage.setString("Welcome to SFML Tennis!\n\nPress space to start the game.");
#endif

    // Define the paddles properties
    sf::Clock      aiTimer;
    const sf::Time aiTime           = sf::seconds(0.1f);
    const float    paddleSpeed      = 400.f;
    float          rightPaddleSpeed = 0.f;
    const float    ballSpeed        = 400.f;
    sf::Angle      ballAngle        = sf::degrees(0); // to be changed later

    sf::Clock clock;
    bool      isPlaying = false;
    while (window.isOpen())
    {
        // Handle events
        while (const std::optional event = window.pollEvent())
        {
            // Window closed or escape key pressed: exit
            if (event->is<sf::Event::Closed>() ||
                (event->is<sf::Event::KeyPressed>() &&
                 event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::Escape))
            {
                window.close();
                break;
            }

            // Space key pressed: play
            if ((event->is<sf::Event::KeyPressed>() &&
                 event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::Space) ||
                event->is<sf::Event::TouchBegan>())
            {
                if (!isPlaying)
                {
                    // (re)start the game
                    isPlaying = true;
                    clock.restart();

                    // Reset the position of the paddles and ball
                    leftPaddle.setPosition({10.f + paddleSize.x / 2.f, gameHeight / 2.f});
                    rightPaddle.setPosition({gameWidth - 10.f - paddleSize.x / 2.f, gameHeight / 2.f});
                    ball.setPosition({gameWidth / 2.f, gameHeight / 2.f});

                    // Reset the ball angle
                    do
                    {
                        // Make sure the ball initial angle is not too much vertical
                        ballAngle = sf::degrees(std::uniform_real_distribution<float>(0, 360)(rng));
                    } while (std::abs(std::cos(ballAngle.asRadians())) < 0.7f);
                }
            }

            // Window size changed, adjust view appropriately
            if (event->is<sf::Event::Resized>())
            {
                sf::View view;
                view.setSize({gameWidth, gameHeight});
                view.setCenter({gameWidth / 2.f, gameHeight / 2.f});
                window.setView(view);
            }
        }

        if (isPlaying)
        {
            const float deltaTime = clock.restart().asSeconds();

            // Move the player's paddle
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) && (leftPaddle.getPosition().y - paddleSize.y / 2 > 5.f))
            {
                leftPaddle.move({0.f, -paddleSpeed * deltaTime});
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) &&
                (leftPaddle.getPosition().y + paddleSize.y / 2 < gameHeight - 5.f))
            {
                leftPaddle.move({0.f, paddleSpeed * deltaTime});
            }

            if (sf::Touch::isDown(0))
            {
                const sf::Vector2i pos       = sf::Touch::getPosition(0);
                const sf::Vector2f mappedPos = window.mapPixelToCoords(pos);
                leftPaddle.setPosition({leftPaddle.getPosition().x, mappedPos.y});
            }

            // Move the computer's paddle
            if (((rightPaddleSpeed < 0.f) && (rightPaddle.getPosition().y - paddleSize.y / 2 > 5.f)) ||
                ((rightPaddleSpeed > 0.f) && (rightPaddle.getPosition().y + paddleSize.y / 2 < gameHeight - 5.f)))
            {
                rightPaddle.move({0.f, rightPaddleSpeed * deltaTime});
            }

            // Update the computer's paddle direction according to the ball position
            if (aiTimer.getElapsedTime() > aiTime)
            {
                aiTimer.restart();
                if (ball.getPosition().y + ballRadius > rightPaddle.getPosition().y + paddleSize.y / 2)
                    rightPaddleSpeed = paddleSpeed;
                else if (ball.getPosition().y - ballRadius < rightPaddle.getPosition().y - paddleSize.y / 2)
                    rightPaddleSpeed = -paddleSpeed;
                else
                    rightPaddleSpeed = 0.f;
            }

            // Move the ball
            ball.move({ballSpeed * deltaTime, ballAngle});

#ifdef SFML_SYSTEM_IOS
            const std::string inputString = "Touch the screen to restart.";
#else
            const std::string inputString = "Press space to restart or\nescape to exit.";
#endif

            // Check collisions between the ball and the screen
            if (ball.getPosition().x - ballRadius < 0.f)
            {
                isPlaying = false;
                pauseMessage.setString("You Lost!\n\n" + inputString);
            }
            if (ball.getPosition().x + ballRadius > gameWidth)
            {
                isPlaying = false;
                pauseMessage.setString("You Won!\n\n" + inputString);
            }
            if (ball.getPosition().y - ballRadius < 0.f)
            {
                ballSound.play();
                ballAngle = -ballAngle;
                ball.setPosition({ball.getPosition().x, ballRadius + 0.1f});
            }
            if (ball.getPosition().y + ballRadius > gameHeight)
            {
                ballSound.play();
                ballAngle = -ballAngle;
                ball.setPosition({ball.getPosition().x, gameHeight - ballRadius - 0.1f});
            }

            std::uniform_real_distribution<float> dist(0, 20);

            // Check the collisions between the ball and the paddles
            // Left Paddle
            if (ball.getPosition().x - ballRadius < leftPaddle.getPosition().x + paddleSize.x / 2 &&
                ball.getPosition().x - ballRadius > leftPaddle.getPosition().x &&
                ball.getPosition().y + ballRadius >= leftPaddle.getPosition().y - paddleSize.y / 2 &&
                ball.getPosition().y - ballRadius <= leftPaddle.getPosition().y + paddleSize.y / 2)
            {
                if (ball.getPosition().y > leftPaddle.getPosition().y)
                    ballAngle = sf::degrees(180) - ballAngle + sf::degrees(dist(rng));
                else
                    ballAngle = sf::degrees(180) - ballAngle - sf::degrees(dist(rng));

                ballSound.play();
                ball.setPosition({leftPaddle.getPosition().x + ballRadius + paddleSize.x / 2 + 0.1f, ball.getPosition().y});
            }

            // Right Paddle
            if (ball.getPosition().x + ballRadius > rightPaddle.getPosition().x - paddleSize.x / 2 &&
                ball.getPosition().x + ballRadius < rightPaddle.getPosition().x &&
                ball.getPosition().y + ballRadius >= rightPaddle.getPosition().y - paddleSize.y / 2 &&
                ball.getPosition().y - ballRadius <= rightPaddle.getPosition().y + paddleSize.y / 2)
            {
                if (ball.getPosition().y > rightPaddle.getPosition().y)
                    ballAngle = sf::degrees(180) - ballAngle + sf::degrees(dist(rng));
                else
                    ballAngle = sf::degrees(180) - ballAngle - sf::degrees(dist(rng));

                ballSound.play();
                ball.setPosition({rightPaddle.getPosition().x - ballRadius - paddleSize.x / 2 - 0.1f, ball.getPosition().y});
            }
        }

        // Clear the window
        window.clear(sf::Color(50, 50, 50));

        if (isPlaying)
        {
            // Draw the paddles and the ball
            window.draw(leftPaddle);
            window.draw(rightPaddle);
            window.draw(ball);
        }
        else
        {
            // Draw the pause message
            window.draw(pauseMessage);
            window.draw(sfmlLogo);
        }

        // Display things on screen
        window.display();
    }

    return EXIT_SUCCESS;
}
