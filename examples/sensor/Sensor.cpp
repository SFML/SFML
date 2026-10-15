////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics.hpp>

#include <SFML/Main.hpp>
#include <algorithm>
#include <array>
#include <iostream>

#include <cstdlib>


namespace
{
sf::Vector3f getPull(sf::Sensor::Type sensor)
{
    // Android reports accelerations in the opposite direction to iOS, flip them so gravity points down on both
#ifdef SFML_SYSTEM_ANDROID
    return -sf::Sensor::getValue(sensor);
#else
    return sf::Sensor::getValue(sensor);
#endif
}

sf::Vector2f toScreen(sf::Vector3f vector)
{
    // The sensor Y axis points to the top of the device, SFML's points down
    return {vector.x, -vector.y};
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
    // Exit early if the sensors are not available
    constexpr std::array sensors = {sf::Sensor::Type::Gravity, sf::Sensor::Type::UserAcceleration, sf::Sensor::Type::Magnetometer};
    for (const auto sensor : sensors)
    {
        if (!sf::Sensor::isAvailable(sensor))
        {
            std::cerr << "Motion sensors not supported on current system, aborting" << std::endl;
            return EXIT_FAILURE;
        }
        sf::Sensor::setEnabled(sensor, true);
    }

    sf::RenderWindow window(sf::VideoMode({800, 600}), "SFML Sensor");
    window.setVerticalSyncEnabled(true);
    sf::Vector2f screenSize(window.getSize());

    // Create the ball
    const float     ballRadius = std::min(screenSize.x, screenSize.y) / 20.f;
    sf::CircleShape ball(ballRadius);
    ball.setOrigin({ballRadius, ballRadius});
    ball.setFillColor(sf::Color(230, 80, 80));
    ball.setOutlineColor(sf::Color::White);
    ball.setOutlineThickness(ballRadius / 8.f);

    sf::Vector2f ballPosition = screenSize / 2.f;
    sf::Vector2f ballVelocity;

    // Create the compass in the top left corner
    const float        compassRadius = std::min(screenSize.x, screenSize.y) / 12.f;
    const sf::Vector2f compassCenter(compassRadius * 1.5f, compassRadius * 1.5f);
    sf::CircleShape    compass(compassRadius);
    compass.setOrigin({compassRadius, compassRadius});
    compass.setPosition(compassCenter);
    compass.setFillColor(sf::Color(30, 30, 40));
    compass.setOutlineColor(sf::Color::White);
    compass.setOutlineThickness(compassRadius / 20.f);

    const float     needleLength = compassRadius * 0.8f;
    const float     needleWidth  = compassRadius * 0.15f;
    sf::ConvexShape northNeedle(3);
    northNeedle.setPoint(0, {needleLength, 0.f});
    northNeedle.setPoint(1, {0.f, needleWidth});
    northNeedle.setPoint(2, {0.f, -needleWidth});
    northNeedle.setFillColor(sf::Color(230, 80, 80));
    northNeedle.setPosition(compassCenter);
    sf::ConvexShape southNeedle(3);
    southNeedle.setPoint(0, {-needleLength, 0.f});
    southNeedle.setPoint(1, {0.f, -needleWidth});
    southNeedle.setPoint(2, {0.f, needleWidth});
    southNeedle.setFillColor(sf::Color::White);
    southNeedle.setPosition(compassCenter);

    // Chosen so the ball crosses the screen in about a second when tilted fully
    const float pixelsPerMeter = std::min(screenSize.x, screenSize.y) / 5.f;

    // Device movements are small compared to the screen at the above scale, so exaggerate them
    constexpr float movementScale = 10.f;

    sf::Clock clock;
    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>() ||
                (event->is<sf::Event::KeyPressed>() &&
                 event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::Escape))
            {
                window.close();
                break;
            }

            // Window size changed: adjust the view
            if (const auto* resized = event->getIf<sf::Event::Resized>())
            {
                screenSize = sf::Vector2f(resized->size);
                window.setView(sf::View(sf::FloatRect({}, screenSize)));
            }
        }

        const float deltaTime = clock.restart().asSeconds();

        // Tilting the device rolls the ball, moving the device pushes the ball the opposite way
        const sf::Vector3f gravity  = getPull(sf::Sensor::Type::Gravity);
        const sf::Vector3f movement = getPull(sf::Sensor::Type::UserAcceleration) * movementScale;
        ballVelocity += (toScreen(gravity) + toScreen(movement)) * pixelsPerMeter * deltaTime;
        ballPosition += ballVelocity * deltaTime;

        // Stop the ball at the edges of the screen
        if (ballPosition.x < ballRadius || ballPosition.x > screenSize.x - ballRadius)
        {
            ballPosition.x = std::clamp(ballPosition.x, ballRadius, screenSize.x - ballRadius);
            ballVelocity.x = 0.f;
        }
        if (ballPosition.y < ballRadius || ballPosition.y > screenSize.y - ballRadius)
        {
            ballPosition.y = std::clamp(ballPosition.y, ballRadius, screenSize.y - ballRadius);
            ballVelocity.y = 0.f;
        }
        ball.setPosition(ballPosition);

        // Find north along the ground, as the magnetic field also points into it in most places
        const sf::Vector3f up    = -gravity;
        const sf::Vector3f field = sf::Sensor::getValue(sf::Sensor::Type::Magnetometer);
        const sf::Vector3f east  = field.cross(up);
        if (east.lengthSquared() > 0.f)
        {
            const sf::Vector3f north = up.cross(east);

            // Heading of the top of the device, undefined when it points straight up
            const sf::Vector2f top(north.normalized().y, east.normalized().y);
            if (top.lengthSquared() > 0.01f)
            {
                const sf::Angle rotation = sf::degrees(-90.f) - top.angle();
                northNeedle.setRotation(rotation);
                southNeedle.setRotation(rotation);
            }
        }

        // Draw everything
        window.clear(sf::Color(50, 50, 60));
        window.draw(ball);
        window.draw(compass);
        window.draw(northNeedle);
        window.draw(southNeedle);
        window.display();
    }
    return 0;
}
