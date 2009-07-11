
////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics.hpp>
#include <map>
#include <math.h>

void DisplayError();


////////////////////////////////////////////////////////////
/// Entry point of application
///
/// \return Application exit code
///
////////////////////////////////////////////////////////////
int main()
{
    // Check that the system can use post effects
    if (sf::PostFX::CanUsePostFX() == false)
    {
        DisplayError();
        return EXIT_SUCCESS;
    }

    // Create the main window
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML PostFX");

    // Load a background image to display
    sf::Image backgroundImage;
    if (!backgroundImage.LoadFromFile("datas/post-fx/background.jpg"))
        return EXIT_FAILURE;
    sf::Sprite background(backgroundImage);

    // Load a sprite which we'll move into the scene
    sf::Image entityImage;
    if (!entityImage.LoadFromFile("datas/post-fx/sprite.png"))
        return EXIT_FAILURE;
    sf::Sprite entity(entityImage);

    // Load the text font
    sf::Font font;
    if (!font.LoadFromFile("datas/post-fx/cheeseburger.ttf"))
        return EXIT_FAILURE;

    // Load the image needed for the wave effect
    sf::Image waveImage;
    if (!waveImage.LoadFromFile("datas/post-fx/wave.jpg"))
        return EXIT_FAILURE;

    // Load all effects
    std::map<std::string, sf::PostFX> effects;
    if (!effects["nothing"].LoadFromFile("datas/post-fx/nothing.sfx"))   return EXIT_FAILURE;
    if (!effects["blur"].LoadFromFile("datas/post-fx/blur.sfx"))         return EXIT_FAILURE;
    if (!effects["colorize"].LoadFromFile("datas/post-fx/colorize.sfx")) return EXIT_FAILURE;
    if (!effects["fisheye"].LoadFromFile("datas/post-fx/fisheye.sfx"))   return EXIT_FAILURE;
    if (!effects["wave"].LoadFromFile("datas/post-fx/wave.sfx"))         return EXIT_FAILURE;
    if (!effects["pixelate"].LoadFromFile("datas/post-fx/pixelate.sfx")) return EXIT_FAILURE;
    std::map<std::string, sf::PostFX>::iterator currentEffect = effects.find("nothing");

    // Do specific initializations
    effects["nothing"].SetTexture("framebuffer", NULL);
    effects["blur"].SetTexture("framebuffer", NULL);
    effects["blur"].SetParameter("offset", 0.f);
    effects["colorize"].SetTexture("framebuffer", NULL);
    effects["colorize"].SetParameter("color", 1.f, 1.f, 1.f);
    effects["fisheye"].SetTexture("framebuffer", NULL);
    effects["wave"].SetTexture("framebuffer", NULL);
    effects["wave"].SetTexture("wave", &waveImage);
    effects["pixelate"].SetTexture("framebuffer", NULL);

    // Define a string for displaying current effect description
    sf::String curFXStr;
    curFXStr.SetText("Current effect is \"" + currentEffect->first + "\"");
    curFXStr.SetFont(font);
    curFXStr.SetPosition(20.f, 0.f);
    curFXStr.SetColor(sf::Color(150, 70, 110));

    // Define a string for displaying help
    sf::String infoStr;
    infoStr.SetText("Move your mouse to change the effect parameters\nPress numpad + and - to change effect\nWarning : some effects may not work\ndepending on your graphics card");
    infoStr.SetFont(font);
    infoStr.SetPosition(20.f, 460.f);
    infoStr.SetColor(sf::Color(200, 100, 150));

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

                // Add key : next effect
                if (event.Key.Code == sf::Key::Add)
                {
                    currentEffect++;
                    if (currentEffect == effects.end())
                        currentEffect = effects.begin();
                    curFXStr.SetText("Current effect is \"" + currentEffect->first + "\"");
                }

                // Subtract key : previous effect
                if (event.Key.Code == sf::Key::Subtract)
                {
                    if (currentEffect == effects.begin())
                        currentEffect = effects.end();
                    currentEffect--;
                    curFXStr.SetText("Current effect is \"" + currentEffect->first + "\"");
                }
            }
        }

        // Get the mouse position in the range [0, 1]
        float mouseX = window.GetInput().GetMouseX() / static_cast<float>(window.GetWidth());
        float mouseY = window.GetInput().GetMouseY() / static_cast<float>(window.GetHeight());

        // Update the current effect
        if      (currentEffect->first == "blur")     currentEffect->second.SetParameter("offset", mouseX * mouseY * 0.05f);
        else if (currentEffect->first == "colorize") currentEffect->second.SetParameter("color", 0.3f, mouseX, mouseY);
        else if (currentEffect->first == "fisheye")  currentEffect->second.SetParameter("mouse", mouseX, 1.f - mouseY);
        else if (currentEffect->first == "wave")     currentEffect->second.SetParameter("offset", mouseX, mouseY);
        else if (currentEffect->first == "pixelate") currentEffect->second.SetParameter("mouse", mouseX, mouseY);

        // Animate the sprite
        float entityX = (cos(clock.GetElapsedTime() * 1.3f) + 1.2f) * 300;
        float entityY = (cos(clock.GetElapsedTime() * 0.8f) + 1.2f) * 200;
        entity.SetPosition(entityX, entityY);
        entity.Rotate(window.GetFrameTime() * 100);

        // Clear the window
        window.Clear();

        // Draw background, sprite and apply the post-fx
        window.Draw(background);
        window.Draw(entity);
        window.Draw(currentEffect->second);

        // Draw interface strings
        window.Draw(curFXStr);
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
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML PostFX");

    // Define a string for displaying the error message
    sf::String error("Sorry, your system doesn't support post-effects");
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
