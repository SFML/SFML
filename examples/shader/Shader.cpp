
////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include "Effect.hpp"
#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>


////////////////////////////////////////////////////////////
// "Pixelate" fragment shader
////////////////////////////////////////////////////////////
class Pixelate : public Effect
{
public :

    Pixelate() :
    Effect("pixelate")
    {
    }

    bool OnLoad()
    {
        // Load the texture and initialize the sprite
        if (!myTexture.LoadFromFile("resources/background.jpg"))
            return false;
        mySprite.SetTexture(myTexture);

        // Load the shader
        if (!myShader.LoadFromFile("resources/pixelate.frag", sf::Shader::Fragment))
            return false;
        myShader.SetParameter("texture", sf::Shader::CurrentTexture);

        return true;
    }

    void OnUpdate(float, float x, float y)
    {
        myShader.SetParameter("pixel_threshold", (x + y) / 30);
    }

    void OnDraw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        states.Shader = &myShader;
        target.Draw(mySprite, states);
    }

private:

    sf::Texture myTexture;
    sf::Sprite mySprite;
    sf::Shader myShader;
};


////////////////////////////////////////////////////////////
// "Wave" vertex shader + "blur" fragment shader
////////////////////////////////////////////////////////////
class WaveBlur : public Effect
{
public :

    WaveBlur() :
    Effect("wave + blur")
    {
    }

    bool OnLoad()
    {
        // Create the text
        myText.SetString("Praesent suscipit augue in velit pulvinar hendrerit varius purus aliquam.\n"
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
        myText.SetCharacterSize(22);
        myText.SetPosition(30, 20);

        // Load the shader
        //if (!myShader.LoadFromFile("resources/wave.vert", sf::Shader::Vertex))
        if (!myShader.LoadFromFile("resources/wave.vert", "resources/blur.frag"))
            return false;

        return true;
    }

    void OnUpdate(float time, float x, float y)
    {
        myShader.SetParameter("wave_phase", time);
        myShader.SetParameter("wave_amplitude", x * 40, y * 40);
        myShader.SetParameter("blur_radius", (x + y) * 0.008f);
    }

    void OnDraw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        states.Shader = &myShader;
        target.Draw(myText, states);
    }

private:

    sf::Text myText;
    sf::Shader myShader;
};


////////////////////////////////////////////////////////////
// "Storm" vertex shader + "blink" fragment shader
////////////////////////////////////////////////////////////
class StormBlink : public Effect
{
public :

    StormBlink() :
    Effect("storm + blink")
    {
    }

    bool OnLoad()
    {
        // Create the points
        myPoints.SetPrimitiveType(sf::Points);
        for (int i = 0; i < 40000; ++i)
        {
            float x = static_cast<float>(std::rand() % 800);
            float y = static_cast<float>(std::rand() % 600);
            sf::Uint8 r = std::rand() % 255;
            sf::Uint8 g = std::rand() % 255;
            sf::Uint8 b = std::rand() % 255;
            myPoints.Append(sf::Vertex(sf::Vector2f(x, y), sf::Color(r, g, b)));
        }

        // Load the shader
        if (!myShader.LoadFromFile("resources/storm.vert", "resources/blink.frag"))
            return false;

        return true;
    }

    void OnUpdate(float time, float x, float y)
    {
        float radius = 200 + std::cos(time) * 150;
        myShader.SetParameter("storm_position", x * 800, y * 600);
        myShader.SetParameter("storm_inner_radius", radius / 3);
        myShader.SetParameter("storm_total_radius", radius);
        myShader.SetParameter("blink_alpha", 0.5f + std::cos(time * 3) * 0.25f);
    }

    void OnDraw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        states.Shader = &myShader;
        target.Draw(myPoints, states);
    }

private:

    sf::VertexArray myPoints;
    sf::Shader myShader;
};


////////////////////////////////////////////////////////////
// "Edge" post-effect fragment shader
////////////////////////////////////////////////////////////
class Edge : public Effect
{
public :

    Edge() :
    Effect("edge post-effect")
    {
    }

    bool OnLoad()
    {
        // Create the off-screen surface
        if (!mySurface.Create(800, 600))
            return false;
        mySurface.SetSmooth(true);

        // Load the textures
        if (!myBackgroundTexture.LoadFromFile("resources/sfml.png"))
            return false;
        myBackgroundTexture.SetSmooth(true);
        if (!myEntityTexture.LoadFromFile("resources/devices.png"))
            return false;
        myEntityTexture.SetSmooth(true);

        // Initialize the background sprite
        myBackgroundSprite.SetTexture(myBackgroundTexture);
        myBackgroundSprite.SetPosition(135, 100);

        // Load the moving entities
        for (int i = 0; i < 6; ++i)
        {
            sf::Sprite entity(myEntityTexture, sf::IntRect(96 * i, 0, 96, 96));
            myEntities.push_back(entity);
        }

        // Load the shader
        if (!myShader.LoadFromFile("resources/edge.frag", sf::Shader::Fragment))
            return false;
        myShader.SetParameter("texture", sf::Shader::CurrentTexture);

        return true;
    }

