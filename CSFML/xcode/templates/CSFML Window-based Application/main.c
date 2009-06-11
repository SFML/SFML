
////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Window.h>


////////////////////////////////////////////////////////////
/// Entry point of application
///
/// \return Application exit code
///
////////////////////////////////////////////////////////////
int main()
{
    // Create main window
	sfVideoMode mode = {640, 480, 32};
	sfWindowSettings settings = {24, 8, 0};
    sfWindow *App = sfWindow_Create (mode, "CSFML Window", sfClose, settings);

    // Start game loop
    while (sfWindow_IsOpened(App))
    {
        // Process events
        sfEvent Event;
        while (sfWindow_GetEvent(App, &Event))
        {
            // Close window : exit
            if (Event.Type == sfEvtClosed)
                sfWindow_Close(App);
			
			// Escape key : exit
            if ((Event.Type == sfEvtKeyPressed) && (Event.Key.Code == sfKeyEscape))
                sfWindow_Close(App);
			
            // Resize event : adjust viewport
            if (Event.Type == sfEvtResized)
                glViewport(0, 0, Event.Size.Width, Event.Size.Height);
        }

		// Set the active window before using OpenGL commands
        // It's useless here because active window is always the same,
        // but don't forget it if you use multiple windows or controls
        sfWindow_SetActive(App, sfTrue);
		
        // Clear color buffer
        glClear(GL_COLOR_BUFFER_BIT);
		
        // Your drawing here...
		
        // Finally, display rendered frame on screen
        sfWindow_Display(App);
    }
	
	// Destroy our window
	sfWindow_Destroy(App);

    return EXIT_SUCCESS;
}
