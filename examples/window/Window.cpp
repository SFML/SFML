
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
    sf::Window window(sf::VideoMode(640, 480, 32), "SFML Window", sf::Style::Default, sf::ContextSettings(32));

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
    while (window.isOpen())
    {
        // Process events
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Close window : exit
            if (event.type == sf::Event::Closed)
                window.close();

            // Escape key : exit
            if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape))
                window.close();

            // Resize event : adjust viewport
            if (event.type == sf::Event::Resized)
                glViewport(0, 0, event.size.width, event.size.height);
       }

        // Activate the window before using OpenGL commands.
        // This is useless here because we have only one window which is
        // always the active one, but don't forget it if you use multiple windows
        window.setActive();

        // Clear color and depth buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Apply some transformations
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glTranslatef(0.f, 0.f, -200.f);
        glRotatef(clock.getElapsedTime().asSeconds() * 50, 1.f, 0.f, 0.f);
        glRotatef(clock.getElapsedTime().asSeconds() * 30, 0.f, 1.f, 0.f);
        glRotatef(clock.getElapsedTime().asSeconds() * 90, 0.f, 0.f, 1.f);

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
        window.display();
    }

    return EXIT_SUCCESS;
}
