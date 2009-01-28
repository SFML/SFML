
////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics.hpp>
#include <map>

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
    sf::RenderWindow App(sf::VideoMode(800, 600), "SFML PostFX");

    // Load a cute background image to display :)
    sf::Image BackgroundImage;
    if (!BackgroundImage.LoadFromFile("datas/post-fx/background.jpg"))
        return EXIT_FAILURE;
    sf::Sprite Background(BackgroundImage);

    // Load the text font
    sf::Font Cheeseburger;
    if (!Cheeseburger.LoadFromFile("datas/post-fx/cheeseburger.ttf"))
        return EXIT_FAILURE;

    // Load the image needed for the wave effect
    sf::Image WaveImage;
    if (!WaveImage.LoadFromFile("datas/post-fx/wave.jpg"))
        return EXIT_FAILURE;

    // Load all effects
    std::map<std::string, sf::PostFX> Effects;
    if (!Effects["nothing"].LoadFromFile("datas/post-fx/nothing.sfx"))   return EXIT_FAILURE;
    if (!Effects["blur"].LoadFromFile("datas/post-fx/blur.sfx"))         return EXIT_FAILURE;
    if (!Effects["colorize"].LoadFromFile("datas/post-fx/colorize.sfx")) return EXIT_FAILURE;
    if (!Effects["fisheye"].LoadFromFile("datas/post-fx/fisheye.sfx"))   return EXIT_FAILURE;
    if (!Effects["wave"].LoadFromFile("datas/post-fx/wave.sfx"))         return EXIT_FAILURE;
    std::map<std::string, sf::PostFX>::iterator CurrentEffect = Effects.find("nothing");

    // Do specific initializations
    Effects["nothing"].SetTexture("framebuffer", NULL);
    Effects["blur"].SetTexture("framebuffer", NULL);
    Effects["blur"].SetParameter("offset", 0.f);
    Effects["colorize"].SetTexture("framebuffer", NULL);
    Effects["colorize"].SetParameter("color", 1.f, 1.f, 1.f);
    Effects["fisheye"].SetTexture("framebuffer", NULL);
    Effects["wave"].SetTexture("framebuffer", NULL);
    Effects["wave"].SetTexture("wave", &WaveImage);

    // Define a string for displaying current effect description
    sf::String CurFXStr;
    CurFXStr.SetText("Current effect is \"" + CurrentEffect->first + "\"");
    CurFXStr.SetFont(Cheeseburger);
    CurFXStr.SetPosition(20.f, 0.f);

    // Define a string for displaying help
    sf::String InfoStr;
    InfoStr.SetText("Move your mouse to change the effect parameters\nPress numpad + and - to change effect\nWarning : some effects may not work\ndepending on your graphics card");
    InfoStr.SetFont(Cheeseburger);
    InfoStr.SetPosition(20.f, 460.f);
    InfoStr.SetColor(sf::Color(200, 100, 150));

    // Start the game loop
    while (App.IsOpened())
    {
        // Process events
        sf::Event Event;
        while (App.GetEvent(Event))
        {
            // Close window : exit
            if (Event.Type == sf::Event::Closed)
                App.Close();

            if (Event.Type == sf::Event::KeyPressed)
            {
                // Escape key : exit
                if (Event.Key.Code == sf::Key::Escape)
                    App.Close();

                // Add key : next effect
                if (Event.Key.Code == sf::Key::Add)
                {
                    CurrentEffect++;
                    if (CurrentEffect == Effects.end())
                        CurrentEffect = Effects.begin();
                    CurFXStr.SetText("Current effect is \"" + CurrentEffect->first + "\"");
                }

                // Subtract key : previous effect
                if (Event.Key.Code == sf::Key::Subtract)
                {
                    if (CurrentEffect == Effects.begin())
                        CurrentEffect = Effects.end();
                    CurrentEffect--;
                    CurFXStr.SetText("Current effect is \"" + CurrentEffect->first + "\"");
                }
            }
        }

        // Get the mouse position in the range [0, 1]
        float X = App.GetInput().GetMouseX() / static_cast<float>(App.GetWidth());
        float Y = App.GetInput().GetMouseY() / static_cast<float>(App.GetHeight());

        // Update the current effect
        if      (CurrentEffect->first == "blur")     CurrentEffect->second.SetParameter("offset", X * Y * 0.1f);
        else if (CurrentEffect->first == "colorize") CurrentEffect->second.SetParameter("color", 0.3f, X, Y);
        else if (CurrentEffect->first == "fisheye")  CurrentEffect->second.SetParameter("mouse", X, 1.f - Y);
        else if (CurrentEffect->first == "wave")     CurrentEffect->second.SetParameter("offset", X, Y);

        // Clear the window
        App.Clear();

        // Draw background and apply the post-fx
        App.Draw(Background);
        App.Draw(CurrentEffect->second);

        // Draw interface strings
        App.Draw(CurFXStr);
        App.Draw(InfoStr);

        // Finally, display the rendered frame on screen
        App.Display();
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
    sf::RenderWindow App(sf::VideoMode(800, 600), "SFML PostFX");

    // Define a string for displaying the error message
    sf::String ErrorStr("Sorry, your system doesn't support post-effects");
    ErrorStr.SetPosition(100.f, 250.f);
    ErrorStr.SetColor(sf::Color(200, 100, 150));

    // Start the game loop
    bool Running = true;
    while (Running)
    {
        // Process events
        sf::Event Event;
        while (App.GetEvent(Event))
        {
            // Close window : exit
            if (Event.Type == sf::Event::Closed)
                Running = false;

            // Escape key : exit
            if ((Event.Type == sf::Event::KeyPressed) && (Event.Key.Code == sf::Key::Escape))
                Running = false;
        }

        // Clear the window
        App.Clear();

        // Draw the error message
        App.Draw(ErrorStr);

        // Finally, display the rendered frame on screen
        App.Display();
    }
}
