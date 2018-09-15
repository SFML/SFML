
////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Window.hpp>
#include <SFML/System/Err.hpp>
#include <SFML/OpenGL.hpp>
#include <X11/Xlib.h>
#include <iostream>
#include <cmath>


////////////////////////////////////////////////////////////
/// Initialize OpenGL states into the specified view
///
/// \param Window Target window to initialize
///
////////////////////////////////////////////////////////////
void initialize(sf::Window& window)
{
    // Activate the window
    window.setActive();

    // Setup OpenGL states
    // Set color and depth clear value
    glClearDepth(1.f);
    glClearColor(0.f, 0.5f, 0.5f, 0.f);

    // Enable Z-buffer read and write
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);

    // Setup a perspective projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    static const double pi = 3.141592654;
    GLdouble extent = std::tan(90.0 * pi / 360.0);
    glFrustum(-extent, extent, -extent, extent, 1.0, 500.0);

    // Enable position and texture coordinates vertex components
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
}

////////////////////////////////////////////////////////////
/// Draw the OpenGL scene (a rotating cube) into
/// the specified view
///
/// \param window      Target window for rendering
/// \param elapsedTime Time elapsed since the last draw
///
////////////////////////////////////////////////////////////
void draw(sf::Window& window, float elapsedTime)
{
    // Activate the window
    window.setActive();

    // Clear color and depth buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Apply some transformations
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.f, 0.f, -200.f);
    glRotatef(elapsedTime * 10.f, 1.f, 0.f, 0.f);
    glRotatef(elapsedTime * 6.f, 0.f, 1.f, 0.f);
    glRotatef(elapsedTime * 18.f, 0.f, 0.f, 1.f);

    // Define a 3D cube (6 faces made of 2 triangles composed by 3 vertices)
    static const GLfloat cube[] =
    {
        // positions    // colors
        -50, -50, -50,  1, 1, 0,
        -50,  50, -50,  1, 1, 0,
        -50, -50,  50,  1, 1, 0,
        -50, -50,  50,  1, 1, 0,
        -50,  50, -50,  1, 1, 0,
        -50,  50,  50,  1, 1, 0,

         50, -50, -50,  1, 1, 0,
         50,  50, -50,  1, 1, 0,
         50, -50,  50,  1, 1, 0,
         50, -50,  50,  1, 1, 0,
         50,  50, -50,  1, 1, 0,
         50,  50,  50,  1, 1, 0,

        -50, -50, -50,  1, 0, 1,
         50, -50, -50,  1, 0, 1,
        -50, -50,  50,  1, 0, 1,
        -50, -50,  50,  1, 0, 1,
         50, -50, -50,  1, 0, 1,
         50, -50,  50,  1, 0, 1,

        -50,  50, -50,  1, 0, 1,
         50,  50, -50,  1, 0, 1,
        -50,  50,  50,  1, 0, 1,
        -50,  50,  50,  1, 0, 1,
         50,  50, -50,  1, 0, 1,
         50,  50,  50,  1, 0, 1,

        -50, -50, -50,  0, 1, 1,
         50, -50, -50,  0, 1, 1,
        -50,  50, -50,  0, 1, 1,
        -50,  50, -50,  0, 1, 1,
         50, -50, -50,  0, 1, 1,
         50,  50, -50,  0, 1, 1,

        -50, -50,  50,  0, 1, 1,
         50, -50,  50,  0, 1, 1,
        -50,  50,  50,  0, 1, 1,
        -50,  50,  50,  0, 1, 1,
         50, -50,  50,  0, 1, 1,
         50,  50,  50,  0, 1, 1
    };

    // Draw the cube
    glVertexPointer(3, GL_FLOAT, 6 * sizeof(GLfloat), cube);
    glColorPointer(3, GL_FLOAT, 6 * sizeof(GLfloat), cube + 3);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}


////////////////////////////////////////////////////////////
/// Entry point of application
///
/// \return Error code
///
////////////////////////////////////////////////////////////
int main()
{
    // Open a connection with the X server
    Display* display = XOpenDisplay(NULL);
    if (!display)
        return EXIT_FAILURE;

    // Get the default screen
    int screen = DefaultScreen(display);

    // Let's create the main window
    XSetWindowAttributes attributes;
    attributes.background_pixel = BlackPixel(display, screen);
    attributes.event_mask       = KeyPressMask;
    Window window = XCreateWindow(display, RootWindow(display, screen),
                                  0, 0, 650, 330, 0,
                                  DefaultDepth(display, screen),
                                  InputOutput,
                                  DefaultVisual(display, screen),
                                  CWBackPixel | CWEventMask, &attributes);
    if (!window)
        return EXIT_FAILURE;

    // Set the window's name
    XStoreName(display, window , "SFML Window");

    // Let's create the windows which will serve as containers for our SFML views
    Window view1 = XCreateWindow(display, window,
                                 10, 10, 310, 310, 0,
                                 DefaultDepth(display, screen),
                                 InputOutput,
                                 DefaultVisual(display, screen),
                                 0, NULL);
    Window view2 = XCreateWindow(display, window,
                                 330, 10, 310, 310, 0,
                                 DefaultDepth(display, screen),
                                 InputOutput,
                                 DefaultVisual(display, screen),
                                 0, NULL);

    // Show our windows
    XMapWindow(display, window);
    XFlush(display);

    // Create our SFML views
    sf::Window sfmlView1(view1);
    sf::Window sfmlView2(view2);

    // Create a clock for measuring elapsed time
    sf::Clock clock;

    // Initialize our views
    initialize(sfmlView1);
    initialize(sfmlView2);

    // Start the event loop
    bool running = true;
    while (running)
    {
        while (XPending(display))
        {
            // Get the next pending event
            XEvent event;
            XNextEvent(display, &event);

            // Process it
            switch (event.type)
            {
                // Any key is pressed: quit
                case KeyPress:
                    running = false;
                    break;
            }
        }

        // Draw something into our views
        draw(sfmlView1, clock.getElapsedTime().asSeconds());
        draw(sfmlView2, clock.getElapsedTime().asSeconds() * 0.3f);

        // Display the views on screen
        sfmlView1.display();
        sfmlView2.display();
    }

    // Close our SFML views before destroying the underlying window
    sfmlView1.close();
    sfmlView2.close();

    // Destroy the window
    XDestroyWindow(display, window);

    // Close the display
    XCloseDisplay(display);

    return EXIT_SUCCESS;
}
