
////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Window.hpp>


////////////////////////////////////////////////////////////
/// Entry point of application
///
/// \return Application exit code
///
////////////////////////////////////////////////////////////
int main()
{
    // Create the main window
    sf::Window App(sf::VideoMode(640, 480, 32), "SFML Window");
	
    // Start game loop
    while (App.IsOpened())
    {
        // Process events
        sf::Event Event;
        while (App.GetEvent(Event))
        {
            // Close window : exit
            if (Event.Type == sf::Event::Closed)
                App.Close();
			
            // Escape key : exit
            if ((Event.Type == sf::Event::KeyPressed) && (Event.Key.Code == sf::Key::Escape))
                App.Close();
			
            // Resize event : adjust viewport
            if (Event.Type == sf::Event::Resized)
                glViewport(0, 0, Event.Size.Width, Event.Size.Height);
        }
		
        // Set the active window before using OpenGL commands
        // It's useless here because active window is always the same,
        // but don't forget it if you use multiple windows or controls
        App.SetActive();
		
        // Clear color buffer
        glClear(GL_COLOR_BUFFER_BIT);
		
        // Your drawing here...
		
        // Finally, display rendered frame on screen
        App.Display();
    }
	
    return EXIT_SUCCESS;
}
