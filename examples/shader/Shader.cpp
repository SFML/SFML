
////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include "Effect.hpp"

#include <array>
#include <random>


namespace
{
std::random_device rd;
std::mt19937       rng(rd());
} // namespace

const sf::Font* Effect::s_font = nullptr;

////////////////////////////////////////////////////////////
// "Pixelate" fragment shader
////////////////////////////////////////////////////////////
class Pixelate : public Effect
{
public:
    Pixelate() : Effect("Pixelate")
    {
    }

    bool onLoad() override
    {
        // Load the texture and initialize the sprite
        if (!m_texture.loadFromFile("resources/background.jpg"))
            return false;
        m_sprite.setTexture(m_texture);

        // Load the shader
        if (!m_shader.loadFromFile("resources/pixelate.frag", sf::Shader::Fragment))
            return false;
        m_shader.setUniform("texture", sf::Shader::CurrentTexture);

        return true;
    }

    void onUpdate(float, float x, float y) override
    {
        m_shader.setUniform("pixel_threshold", (x + y) / 30);
    }

    void onDraw(sf::RenderTarget& target, const sf::RenderStates& states) const override
    {
        sf::RenderStates statesCopy(states);
        statesCopy.shader = &m_shader;
        target.draw(m_sprite, statesCopy);
    }

private:
    sf::Texture m_texture;
    sf::Sprite  m_sprite;
    sf::Shader  m_shader;
};


////////////////////////////////////////////////////////////
// "Wave" vertex shader + "blur" fragment shader
////////////////////////////////////////////////////////////
class WaveBlur : public Effect
{
public:
    WaveBlur() : Effect("Wave + Blur"), m_text(getFont())
    {
    }

    bool onLoad() override
    {
        // Create the text
        m_text.setString(
            "Praesent suscipit augue in velit pulvinar hendrerit varius purus aliquam.\n"
            "Mauris mi odio, bibendum quis fringilla a, laoreet vel orci. Proin vitae vulputate tortor.\n"
            "Praesent cursus ultrices justo, ut feugiat ante vehicula quis.\n"
            "Donec fringilla scelerisque mauris et viverra.\n"
            "Maecenas adipiscing ornare scelerisque. Nullam at libero elit.\n"
            "Pellentesque habitant morbi tristique senectus et netus et malesuada fames ac turpis egestas.\n"
            "Nullam leo urna, tincidunt id semper eget, ultricies sed mi.\n"
            "Morbi mauris massa, commodo id dignissim vel, lobortis et elit.\n"
            "Fusce vel libero sed neque scelerisque venenatis.\n"
            "Integer mattis tincidunt quam vitae iaculis.\n"
            "Vivamus fringilla sem non velit venenatis fermentum.\n"
            "Vivamus varius tincidunt nisi id vehicula.\n"
            "Integer ullamcorper, enim vitae euismod rutrum, massa nisl semper ipsum,\n"
            "vestibulum sodales sem ante in massa.\n"
            "Vestibulum in augue non felis convallis viverra.\n"
            "Mauris ultricies dolor sed massa convallis sed aliquet augue fringilla.\n"
            "Duis erat eros, porta in accumsan in, blandit quis sem.\n"
            "In hac habitasse platea dictumst. Etiam fringilla est id odio dapibus sit amet semper dui laoreet.\n");
        m_text.setCharacterSize(22);
        m_text.setPosition({30.f, 20.f});

        // Load the shader
        return m_shader.loadFromFile("resources/wave.vert", "resources/blur.frag");
    }

    void onUpdate(float time, float x, float y) override
    {
        m_shader.setUniform("wave_phase", time);
        m_shader.setUniform("wave_amplitude", sf::Vector2f(x * 40, y * 40));
        m_shader.setUniform("blur_radius", (x + y) * 0.008f);
    }

    void onDraw(sf::RenderTarget& target, const sf::RenderStates& states) const override
    {
        sf::RenderStates statesCopy(states);
        statesCopy.shader = &m_shader;
        target.draw(m_text, statesCopy);
    }

private:
    sf::Text   m_text;
    sf::Shader m_shader;
};


////////////////////////////////////////////////////////////
// "Storm" vertex shader + "blink" fragment shader
////////////////////////////////////////////////////////////
class StormBlink : public Effect
{
public:
    StormBlink() : Effect("Storm + Blink")
    {
    }

    bool onLoad() override
    {
        std::uniform_real_distribution<float>        xDistribution(0, 800);
        std::uniform_real_distribution<float>        yDistribution(0, 600);
        std::uniform_int_distribution<std::uint16_t> colorDistribution(0, 255);

        // Create the points
        m_points.setPrimitiveType(sf::PrimitiveType::Points);
        for (int i = 0; i < 40000; ++i)
        {
            auto x = xDistribution(rng);
            auto y = yDistribution(rng);
            auto r = static_cast<std::uint8_t>(colorDistribution(rng));
            auto g = static_cast<std::uint8_t>(colorDistribution(rng));
            auto b = static_cast<std::uint8_t>(colorDistribution(rng));
            m_points.append(sf::Vertex(sf::Vector2f(x, y), sf::Color(r, g, b)));
        }

        // Load the shader
        return m_shader.loadFromFile("resources/storm.vert", "resources/blink.frag");
    }

