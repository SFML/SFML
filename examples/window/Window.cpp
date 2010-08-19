
////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>


////////////////////////////////////////////////////////////
/// Entry point of application
///
/// \return Application exit code
///
////////////////////////////////////////////////////////////
int main()
{
    // Create the main window
    sf::Window window(sf::VideoMode(640, 480, 32), "SFML Window");

    // Create a clock for measuring the time elapsed
    sf::Clock clock;

    // Set the color and depth clear values
    glClearDepth(1.f);
    glClearColor(0.f, 0.f, 0.f, 0.f);

    // Enable Z-buffer read and write
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);

    // Setup a perspective projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90.f, 1.f, 1.f, 500.f);

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

            // Resize event : adjust viewport
            if (event.Type == sf::Event::Resized)
                glViewport(0, 0, event.Size.Width, event.Size.Height);
       }

        // Activate the window before using OpenGL commands.
        // This is useless here because we have only one window which is
        // always the active one, but don't forget it if you use multiple windows
        window.SetActive();

        // Clear color and depth buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Apply some transformations
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glTranslatef(0.f, 0.f, -200.f);
        glRotatef(clock.GetElapsedTime() * 50, 1.f, 0.f, 0.f);
        glRotatef(clock.GetElapsedTime() * 30, 0.f, 1.f, 0.f);
        glRotatef(clock.GetElapsedTime() * 90, 0.f, 0.f, 1.f);

        // Draw a cube
        glBegin(GL_QUADS);

            glColor3f(1.f, 0.f, 0.f);
            glVertex3f(-50.f, -50.f, -50.f);
            glVertex3f(-50.f,  50.f, -50.f);
            glVertex3f( 50.f,  50.f, -50.f);
            glVertex3f( 50.f, -50.f, -50.f);

            glColor3f(1.f, 0.f, 0.f);
            glVertex3f(-50.f, -50.f, 50.f);
            glVertex3f(-50.f,  50.f, 50.f);
            glVertex3f( 50.f,  50.f, 50.f);
            glVertex3f( 50.f, -50.f, 50.f);

            glColor3f(0.f, 1.f, 0.f);
            glVertex3f(-50.f, -50.f, -50.f);
            glVertex3f(-50.f,  50.f, -50.f);
            glVertex3f(-50.f,  50.f,  50.f);
            glVertex3f(-50.f, -50.f,  50.f);

            glColor3f(0.f, 1.f, 0.f);
            glVertex3f(50.f, -50.f, -50.f);
            glVertex3f(50.f,  50.f, -50.f);
            glVertex3f(50.f,  50.f,  50.f);
            glVertex3f(50.f, -50.f,  50.f);

            glColor3f(0.f, 0.f, 1.f);
            glVertex3f(-50.f, -50.f,  50.f);
            glVertex3f(-50.f, -50.f, -50.f);
            glVertex3f( 50.f, -50.f, -50.f);
            glVertex3f( 50.f, -50.f,  50.f);

            glColor3f(0.f, 0.f, 1.f);
            glVertex3f(-50.f, 50.f,  50.f);
            glVertex3f(-50.f, 50.f, -50.f);
            glVertex3f( 50.f, 50.f, -50.f);
            glVertex3f( 50.f, 50.f,  50.f);

        glEnd();

        // Finally, display the rendered frame on screen
        window.Display();
    }

    return EXIT_SUCCESS;
}
