
////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cmath>
#include <ctime>
#include <cstdlib>


////////////////////////////////////////////////////////////
/// Entry point of application
///
/// \return Application exit code
///
////////////////////////////////////////////////////////////
int main()
{
    std::srand(static_cast<unsigned int>(std::time(NULL)));

    // Define some constants
    const float pi = 3.14159f;
    const int gameWidth = 800;
    const int gameHeight = 600;
    sf::Vector2f paddleSize(25, 100);
    float ballRadius = 10.f;

    // Create the window of the application
    sf::RenderWindow window(sf::VideoMode(gameWidth, gameHeight, 32), "SFML Pong");
    window.EnableVerticalSync(true);

    // Load the sounds used in the game
    sf::SoundBuffer ballSoundBuffer;
    if (!ballSoundBuffer.LoadFromFile("resources/ball.wav"))
        return EXIT_FAILURE;
    sf::Sound ballSound(ballSoundBuffer);

    // Create the left paddle
    sf::RectangleShape leftPaddle;
    leftPaddle.SetSize(paddleSize - sf::Vector2f(3, 3));
    leftPaddle.SetOutlineThickness(3);
    leftPaddle.SetOutlineColor(sf::Color::Black);
    leftPaddle.SetFillColor(sf::Color(100, 100, 200));
    leftPaddle.SetOrigin(paddleSize / 2.f);

    // Create the right paddle
    sf::RectangleShape rightPaddle;
    rightPaddle.SetSize(paddleSize - sf::Vector2f(3, 3));
    rightPaddle.SetOutlineThickness(3);
    rightPaddle.SetOutlineColor(sf::Color::Black);
    rightPaddle.SetFillColor(sf::Color(200, 100, 100));
    rightPaddle.SetOrigin(paddleSize / 2.f);

    // Create the ball
    sf::CircleShape ball;
    ball.SetRadius(ballRadius - 3);
    ball.SetOutlineThickness(3);
    ball.SetOutlineColor(sf::Color::Black);
    ball.SetFillColor(sf::Color::White);
    ball.SetOrigin(ballRadius / 2, ballRadius / 2);

    // Load the text font
    sf::Font font;
    if (!font.LoadFromFile("resources/sansation.ttf"))
        return EXIT_FAILURE;

    // Initialize the pause message
    sf::Text pauseMessage;
    pauseMessage.SetFont(font);
    pauseMessage.SetCharacterSize(40);
    pauseMessage.SetPosition(170.f, 150.f);
    pauseMessage.SetColor(sf::Color::White);
    pauseMessage.SetString("Welcome to SFML pong!\nPress space to start the game");

    // Define the paddles properties
    sf::Clock AITimer;
    const sf::Uint32 AITime = 300;
    const float paddleSpeed = 400.f;
    float rightPaddleSpeed  = 0.f;
    const float ballSpeed   = 400.f;
    float ballAngle         = 0.f; // to be changed later

    bool isPlaying = false;
    while (window.IsOpened())
    {
        // Handle events
        sf::Event event;
        while (window.PollEvent(event))
        {
            // Window closed or escape key pressed: exit
            if ((event.Type == sf::Event::Closed) || 
               ((event.Type == sf::Event::KeyPressed) && (event.Key.Code == sf::Keyboard::Escape)))
            {
                window.Close();
                break;
            }

            // Space key pressed: play
            if ((event.Type == sf::Event::KeyPressed) && (event.Key.Code == sf::Keyboard::Space))
            {
                if (!isPlaying)
                {
                    // (re)start the game
                    isPlaying = true;

                    // Reset the position of the paddles and ball
                    leftPaddle.SetPosition(10 + paddleSize.x / 2, gameHeight / 2);
                    rightPaddle.SetPosition(gameWidth - 10 - paddleSize.x / 2, gameHeight / 2);
                    ball.SetPosition(gameWidth / 2, gameHeight / 2);

                    // Reset the ball angle
                    do
                    {
                        // Make sure the ball initial angle is not too much vertical
                        ballAngle = (std::rand() % 360) * 2 * pi / 360;
                    }
                    while (std::abs(std::cos(ballAngle)) < 0.7f);
                }
            }
        }

        if (isPlaying)
        {
            float deltaTime = window.GetFrameTime() / 1000.f;

            // Move the player's paddle
            if (sf::Keyboard::IsKeyPressed(sf::Keyboard::Up) &&
               (leftPaddle.GetPosition().y - paddleSize.y / 2 > 5.f))
            {
                leftPaddle.Move(0.f, -paddleSpeed * deltaTime);
            }
            if (sf::Keyboard::IsKeyPressed(sf::Keyboard::Down) &&
               (leftPaddle.GetPosition().y + paddleSize.y / 2 < gameHeight - 5.f))
            {
                leftPaddle.Move(0.f, paddleSpeed * deltaTime);
            }

            // Move the computer's paddle
            if (((rightPaddleSpeed < 0.f) && (rightPaddle.GetPosition().y - paddleSize.y / 2 > 5.f)) ||
                ((rightPaddleSpeed > 0.f) && (rightPaddle.GetPosition().y + paddleSize.y / 2 < gameHeight - 5.f)))
            {
                rightPaddle.Move(0.f, rightPaddleSpeed * deltaTime);
            }

            // Update the computer's paddle direction according to the ball position
            if (AITimer.GetElapsedTime() > AITime)
            {
                AITimer.Reset();
                if (ball.GetPosition().y + ballRadius > rightPaddle.GetPosition().y + paddleSize.y / 2)
                    rightPaddleSpeed = paddleSpeed;
                else if (ball.GetPosition().y - ballRadius < rightPaddle.GetPosition().y - paddleSize.y / 2)
                    rightPaddleSpeed = -paddleSpeed;
                else
                    rightPaddleSpeed = 0.f;
            }

            // Move the ball
            float factor = ballSpeed * deltaTime;
            ball.Move(std::cos(ballAngle) * factor, std::sin(ballAngle) * factor);

            // Check collisions between the ball and the screen
            if (ball.GetPosition().x - ballRadius < 0.f)
            {
                isPlaying = false;
                pauseMessage.SetString("You lost !\nPress space to restart or\nescape to exit");
            }
            if (ball.GetPosition().x + ballRadius > 800)
            {
                isPlaying = false;
                pauseMessage.SetString("You won !\nPress space to restart or\nescape to exit");
            }
            if (ball.GetPosition().y - ballRadius < 0.f)
            {
                ballSound.Play();
                ballAngle = -ballAngle;
                ball.SetPosition(ball.GetPosition().x, ballRadius + 0.1f);
            }
            if (ball.GetPosition().y + ballRadius > gameHeight)
            {
                ballSound.Play();
                ballAngle = -ballAngle;
                ball.SetPosition(ball.GetPosition().x, gameHeight - ballRadius - 0.1f);
            }

            // Check the collisions between the ball and the paddles
            // Left Paddle
            if (ball.GetPosition().x - ballRadius < leftPaddle.GetPosition().x + paddleSize.x / 2 && 
                ball.GetPosition().x - ballRadius > leftPaddle.GetPosition().x &&
                ball.GetPosition().y + ballRadius >= leftPaddle.GetPosition().y - paddleSize.y / 2 &&
                ball.GetPosition().y - ballRadius <= leftPaddle.GetPosition().y + paddleSize.y / 2)
            {
                if (ball.GetPosition().y > leftPaddle.GetPosition().y)
                    ballAngle = pi - ballAngle + (std::rand() % 20) * pi / 180;
                else
                    ballAngle = pi - ballAngle - (std::rand() % 20) * pi / 180;

                ballSound.Play();
                ball.SetPosition(leftPaddle.GetPosition().x + ballRadius + paddleSize.x / 2 + 0.1f, ball.GetPosition().y);
            }

            // Right Paddle
            if (ball.GetPosition().x + ballRadius > rightPaddle.GetPosition().x - paddleSize.x / 2 &&
                ball.GetPosition().x + ballRadius < rightPaddle.GetPosition().x &&
                ball.GetPosition().y + ballRadius >= rightPaddle.GetPosition().y - paddleSize.y / 2 &&
                ball.GetPosition().y - ballRadius <= rightPaddle.GetPosition().y + paddleSize.y / 2)
            {
                if (ball.GetPosition().y > rightPaddle.GetPosition().y)
                    ballAngle = pi - ballAngle + (std::rand() % 20) * pi / 180;
                else
                    ballAngle = pi - ballAngle - (std::rand() % 20) * pi / 180;

                ballSound.Play();
                ball.SetPosition(rightPaddle.GetPosition().x - ballRadius - paddleSize.x / 2 - 0.1f, ball.GetPosition().y);
            }
        }

        // Clear the window
        window.Clear(sf::Color(50, 200, 50));

        if (isPlaying)
        {
            // Draw the paddles and the ball
            window.Draw(leftPaddle);
            window.Draw(rightPaddle);
            window.Draw(ball);
        }
        else
        {
            // Draw the pause message
            window.Draw(pauseMessage);
        }

        // Display things on screen
        window.Display();
    }

    return EXIT_SUCCESS;
}
