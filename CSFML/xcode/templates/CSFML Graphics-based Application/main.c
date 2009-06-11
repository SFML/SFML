
////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics.h>


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
    sfRenderWindow *App = sfRenderWindow_Create (mode, "CSFML Graphics", sfClose, settings);
	sfShape *Shape = sfShape_CreateCircle(200, 200, 100, sfYellow,10.f, sfBlue);

    // Start game loop
    while (sfRenderWindow_IsOpened(App))
    {
        // Process events
        sfEvent Event;
        while (sfRenderWindow_GetEvent(App, &Event))
        {
            // Close window : exit
            if (Event.Type == sfEvtClosed)
                sfRenderWindow_Close(App);
        }

        // Clear screen
        sfRenderWindow_Clear(App, sfBlack);

        // Draw shape
        sfRenderWindow_DrawShape(App, Shape);

        // Finally, display the rendered frame on screen
        sfRenderWindow_Display(App);
    }
	
	// Destroy our objects
	sfShape_Destroy(Shape);
	sfRenderWindow_Destroy(App);

    return EXIT_SUCCESS;
}