    void onUpdate(float time, float x, float y) override
    {
        float radius = 200 + std::cos(time) * 150;
        m_shader.setUniform("storm_position", sf::Vector2f(x * 800, y * 600));
        m_shader.setUniform("storm_inner_radius", radius / 3);
        m_shader.setUniform("storm_total_radius", radius);
        m_shader.setUniform("blink_alpha", 0.5f + std::cos(time * 3) * 0.25f);
    }

    void onDraw(sf::RenderTarget& target, const sf::RenderStates& states) const override
    {
        sf::RenderStates statesCopy(states);
        statesCopy.shader = &m_shader;
        target.draw(m_points, statesCopy);
    }

private:
    sf::VertexArray m_points;
    sf::Shader      m_shader;
};


////////////////////////////////////////////////////////////
// "Edge" post-effect fragment shader
////////////////////////////////////////////////////////////
class Edge : public Effect
{
public:
    Edge() : Effect("Edge Post-effect")
    {
    }

    bool onLoad() override
    {
        // Create the off-screen surface
        if (!m_surface.create({800, 600}))
            return false;
        m_surface.setSmooth(true);

        // Load the textures
        if (!m_backgroundTexture.loadFromFile("resources/sfml.png"))
            return false;
        m_backgroundTexture.setSmooth(true);
        if (!m_entityTexture.loadFromFile("resources/devices.png"))
            return false;
        m_entityTexture.setSmooth(true);

        // Initialize the background sprite
        m_backgroundSprite.setTexture(m_backgroundTexture);
        m_backgroundSprite.setPosition({135.f, 100.f});

        // Load the moving entities
        for (int i = 0; i < 6; ++i)
        {
            sf::Sprite entity(m_entityTexture, sf::IntRect({96 * i, 0}, {96, 96}));
            m_entities.push_back(entity);
        }

        // Load the shader
        if (!m_shader.loadFromFile("resources/edge.frag", sf::Shader::Fragment))
            return false;
        m_shader.setUniform("texture", sf::Shader::CurrentTexture);

        return true;
    }

    void onUpdate(float time, float x, float y) override
    {
        m_shader.setUniform("edge_threshold", 1 - (x + y) / 2);

        // Update the position of the moving entities
        for (std::size_t i = 0; i < m_entities.size(); ++i)
        {
            sf::Vector2f position;
            position.x = std::cos(0.25f * (time * static_cast<float>(i) + static_cast<float>(m_entities.size() - i))) * 300 +
                         350;
            position.y = std::sin(0.25f * (time * static_cast<float>(m_entities.size() - i) + static_cast<float>(i))) * 200 +
                         250;
            m_entities[i].setPosition(position);
        }

        // Render the updated scene to the off-screen surface
        m_surface.clear(sf::Color::White);
        m_surface.draw(m_backgroundSprite);
        for (const sf::Sprite& entity : m_entities)
            m_surface.draw(entity);
        m_surface.display();
    }

    void onDraw(sf::RenderTarget& target, const sf::RenderStates& states) const override
    {
        sf::RenderStates statesCopy(states);
        statesCopy.shader = &m_shader;
        target.draw(sf::Sprite(m_surface.getTexture()), statesCopy);
    }

private:
    sf::RenderTexture       m_surface;
    sf::Texture             m_backgroundTexture;
    sf::Texture             m_entityTexture;
    sf::Sprite              m_backgroundSprite;
    std::vector<sf::Sprite> m_entities;
    sf::Shader              m_shader;
};


////////////////////////////////////////////////////////////
// "Geometry" geometry shader example
////////////////////////////////////////////////////////////
class Geometry : public Effect
{
public:
    Geometry() : Effect("Geometry Shader Billboards"), m_pointCloud(sf::PrimitiveType::Points, 10000)
    {
    }

    bool onLoad() override
    {
        // Check if geometry shaders are supported
        if (!sf::Shader::isGeometryAvailable())
            return false;

        // Move the points in the point cloud to random positions
        for (std::size_t i = 0; i < 10000; ++i)
        {
            // Spread the coordinates from -480 to +480
            // So they'll always fill the viewport at 800x600
            std::uniform_real_distribution<float> positionDistribution(-480, 480);
            m_pointCloud[i].position.x = positionDistribution(rng);
            m_pointCloud[i].position.y = positionDistribution(rng);
        }

        // Load the texture
        if (!m_logoTexture.loadFromFile("resources/logo.png"))
            return false;

        // Load the shader
        if (!m_shader.loadFromFile("resources/billboard.vert", "resources/billboard.geom", "resources/billboard.frag"))
            return false;
        m_shader.setUniform("texture", sf::Shader::CurrentTexture);

        // Set the render resolution (used for proper scaling)
        m_shader.setUniform("resolution", sf::Vector2f(800, 600));

        return true;
    }