    void OnUpdate(float time, float x, float y)
    {
        myShader.SetParameter("edge_threshold", 1 - (x + y) / 2);

        // Update the position of the moving entities
        for (std::size_t i = 0; i < myEntities.size(); ++i)
        {
            float x = std::cos(0.25f * (time * i + (myEntities.size() - i))) * 300 + 350;
            float y = std::sin(0.25f * (time * (myEntities.size() - i) + i)) * 200 + 250;
            myEntities[i].SetPosition(x, y);
        }

        // Render the updated scene to the off-screen surface
        mySurface.Clear(sf::Color::White);
        mySurface.Draw(myBackgroundSprite);
        for (std::size_t i = 0; i < myEntities.size(); ++i)
            mySurface.Draw(myEntities[i]);
        mySurface.Display();
    }

    void OnDraw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        states.Shader = &myShader;
        target.Draw(sf::Sprite(mySurface.GetTexture()), states);
    }

private:

    sf::RenderTexture mySurface;
    sf::Texture myBackgroundTexture;
    sf::Texture myEntityTexture;
    sf::Sprite myBackgroundSprite;
    std::vector<sf::Sprite> myEntities;
    sf::Shader myShader;
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
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Shader");
    window.EnableVerticalSync(true);

    // Create the effects
    std::vector<Effect*> effects;
    effects.push_back(new Pixelate);
    effects.push_back(new WaveBlur);
    effects.push_back(new StormBlink);
    effects.push_back(new Edge);
    std::size_t current = 0;

    // Initialize them
    for (std::size_t i = 0; i < effects.size(); ++i)
        effects[i]->Load();

    // Create the messages background
    sf::Texture textBackgroundTexture;
    if (!textBackgroundTexture.LoadFromFile("resources/text-background.png"))
        return EXIT_FAILURE;
    sf::Sprite textBackground(textBackgroundTexture);
    textBackground.SetPosition(0, 520);
    textBackground.SetColor(sf::Color(255, 255, 255, 200));

    // Load the messages font
    sf::Font font;
    if (!font.LoadFromFile("resources/sansation.ttf"))
        return EXIT_FAILURE;

    // Create the description text
    sf::Text description("Current effect: " + effects[current]->GetName(), font, 20);
    description.SetPosition(10, 530);
    description.SetColor(sf::Color(80, 80, 80));

    // Create the instructions text
    sf::Text instructions("Press left and right arrows to change the current shader", font, 20);
    instructions.SetPosition(280, 555);
    instructions.SetColor(sf::Color(80, 80, 80));

    // Start the game loop
    sf::Clock clock;
    while (window.IsOpened())
    {
        // Process events
        sf::Event event;
        while (window.PollEvent(event))
        {
            // Close window: exit
            if (event.Type == sf::Event::Closed)
                window.Close();

            if (event.Type == sf::Event::KeyPressed)
            {
                switch (event.Key.Code)
                {
                    // Escape key: exit
                    case sf::Keyboard::Escape:
                        window.Close();
                        break;

                    // Left arrow key: previous shader
                    case sf::Keyboard::Left:
                        if (current == 0)
                            current = effects.size() - 1;
                        else
                            current--;
                        description.SetString("Current effect: " + effects[current]->GetName());
                        break;

                    // Right arrow key: next shader
                    case sf::Keyboard::Right:
                        if (current == effects.size() - 1)
                            current = 0;
                        else
                            current++;
                        description.SetString("Current effect: " + effects[current]->GetName());
                        break;

                    default:
                        break;
                }
            }
        }

        // Update the current example
        float x = static_cast<float>(sf::Mouse::GetPosition(window).x) / window.GetWidth();
        float y = static_cast<float>(sf::Mouse::GetPosition(window).y) / window.GetHeight();
        effects[current]->Update(clock.GetElapsedTime() / 1000.f, x, y);

        // Clear the window
        window.Clear(sf::Color(255, 128, 0));

        // Draw the current example
        window.Draw(*effects[current]);

        // Draw the text
        window.Draw(textBackground);
        window.Draw(instructions);
        window.Draw(description);

        // Finally, display the rendered frame on screen
        window.Display();
    }

    // delete the effects
    for (std::size_t i = 0; i < effects.size(); ++i)
        delete effects[i];

    return EXIT_SUCCESS;
}
