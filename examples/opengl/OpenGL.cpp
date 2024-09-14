#include "SFML/Graphics/Font.hpp"
#include "SFML/Graphics/Image.hpp"
#include "SFML/Graphics/RenderStates.hpp"
#include "SFML/Graphics/RenderTexture.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Text.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "SFML/System/Clock.hpp"
#include "SFML/System/Vector2.hpp"
#include "SFML/Window/Event.hpp"
#include "SFML/Window/VideoMode.hpp"

#include <random>
#include <string>
#include <vector>

#include <cstddef>


float getRndFloat(float min, float max)
{
    static std::mt19937 rng(std::random_device{}());
    return std::uniform_real_distribution<float>{min, max}(rng);
}

int main()
{

    //
    //
    // Set up window
    constexpr sf::Vector2f windowSize{1024.f, 768.f};

    sf::RenderWindow window(sf::VideoMode{{1024u, 768u}}, "upstream batching");

    window.setVerticalSyncEnabled(true);

    //
    //
    // Load fonts
    const sf::Font fontTuffy("resources/tuffy.ttf");
    const sf::Font fontMouldyCheese("resources/mouldycheese.ttf");

    //
    //
    // Load images
    const sf::Image imgElephant("resources/elephant.png");
    const sf::Image imgGiraffe("resources/giraffe.png");
    const sf::Image imgMonkey("resources/monkey.png");
    const sf::Image imgPig("resources/pig.png");
    const sf::Image imgRabbit("resources/rabbit.png");
    const sf::Image imgSnake("resources/snake.png");

    //
    //
    // Texture array
    const sf::Texture spriteTextures[]{sf::Texture(imgElephant),
                                       sf::Texture(imgGiraffe),
                                       sf::Texture(imgMonkey),
                                       sf::Texture(imgPig),
                                       sf::Texture(imgRabbit),
                                       sf::Texture(imgSnake)};

    //
    //
    // Simulation stuff
    constexpr const char* names[]{"Elephant", "Giraffe", "Monkey", "Pig", "Rabbit", "Snake"};

    struct Entity
    {
        sf::Text     text;
        sf::Sprite   sprite;
        sf::Vector2f velocity;
        float        torque{};
    };

    std::vector<Entity> entities;

    const auto populateEntities = [&](const std::size_t n)
    {
        if (n < entities.size())
        {
            entities.erase(entities.begin() + static_cast<std::ptrdiff_t>(n), entities.end());
            return;
        }

        entities.clear();
        entities.reserve(n);

        for (std::size_t i = 0u; i < n; ++i)
        {
            const std::size_t  type    = i % 6u;
            const sf::Texture& texture = spriteTextures[type];

            const auto label = std::string{names[i % 6u]} + " #" + std::to_string((i / (type + 1)) + 1);

            entities.push_back({sf::Text{i % 2u == 0u ? fontTuffy : fontMouldyCheese, label},
                                sf::Sprite{texture},
                                sf::Vector2f{getRndFloat(-2.5f, 2.5f), getRndFloat(-2.5f, 2.5f)},
                                getRndFloat(-0.05f, 0.05f)});

            auto& [text, sprite, velocity, torque] = entities.back();

            sprite.setOrigin(sf::Vector2f(texture.getSize()) / 2.f);
            sprite.setRotation(sf::degrees(getRndFloat(0.f, 360.f)));

            const float scaleFactor = getRndFloat(0.08f, 0.17f);
            sprite.setScale({scaleFactor, scaleFactor});
            text.setScale({scaleFactor * 3.5f, scaleFactor * 3.5f});

            sprite.setPosition({getRndFloat(0.f, windowSize.x), getRndFloat(0.f, windowSize.y)});

            text.setFillColor(sf::Color::Black);
            text.setOutlineColor(sf::Color::White);
            text.setOutlineThickness(5.f);

            text.setOrigin(text.getLocalBounds().size / 2.f);
        }
    };

    //
    //
    // Set up UI elements
    bool drawSprites = true;
    bool drawText    = true;
    int  numEntities = 10000;

    //
    //
    // Set up clock and time sampling stuff
    sf::Clock clock;
    sf::Clock fpsClock;

    std::vector<float> samplesUpdateMs{};
    std::vector<float> samplesDrawMs{};
    std::vector<float> samplesFPS{};

    const auto recordUs = [&](std::vector<float>& target, const float value)
    {
        target.push_back(value);

        if (target.size() > 32u)
            target.erase(target.begin());
    };

    const auto getAverage = [&](const std::vector<float>& target)
    {
        double accumulator = 0.0;

        for (auto value : target)
            accumulator += static_cast<double>(value);

        return accumulator / static_cast<double>(target.size());
    };

    //
    //
    // Set up initial simulation state
    populateEntities(static_cast<std::size_t>(numEntities));

    //
    //
    // Simulation loop
    while (true)
    {
        fpsClock.restart();

        ////////////////////////////////////////////////////////////
        // Event handling
        ////////////////////////////////////////////////////////////
        while (auto event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>() ||
                (event->is<sf::Event::KeyPressed>() &&
                 event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::Escape))
            {
                window.close();
                break;
            }
        }

        ////////////////////////////////////////////////////////////
        // Update step
        ////////////////////////////////////////////////////////////
        {
            clock.restart();

            for (auto& [text, sprite, velocity, torque] : entities)
            {
                sprite.move(velocity);
                sprite.rotate(sf::radians(torque));

                if ((sprite.getPosition().x > windowSize.x && velocity.x > 0.f) ||
                    (sprite.getPosition().x < 0.f && velocity.x < 0.f))
                    velocity.x = -velocity.x;

                if ((sprite.getPosition().y > windowSize.y && velocity.y > 0.f) ||
                    (sprite.getPosition().y < 0.f && velocity.y < 0.f))
                    velocity.y = -velocity.y;

                text.setPosition(sprite.getPosition() - sf::Vector2f{0.f, 250.f * sprite.getScale().x});
            }

            recordUs(samplesUpdateMs, clock.getElapsedTime().asSeconds() * 1000.f);
        }

        ////////////////////////////////////////////////////////////
        // Draw step
        ////////////////////////////////////////////////////////////
        {
            clock.restart();
            window.clear();

            for (const Entity& entity : entities)
            {
                if (drawSprites)
                    window.draw(entity.sprite);

                if (drawText)
                    window.draw(entity.text);
            }

            recordUs(samplesDrawMs, clock.getElapsedTime().asSeconds() * 1000.f);
        }

        window.display();

        recordUs(samplesFPS, 1.f / fpsClock.getElapsedTime().asSeconds());

        window.setTitle(
            "FPS: " + std::to_string(getAverage(samplesFPS)) + " || U: " + std::to_string(getAverage(samplesUpdateMs)) +
            " || D: " + std::to_string(getAverage(samplesDrawMs)));
    }
}
