
////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics.hpp>
#include <map>
#include <math.h>


void DisplayError();

////////////////////////////////////////////////////////////
/// A class to simplify shader selection
///
////////////////////////////////////////////////////////////
class ShaderSelector
{
public :

    // Constructor
    ShaderSelector(std::map<std::string, sf::Shader>& owner, const std::string& shader) :
    myOwner   (&owner),
    myIterator(owner.find(shader))
    {
    }

    // Select the previous shader
    void GotoPrevious()
    {
        if (myIterator == myOwner->begin())
            myIterator = myOwner->end();
        myIterator--;
    }

    // Select the next shader
    void GotoNext()
    {
        myIterator++;
        if (myIterator == myOwner->end())
            myIterator = myOwner->begin();
    }

    // Update the shader parameters
    void Update(float x, float y)
    {
        if      (myIterator->first == "blur")     myIterator->second.SetParameter("offset", x * y * 0.03f);
        else if (myIterator->first == "colorize") myIterator->second.SetParameter("color", 0.3f, x, y);
        else if (myIterator->first == "edge")     myIterator->second.SetParameter("threshold", x * y);
        else if (myIterator->first == "fisheye")  myIterator->second.SetParameter("mouse", x, y);
        else if (myIterator->first == "wave")     myIterator->second.SetParameter("offset", x, y);
        else if (myIterator->first == "pixelate") myIterator->second.SetParameter("mouse", x, y);
    }

    // Get the name of the current shader
    const std::string& GetName() const
    {
        return myIterator->first;
    }

    // Get the current shader
    const sf::Shader& GetShader() const
    {
        return myIterator->second;
    }

private :

    std::map<std::string, sf::Shader>* myOwner;
    std::map<std::string, sf::Shader>::iterator myIterator;
};


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
        DisplayError();
        return EXIT_SUCCESS;
    }

    // Create the main window
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Shader");

    // Create the render image
    sf::RenderImage image;
    if (!image.Create(window.GetWidth(), window.GetHeight()))
        return EXIT_FAILURE;

    // Load a background image to display
    sf::Image backgroundImage;
    if (!backgroundImage.LoadFromFile("resources/background.jpg"))
        return EXIT_FAILURE;
    sf::Sprite background(backgroundImage);
    backgroundImage.SetSmooth(false);

    // Load a sprite which we'll move into the scene
    sf::Image entityImage;
    if (!entityImage.LoadFromFile("resources/sprite.png"))
        return EXIT_FAILURE;
    sf::Sprite entity(entityImage);

    // Load the text font
    sf::Font font;
    if (!font.LoadFromFile("resources/sansation.ttf"))
        return EXIT_FAILURE;

    // Load the image needed for the wave shader
    sf::Image waveImage;
    if (!waveImage.LoadFromFile("resources/wave.jpg"))
        return EXIT_FAILURE;

    // Load all shaders
    std::map<std::string, sf::Shader> shaders;
    if (!shaders["nothing"].LoadFromFile("resources/nothing.sfx"))   return EXIT_FAILURE;
    if (!shaders["blur"].LoadFromFile("resources/blur.sfx"))         return EXIT_FAILURE;
    if (!shaders["colorize"].LoadFromFile("resources/colorize.sfx")) return EXIT_FAILURE;
    if (!shaders["edge"].LoadFromFile("resources/edge.sfx"))         return EXIT_FAILURE;
    if (!shaders["fisheye"].LoadFromFile("resources/fisheye.sfx"))   return EXIT_FAILURE;
    if (!shaders["wave"].LoadFromFile("resources/wave.sfx"))         return EXIT_FAILURE;
    if (!shaders["pixelate"].LoadFromFile("resources/pixelate.sfx")) return EXIT_FAILURE;
    ShaderSelector backgroundShader(shaders, "nothing");
    ShaderSelector entityShader(shaders, "nothing");
    ShaderSelector globalShader(shaders, "nothing");

    // Do specific initializations
    shaders["nothing"].SetCurrentTexture("texture");
    shaders["blur"].SetCurrentTexture("texture");
    shaders["blur"].SetParameter("offset", 0.f);
    shaders["colorize"].SetCurrentTexture("texture");
    shaders["colorize"].SetParameter("color", 1.f, 1.f, 1.f);
    shaders["edge"].SetCurrentTexture("texture");
    shaders["fisheye"].SetCurrentTexture("texture");
    shaders["wave"].SetCurrentTexture("texture");
    shaders["wave"].SetTexture("wave", waveImage);
    shaders["pixelate"].SetCurrentTexture("texture");

    // Define a string for displaying the description of the current shader
    sf::Text shaderStr;
    shaderStr.SetFont(font);
    shaderStr.SetCharacterSize(20);
    shaderStr.SetPosition(5.f, 0.f);
    shaderStr.SetColor(sf::Color(250, 100, 30));
    shaderStr.SetString("Background shader: \"" + backgroundShader.GetName() + "\"\n"
                        "Flower shader: \"" + entityShader.GetName() + "\"\n"
                        "Global shader: \"" + globalShader.GetName() + "\"\n");

    // Define a string for displaying help
    sf::Text infoStr;
    infoStr.SetFont(font);
    infoStr.SetCharacterSize(20);
    infoStr.SetPosition(5.f, 500.f);
    infoStr.SetColor(sf::Color(250, 100, 30));
    infoStr.SetString("Move your mouse to change the shaders' parameters\n"
                      "Press numpad 1/4 to change the background shader\n"
                      "Press numpad 2/5 to change the flower shader\n"
                      "Press numpad 3/6 to change the global shader");