    void onUpdate(float /*time*/, float x, float y) override
    {
        // Reset our transformation matrix
        m_transform = sf::Transform::Identity;
        // Move to the center of the window
        m_transform.translate({400.f, 300.f});
        // Rotate everything based on cursor position
        m_transform.rotate(sf::degrees(x * 360.f));

        // Adjust billboard size to scale between 25 and 75
        float size = 25 + std::abs(y) * 50;

        // Update the shader parameter
        m_shader.setUniform("size", sf::Vector2f(size, size));
    }

    void onDraw(sf::RenderTarget& target, const sf::RenderStates& states) const override
    {
        sf::RenderStates statesCopy(states);

        // Prepare the render state
        statesCopy.shader    = &m_shader;
        statesCopy.texture   = &m_logoTexture;
        statesCopy.transform = m_transform;

        // Draw the point cloud
        target.draw(m_pointCloud, statesCopy);
    }

private:
    sf::Texture     m_logoTexture;
    sf::Transform   m_transform;
    sf::Shader      m_shader;
    sf::VertexArray m_pointCloud;
};


////////////////////////////////////////////////////////////
/// Entry point of application
///
/// \return Application exit code
///
////////////////////////////////////////////////////////////
int main()
{
    // Create the main window
    sf::RenderWindow window(sf::VideoMode({800, 600}), "SFML Shader", sf::Style::Titlebar | sf::Style::Close);
    window.setVerticalSyncEnabled(true);

    // Load the application font and pass it to the Effect class
    sf::Font font;
    if (!font.loadFromFile("resources/tuffy.ttf"))
        return EXIT_FAILURE;
    Effect::setFont(font);

    // Create the effects
    Pixelate   pixelateEffect;
    WaveBlur   waveBlurEffect;
    StormBlink stormBlinkEffect;
    Edge       edgeEffect;
    Geometry   geometryEffect;

    const std::array<Effect*, 5> effects{&pixelateEffect, &waveBlurEffect, &stormBlinkEffect, &edgeEffect, &geometryEffect};

    std::size_t current = 0;

    // Initialize them
    for (Effect* effect : effects)
        effect->load();

    // Create the messages background
    sf::Texture textBackgroundTexture;
    if (!textBackgroundTexture.loadFromFile("resources/text-background.png"))
        return EXIT_FAILURE;
    sf::Sprite textBackground(textBackgroundTexture);
    textBackground.setPosition({0.f, 520.f});
    textBackground.setColor(sf::Color(255, 255, 255, 200));

    // Create the description text
    sf::Text description(font, "Current effect: " + effects[current]->getName(), 20);
    description.setPosition({10.f, 530.f});
    description.setFillColor(sf::Color(80, 80, 80));

    // Create the instructions text
    sf::Text instructions(font, "Press left and right arrows to change the current shader", 20);
    instructions.setPosition({280.f, 555.f});
    instructions.setFillColor(sf::Color(80, 80, 80));

    // Start the game loop
    sf::Clock clock;
    while (window.isOpen())
    {
        // Process events
        for (sf::Event event; window.pollEvent(event);)
        {
            // Close window: exit
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed)
            {
                switch (event.key.code)
                {
                    // Escape key: exit
                    case sf::Keyboard::Escape:
                        window.close();
                        break;

                    // Left arrow key: previous shader
                    case sf::Keyboard::Left:
                        if (current == 0)
                            current = effects.size() - 1;
                        else
                            --current;
                        description.setString("Current effect: " + effects[current]->getName());
                        break;

                    // Right arrow key: next shader
                    case sf::Keyboard::Right:
                        if (current == effects.size() - 1)
                            current = 0;
                        else
                            ++current;
                        description.setString("Current effect: " + effects[current]->getName());
                        break;

                    default:
                        break;
                }
            }
        }

        // Update the current example
        const auto [x, y] = sf::Vector2f(sf::Mouse::getPosition(window)).cwiseDiv(sf::Vector2f(window.getSize()));
        effects[current]->update(clock.getElapsedTime().asSeconds(), x, y);

        // Clear the window
        if (effects[current]->getName() == "Edge Post-effect")
        {
            window.clear(sf::Color::White);
        }
        else
        {
            window.clear(sf::Color(50, 50, 50));
        }

        // Draw the current example
        window.draw(*effects[current]);

        // Draw the text
        window.draw(textBackground);
        window.draw(instructions);
        window.draw(description);

        // Finally, display the rendered frame on screen
        window.display();
    }
}