sf::RenderImage test;
test.Create(800, 600);

    // Create a clock to measure the total time elapsed
    sf::Clock clock;

    // Start the game loop
    while (window.IsOpened())
    {
        // Process events
        sf::Event event;
        while (window.GetEvent(event))
        {
            // Close window : exit
            if (event.Type == sf::Event::Closed)
                window.Close();

            if (event.Type == sf::Event::KeyPressed)
            {
                // Escape key : exit
                if (event.Key.Code == sf::Key::Escape)
                    window.Close();

                // Numpad : switch effect
                switch (event.Key.Code)
                {
                    case sf::Key::Numpad1 : backgroundShader.GotoPrevious(); break;
                    case sf::Key::Numpad4 : backgroundShader.GotoNext();     break;
                    case sf::Key::Numpad2 : entityShader.GotoPrevious();     break;
                    case sf::Key::Numpad5 : entityShader.GotoNext();         break;
                    case sf::Key::Numpad3 : globalShader.GotoPrevious();     break;
                    case sf::Key::Numpad6 : globalShader.GotoNext();         break;
                    default :                                                break;
                }

                // Update the text
                shaderStr.SetString("Background shader: \"" + backgroundShader.GetName() + "\"\n"
                                    "Entity shader: \"" + entityShader.GetName() + "\"\n"
                                    "Global shader: \"" + globalShader.GetName() + "\"\n");
            }
        }

        // Get the mouse position in the range [0, 1]
        float mouseX = window.GetInput().GetMouseX() / static_cast<float>(window.GetWidth());
        float mouseY = window.GetInput().GetMouseY() / static_cast<float>(window.GetHeight());

        // Update the shaders
        backgroundShader.Update(mouseX, mouseY);
        entityShader.Update(mouseX, mouseY);
        globalShader.Update(mouseX, mouseY);

        // Animate the entity
        float entityX = (cos(clock.GetElapsedTime() * 1.3f) + 1.2f) * 300;
        float entityY = (cos(clock.GetElapsedTime() * 0.8f) + 1.2f) * 200;
        entity.SetPosition(entityX, entityY);
        entity.Rotate(window.GetFrameTime() * 100);

        // Draw the background and the moving entity to the render image
        image.Clear();
        image.Draw(background, backgroundShader.GetShader());
        image.Draw(entity, entityShader.GetShader());
        image.Display();

        // Draw the contents of the render image to the window
        sf::Sprite screen(image.GetImage());
        window.Draw(screen, globalShader.GetShader());

        // Draw the interface texts
        window.Draw(shaderStr);
        window.Draw(infoStr);

        // Finally, display the rendered frame on screen
        window.Display();
    }

    return EXIT_SUCCESS;
}


////////////////////////////////////////////////////////////
/// Fonction called when the post-effects are not supported ;
/// Display an error message and wait until the user exits
///
////////////////////////////////////////////////////////////
void DisplayError()
{
    // Create the main window
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Shader");

    // Define a string for displaying the error message
    sf::Text error("Sorry, your system doesn't support shaders");
    error.SetPosition(100.f, 250.f);
    error.SetColor(sf::Color(200, 100, 150));

    // Start the game loop
    while (window.IsOpened())
    {
        // Process events
        sf::Event event;
        while (window.GetEvent(event))
        {
            // Close window : exit
            if (event.Type == sf::Event::Closed)
                window.Close();

            // Escape key : exit
            if ((event.Type == sf::Event::KeyPressed) && (event.Key.Code == sf::Key::Escape))
                window.Close();
        }

        // Clear the window
        window.Clear();

        // Draw the error message
        window.Draw(error);

        // Finally, display the rendered frame on screen
        window.Display();
    }
}
